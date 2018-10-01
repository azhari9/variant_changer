/*!
 * dbus handler for send and receive info
 * Author : Muhammad Al Azhari 
*/
#include <variant_changer.h>

void CreateDbusConnection_Type_String(char *string,char *msg_name)
{
	DBusMessage *message;
	DBusConnection *connection;
	DBusError error;
	dbus_error_init (&error);

	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

	if(!connection)
	{
		dbus_error_free (&error);
		return;
	}

	message = dbus_message_new_signal("/com/clarion/ivi/vehicledata","com.clarion.ivi.vehicledata", msg_name);
	dbus_message_append_args(message, DBUS_TYPE_STRING,&string,DBUS_TYPE_INVALID);
	dbus_connection_send(connection, message, NULL);
	dbus_connection_flush(connection);
}

void CreateDbusConnection_Type_Array(unsigned char arr[], int msg_length, char *msg_name)
{
	FUNCTION_IN();
	DBusMessage *message;
	DBusConnection *connection;
	DBusError error;
	dbus_error_init (&error);

	int i;
	unsigned char *DBuff_data;
	DBuff_data = (unsigned char *)malloc(256);

	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

	if(!connection)
	{
		dbus_error_free (&error);
		return;
	}

	for(i=0; i<msg_length; i++)
	{
		DBuff_data[i] = arr[i];
	}

	message = dbus_message_new_signal("/com/clarion/ivi/vehicledata","com.clarion.ivi.vehicledata", msg_name);

	if(!dbus_message_append_args(message, DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &DBuff_data, msg_length, DBUS_TYPE_INVALID))
	{
		fprintf(stderr, "Error append arguments\n");
	}
	dbus_connection_send(connection, message, NULL);
	dbus_connection_flush(connection);

	FUNCTION_OUT();
}

unsigned char MonitorDbusConnection_Type_Array(unsigned char arr[], int msg_length, char *msg_name_s,unsigned char Buffer[],int bufferSize, char *msg_name)
{
	FUNCTION_IN();
	pthread_mutex_lock(&lock);
	#ifdef DEBUG
		printf(ANSI_COLOR_YELLOW"Function Lock\n"ANSI_COLOR_RESET);
	#endif
	DBusMessage *message;
	DBusConnection *connection; 
	DBusError error;
	dbus_error_init (&error);
	
	int i,count=0,counter=0,ret;
	unsigned char *pBuffer;
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	if(!connection)
	{
		dbus_error_free (&error);
		return;
	}


	// request our name on the bus and check for errors
   ret = dbus_bus_request_name(connection, "clarion.brace.ipc", DBUS_NAME_FLAG_REPLACE_EXISTING , &error);
   if (dbus_error_is_set(&error)) { 
      fprintf(stderr, "Name Error (%s)\n", error.message);
      dbus_error_free(&error); 
   }

   // add a rule for which messages we want to see
   dbus_bus_add_match(connection, "type='signal',interface='clarion.brace.ipc'", &error); // see signals from the given interface
   dbus_connection_flush(connection);
   if (dbus_error_is_set(&error)) { 
      fprintf(stderr, "Match Error (%s)\n", error.message);
   }
   #ifdef DEBUG
   printf("Match rule sent\n");
   printf("msg_name_s = %s \nmsg_name = %s\n",msg_name_s,msg_name); 
   #endif
   
	 while (count < 10 || counter < 10) {

		
      // non blocking read of the next available message
      dbus_connection_read_write(connection, 0);
      message = dbus_connection_pop_message(connection);

      // loop again if we haven't read a message
		 if (NULL == message) { 
         usleep(100);
		 counter++;
         continue;
		 fprintf(stderr, "Message Null\n");
        }
		
		// check if the message is a signal from the correct interface and with the correct name
		CreateDbusConnection_Type_Array(arr,msg_length,msg_name_s);
		if (dbus_message_is_signal(message, "clarion.brace.ipc", msg_name)) 
		{
			
			dbus_message_get_args(message, &error, DBUS_TYPE_ARRAY,DBUS_TYPE_BYTE, &pBuffer, &bufferSize, DBUS_TYPE_INVALID);
			if (dbus_error_is_set(&error))
			{ 
				fprintf(stderr,"Message arguments are not extracted correctly: %s", error.message);
				dbus_error_free(&error);
			}
			else
			{
				#ifdef DEBUG
				printf("Got Signal with value req:%s res:%s\n", msg_name_s,msg_name);
				printf("[%s] pBuffer : ",msg_name);
				for(i = 0; i < bufferSize; i++)
				{
					printf("%d | ",pBuffer[i]);
				}
				printf("\n");
				
				#endif
				for(i = 0; i < bufferSize; i++)
				{
					Buffer[i]=pBuffer[i];
				}
				// free the message
				dbus_message_unref(message);
				
				break;
			}

		}
		else
		{
			count++;
			#ifdef DEBUG
				printf("[%s] [NOT RECEIVE] request from:%s respond for:%s [%d]\n",__func__, msg_name_s, msg_name,count);
			#endif
		}
		
      
   }
   pthread_mutex_unlock(&lock);
   FUNCTION_OUT();
   #ifdef DEBUG
   printf(ANSI_COLOR_YELLOW"Function Unlock\n"ANSI_COLOR_RESET);
   #endif
}

unsigned char Monitor_Clock_DbusConnection_Type_Byte(unsigned char clock[],char *msg_name)
{
	FUNCTION_IN();
	DBusMessage *message;
	DBusConnection *connection;
	DBusError error;
	dbus_error_init (&error);
	unsigned char *Buffer;
	
	int i,count=0,counter=0,ret;
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	if(!connection)
	{
		dbus_error_free (&error);
		return;
	}


	// request our name on the bus and check for errors
   ret = dbus_bus_request_name(connection, "clarion.brace.ipc", DBUS_NAME_FLAG_REPLACE_EXISTING , &error);
   if (dbus_error_is_set(&error)) { 
      fprintf(stderr, "Name Error (%s)\n", error.message);
      dbus_error_free(&error); 
   }

   // add a rule for which messages we want to see
   dbus_bus_add_match(connection, "type='signal',interface='clarion.brace.ipc'", &error); // see signals from the given interface
   dbus_connection_flush(connection);
   if (dbus_error_is_set(&error)) { 
      fprintf(stderr, "Match Error (%s)\n", error.message);
     // exit(1); 
   }
   #ifdef DEBUG
   printf("Match rule sent\n");
   #endif
	 while (count < 10 || counter < 10) {
      // non blocking read of the next available message
      dbus_connection_read_write(connection, 0);
      message = dbus_connection_pop_message(connection);

      // loop again if we haven't read a message
        if (NULL == message) { 
         usleep(100);
		 counter++;
         continue;
      }
	  else
	  {  
			 // check if the message is a signal from the correct interface and with the correct name
		  if (dbus_message_is_signal(message, "clarion.brace.ipc", msg_name)) 
		  {
			dbus_message_get_args(message, &error,DBUS_TYPE_BYTE, &clock[0],
												DBUS_TYPE_BYTE,&clock[1],
												DBUS_TYPE_BYTE,&clock[2],
												DBUS_TYPE_BYTE,&clock[3],
												DBUS_TYPE_BYTE,&clock[4],
												DBUS_TYPE_BYTE,&clock[5],
												DBUS_TYPE_BYTE,&clock[6], 
												DBUS_TYPE_INVALID);
			if (dbus_error_is_set(&error))
			{
				fprintf(stderr,"Message arguments are not extracted correctly: %s", error.message);
				dbus_error_free(&error);
			}
			else
			{
				#ifdef DEBUG
				printf("Buffer : %d:%d:%d %d/%d/%d%d\n",clock[0],clock[1],clock[2],clock[3],clock[4],clock[5],clock[6]);
				#endif
				switch(clock[0]){
					case 24:clock[0] = 0;break;
					case 25:clock[0] = 1;break;
					case 26:clock[0] = 2;break;
					case 27:clock[0] = 3;break;
					case 28:clock[0] = 4;break;
					case 29:clock[0] = 5;break;
					case 30:clock[0] = 6;break;
					case 31:clock[0] = 7;break;
				};
				
				dbus_message_unref(message);
				break;
			}

		  }
		  else
		  {
			  count++;
			   #ifdef DEBUG
			  printf("%s NOT RECEIVE [%d]\n",__func__,count);
			  printf("%s respond for:%s\n",__func__, msg_name);
			   #endif
			  dbus_message_unref(message);
			 
		  }
	  }
	}
   FUNCTION_OUT();

   return 0;
}

unsigned char Monitor_DbusConnection(unsigned char pBuffer[],int bufferSize,char *msg_name)
{
	FUNCTION_IN();
	DBusMessage *message;
	DBusConnection *connection;
	DBusError error;
	dbus_error_init (&error);
	unsigned char *Buffer;
	
	int i,count,counter,ret;
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	if(!connection)
	{
		dbus_error_free (&error);
		return;
	}


	// request our name on the bus and check for errors
   ret = dbus_bus_request_name(connection, "clarion.brace.ipc", DBUS_NAME_FLAG_REPLACE_EXISTING , &error);
   if (dbus_error_is_set(&error)) { 
      fprintf(stderr, "Name Error (%s)\n", error.message);
      dbus_error_free(&error); 
   }

   // add a rule for which messages we want to see
   dbus_bus_add_match(connection, "type='signal',interface='clarion.brace.ipc'", &error); // see signals from the given interface
   dbus_connection_flush(connection);
   if (dbus_error_is_set(&error)) { 
      fprintf(stderr, "Match Error (%s)\n", error.message);
     // exit(1); 
   }
   #ifdef DEBUG
   printf("Match rule sent\n");
   #endif
	 while (count < 10 || counter < 10) {
      // non blocking read of the next available message
      dbus_connection_read_write(connection, 0);
      message = dbus_connection_pop_message(connection);

      // loop again if we haven't read a message
        if (NULL == message) { 
         usleep(100);
		 counter++;
         continue;
      }
	  else
	  {  
			 // check if the message is a signal from the correct interface and with the correct name
		  if (dbus_message_is_signal(message, "clarion.brace.ipc", msg_name)) 
		  {
			dbus_message_get_args(message, &error, DBUS_TYPE_ARRAY,DBUS_TYPE_BYTE, &pBuffer, &bufferSize, DBUS_TYPE_INVALID);
			if (dbus_error_is_set(&error))
			{
				fprintf(stderr,"Message arguments are not extracted correctly: %s", error.message);
				dbus_error_free(&error);
			}
			else
			{
				dbus_message_unref(message);
				break;
			}

		  }
		  else
		  {
			  count++;
			   #ifdef DEBUG
			  printf("%s NOT RECEIVE [%d]\n",__func__,count);
			  dbus_message_unref(message);
			  #endif
		  }
	  }
	}
   FUNCTION_OUT();

   return 0;
}
unsigned char MonitorDbusConnection_Type_Single_Byte(unsigned char data,char *msg_name)
{
	FUNCTION_IN();
	DBusMessage *message;
	DBusConnection *connection;
	DBusError error;
	dbus_error_init (&error);
	unsigned char *Buffer;

	
	int i,count,counter,ret;
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	if(!connection)
	{
		dbus_error_free (&error);
		return;
	}


	// request our name on the bus and check for errors
   ret = dbus_bus_request_name(connection, "clarion.brace.ipc", DBUS_NAME_FLAG_REPLACE_EXISTING , &error);
   if (dbus_error_is_set(&error)) { 
      fprintf(stderr, "Name Error (%s)\n", error.message);
      dbus_error_free(&error); 
   }

   // add a rule for which messages we want to see
   dbus_bus_add_match(connection, "type='signal',interface='clarion.brace.ipc'", &error); // see signals from the given interface
   dbus_connection_flush(connection);
   if (dbus_error_is_set(&error)) { 
      fprintf(stderr, "Match Error (%s)\n", error.message);
     // exit(1); 
   }
   #ifdef DEBUG
   printf("Match rule sent\n");
   #endif
	// while (counter < 10 || count < 10) {

	  // non blocking read of the next available message
	  dbus_connection_read_write(connection, 0);
	  message = dbus_connection_pop_message(connection);

	  // loop again if we haven't read a message
	  /* if (NULL == message) 
	  { 
		 usleep(100);
		 counter++;
		 continue;
	  }
	  else
	  { */
			// check if the message is a signal from the correct interface and with the correct name
		  if (dbus_message_is_signal(message, "clarion.brace.ipc", msg_name)) 
		  {
			dbus_message_get_args(message, &error,DBUS_TYPE_BYTE, &data,DBUS_TYPE_INVALID);
			if (dbus_error_is_set(&error))
			{
				fprintf(stderr,"Message arguments are not extracted correctly: %s", error.message);
				dbus_error_free(&error);
			}
			else
			{
				#ifdef DEBUG
					printf("Buffer : %d\n",data);
				#endif
				
				dbus_message_unref(message);
				//break;
			}

		  }
		  else
		  {
			  count++;
			  #ifdef DEBUG
				printf("%s NOT RECEIVE [%d]\n",__func__,count);
				dbus_message_unref(message);
			  #endif
		  }
	  //}

   //}
   FUNCTION_OUT();

   return 0;
}