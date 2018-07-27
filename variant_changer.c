/*!
 * Variant changer through IPC
 * Author : Muhammad Al Azhari 
*/
#include <variant_changer.h>
unsigned char Gdbus_data[5];

void Usage(char *filename) {
	printf("Usage: %s <file> <string>\n", filename);
	printf("version 1.0 Copyright(c) sorserve.rnd.clarion.com.my\n", filename);
}

char *reserve;
int variant_id;
char oem_info[30];
int line_num = 1;

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

unsigned char MonitorDbusConnection_Type_Array(unsigned char Buffer[],int bufferSize, char *msg_name)
{
	FUNCTION_IN();
	DBusMessage *message;
	DBusConnection *connection;
	DBusError error;
	dbus_error_init (&error);
	
	
	int i,count,counter,ret;
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
     // exit(1); 
   }
   #ifdef DEBUG
   printf("Match rule sent\n");
   #endif
	 while (count < 100 || counter < 100) {

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
					#ifdef DEBUG
					printf("Got Signal with value %s\n", pBuffer);
					printf("pBuffer : ");
					for(i = 0; i < bufferSize; i++)
					{
						printf("%d | ",pBuffer[i]);
					}
					printf("\n");
					// free the message
					#endif
					for(i = 0; i < bufferSize; i++)
					{
						Buffer[i]=pBuffer[i];
					}
					dbus_message_unref(message);
					
					break;
				}

			}
			else
			{
				count++;
				#ifdef DEBUG
					printf("%s NOT RECEIVE [%d]\n",__func__,count);
				#endif
			}
		}
      
   }
   FUNCTION_OUT();
}

unsigned char MonitorDbusConnection_Type_Byte(unsigned char clock[],char *msg_name)
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
				clock[0]=clock[0]+8;
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
	 while (counter < 10 || count < 10) {

	  // non blocking read of the next available message
	  dbus_connection_read_write(connection, 0);
	  message = dbus_connection_pop_message(connection);

	  // loop again if we haven't read a message
	  if (NULL == message) 
	  { 
		 usleep(100);
		 counter++;
		 continue;
	  }
	  else
	  {
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

void ucase(char s[]) {
   FUNCTION_IN();
   int c = 0;
 
   while (s[c] != '\0') {
      if (s[c] >= 'a' && s[c] <= 'z') {
         s[c] = s[c] - 32;
      }
      c++;
   }
   FUNCTION_OUT();
}

int oem_variant(char str_oem_info[])
{
	FUNCTION_IN();
	FILE *fp_oem,*fp_oem_info;
	int MAXCHAR = 30;
    	char str_oem[MAXCHAR];
	int oem_num;

	char* oem_file = "/sys/kernel/vehicle_variant/oem";
	char* oem_info_file = "/sys/kernel/vehicle_variant/oem_info";
	// char* oem_file = "/home/root/oem";
	// char* oem_info_file = "/home/root/oem_info";
 
	if( ( fp_oem_info = fopen(oem_info_file, "rb" ) ) == NULL ) // checks to see if file exists
    { 
        return -1; 
    }
	else
	{
		while (fgets(str_oem_info, MAXCHAR, fp_oem_info) != NULL)
		ucase(str_oem_info);
		fclose(fp_oem_info);
	}
    
	if( ( fp_oem = fopen(oem_file, "rb" ) ) == NULL ) // checks to see if file exists
    { 
        return -1; 
    }
	else
	{
		while (fgets(str_oem, MAXCHAR, fp_oem) != NULL)
		oem_num = atoi(str_oem);
		fclose(fp_oem);
	}
    FUNCTION_OUT();
	return oem_num;
}

int get_variant(void)
{
	FUNCTION_IN();
	FILE *fp_variant;
	int MAXCHAR = 30;
	char str_variant[MAXCHAR];
	int variant_num;

	char* variant_file = "/sys/kernel/vehicle_variant/variant_req";
    
	if( ( fp_variant = fopen(variant_file, "rb" ) ) == NULL ) // checks to see if file exists
    { 
        return -1; 
    }
	else
	{
		while (fgets(str_variant, MAXCHAR, fp_variant) != NULL)
		variant_num = atoi(str_variant);
		fclose(fp_variant);
	}
    FUNCTION_OUT();
	return variant_num;
}

int variant_status(int var_id)
{
	FUNCTION_IN();
	char *fname="/usr/share/variantconf/variant.conf";
	FILE *file = fopen(fname, "r");
	int count = 0;
	int current_var;
	char *reserve_id;
	int status;
	int selected_var = line_num+var_id-1;

	if ( file != NULL )
	{
		char line[256]; /* or other suitable maximum line size */
		while (fgets(line, sizeof(line), file) != NULL) /* read a line */
		{
			if (count >= line_num)
			{
				if(count == selected_var )
				{
					reserve_id = strstr(line,"RESERVE");
					if(reserve_id == NULL)
						return 1; //not reserve variant
					else
						return 2; //reserve variant
				}
				
				if(count == line_num+14)
				{
					break;
				}
			
			}
			count++;
		}

		fclose(file);
	}
	else
	{
		//file doesn't exist
	}
	FUNCTION_OUT();
	return 0;
}

void variant_info(char *fname,int lineNumber)
{
	FUNCTION_IN();
	FILE *file = fopen(fname, "r");
	int count = 0;
	int current_var;
	
	current_var = lineNumber+variant_id-1;
	#ifdef CURSOR_CTRL
	system("clear && printf '\e[3J'");
	#else
	system("clear");
	#endif
	
	printf(ANSI_COLOR_RED   "Creating variant will automatically delete snapshot and re-create the snapshot image!"     ANSI_COLOR_RESET "\n" );
	printf(ANSI_COLOR_RED   "Current vehicle oem : %s"     ANSI_COLOR_RESET "\n",oem_info);
	
	if ( file != NULL )
	{
		char line[256]; /* or other suitable maximum line size */
		while (fgets(line, sizeof(line), file) != NULL) /* read a line */
		{
			if (count >= lineNumber)
			{

				reserve = strstr(line,"RESERVE");
				if(reserve == NULL)
				{
					if(count == current_var)
					{
						VAR_INFO_C(">%s",line);
					}
					else
					{
						VAR_INFO_G("%s",line);
					}
					
				}
				else
				{
					if(count == current_var)
					{
						VAR_INFO_C(">%s",line);
					}
					else
					{
						VAR_INFO_Y("%s",line);
					}
				}
				if(count == lineNumber+14)
				{
					break;
				}
			
			}
			count++;
		}

		fclose(file);
	}
	else
	{
		//file doesn't exist
	}
	FUNCTION_OUT();
}

int Search_in_File(char str[]) {
	FUNCTION_IN();
	char *fname="/usr/share/variantconf/variant.conf";
	FILE *fp;
	int find_result = 0;
	char temp[512];
	
	//gcc users
	if((fp = fopen(fname, "r")) == NULL) {
		VAR_INFO_R("Cannot find %s\n",fname);
		VAR_INFO_R("Please insert variant configuration at %s to proceed!!\n",fname);
		//Usage(fname);
		return(-1);
	}

	while(fgets(temp, sizeof(temp), fp) != NULL) 
	{
		if((strstr(temp, str)) != NULL) {
			VAR_INFO_M("OEM :: %s\n", temp);
			find_result++;
			break;
		}
		line_num++;
	}
	
	
	if(find_result == 0) {
		printf("\nSorry, couldn't find a match.\n");
	}
	else
	{
		variant_info(fname,line_num);
	}
	
	if(fp) {
		fclose(fp);
	}
	FUNCTION_OUT();
   	return(0);
}

void set_variant(void)
{
	FUNCTION_IN();
	int unknown_variant=0;
	
	char *setvehicle_name = "R_SET_VEHICLE_VARIANT";
	int vehicle_type;
	unsigned char vehicle_data[1];
	int len = 1;
	char yes_no;
	unsigned int reserve_sts;
	
	printf("\n\n");
	printf(ANSI_COLOR_YELLOW "  Vehicle Variant Selection:\t"     ANSI_COLOR_RESET );
	scanf("%d",&vehicle_type);
	printf("\n");
	
	reserve_sts = variant_status(vehicle_type);
	if( vehicle_type < 1 || vehicle_type > 15)
	{
		printf("\n\n");
		printf(ANSI_COLOR_RED "  INVALID VARIANT ID!! [%d]                   "     ANSI_COLOR_RESET "\n\n",vehicle_type);
		unknown_variant = 1;
	}
	else if(reserve_sts == 2 && vehicle_type >= 1 && vehicle_type <= 15 )
	{
		printf(ANSI_COLOR_YELLOW "  Selection for RESERVE Variant %d           "     ANSI_COLOR_RESET "\n",vehicle_type );
		printf(ANSI_COLOR_YELLOW "  ##If this is not RESERVE Variant, please update /usr/share/variantconf/variant.conf"     ANSI_COLOR_RESET "\n\n",vehicle_type );
		printf(ANSI_COLOR_CYAN   "  do you want to proceed(Y/N)?\t"ANSI_COLOR_RESET);
		scanf(" %c",&yes_no);
		switch(yes_no)
		{
			case 'Y':
			case 'y':
					printf("\n");
					printf(ANSI_COLOR_YELLOW "  Proceed with reserved variant id %d           "     ANSI_COLOR_RESET "\n\n",vehicle_type );
					vehicle_data[0] = vehicle_type;
					CreateDbusConnection_Type_Array(vehicle_data,len,setvehicle_name);
					break;
			case 'N':
			case 'n':
					unknown_variant = 1;
					printf("\n\n" ANSI_COLOR_GREEN " Good Choice!!"ANSI_COLOR_YELLOW" :)" ANSI_COLOR_RESET "\n\n",vehicle_type );
					break;
			default :
					printf("\n\n");
					printf(ANSI_COLOR_RED "  INVALID ANSWER!! [%c]                   "     ANSI_COLOR_RESET "\n\n",yes_no);
					unknown_variant = 1;
					break;
		}
	}
	else if( vehicle_type == variant_id )
	{
		printf(ANSI_COLOR_YELLOW "  Selection for current used Variant :: %d           "     ANSI_COLOR_RESET "\n\n",vehicle_type );
		printf(ANSI_COLOR_CYAN   "  do you want to proceed(Y/N)?\t"ANSI_COLOR_RESET);
		scanf(" %c",&yes_no);
		switch(yes_no)
		{
			case 'Y':
			case 'y':
					printf("\n");
					printf(ANSI_COLOR_YELLOW "  Proceed with variant id %d           "     ANSI_COLOR_RESET "\n\n",vehicle_type );
					vehicle_data[0] = vehicle_type;
					CreateDbusConnection_Type_Array(vehicle_data,len,setvehicle_name);
					break;
			case 'N':
			case 'n':
					unknown_variant = 1;
					printf("\n\n" ANSI_COLOR_GREEN " Good Choice!!"ANSI_COLOR_YELLOW" :)" ANSI_COLOR_RESET "\n\n",vehicle_type );
					break;
			default :
					printf("\n\n");
					printf(ANSI_COLOR_RED "  INVALID ANSWER!! [%c]                   "     ANSI_COLOR_RESET "\n\n",yes_no);
					unknown_variant = 1;
					break;
		}
	}
	else
	{
		printf(ANSI_COLOR_CYAN "  Selection for variant %d               "     ANSI_COLOR_RESET "\n",vehicle_type );
		vehicle_data[0] = vehicle_type;
		CreateDbusConnection_Type_Array(vehicle_data,len,setvehicle_name);
	}
	
	printf("\n");
	if( unknown_variant == 1 )
	{
		printf(ANSI_COLOR_MAGENTA" Thank You :)"     ANSI_COLOR_RESET"\n");
		exit(0);
	}
	else
	{
		system("delete-snapshot");
		printf(ANSI_COLOR_RED   "Snapshot Partition has been deleted"     ANSI_COLOR_RESET "\n\n" );
		printf(ANSI_COLOR_RED   "System will reboot                "     ANSI_COLOR_RESET "\n\n" );
		printf("\n");
		exit(0);
	}
	FUNCTION_IN();
}

int main( int argc, char **argv)
{
	int i,ret;
	int oem;
	char yes_no;
	char *setSoftwareVersion_name = "R_SOFTVERS";
	char *resSoftwareVersion_name = "S_SOFTVERS";
	char *resCLOCK = "S_GET_CLOCK_SET";
	unsigned char Sdbus_data[5];
	int rlen=5,glen=1;
	int update = 0;
	int info = 0 ;
	unsigned char clock_show[7];
	if (argc == 2) {
      //to update software
	  if(0 == strcmp(argv[1], "pp_update"))
	   {
		   update = 1;
	   }
	   else if(0 == strcmp(argv[1], "pp_info"))
	   {
		   info = 1;
	   }
   }
   else
   {
	   // to variant changer
   }

	oem = oem_variant(oem_info);
	variant_id = get_variant();
	Sdbus_data[0]=1;
	CreateDbusConnection_Type_Array(Sdbus_data,glen,setSoftwareVersion_name);
	MonitorDbusConnection_Type_Array(Gdbus_data,rlen,resSoftwareVersion_name);
	
		if( oem == -1)
		{
			#ifdef CURSOR_CTRL
			system("clear && printf '\e[3J'");
			system("printf '\e[?25h'");
			#else
			system("clear");
			#endif
			printf("\n\n");
			VAR_INFO_G("        Variant Tools V1.0      \n");
			VAR_INFO_Y(" ===============================\n");
			VAR_INFO_G(" WELCOME To Variant Changer tool\n");
			VAR_INFO_Y(" ===============================\n\n");
			VAR_INFO_G(" PP Software Version  :: "ANSI_COLOR_MAGENTA"%d.%d\n",Gdbus_data[2],Gdbus_data[3]);
			VAR_INFO_G(" Hardware Version     :: "ANSI_COLOR_MAGENTA"%d.%d\n\n",Gdbus_data[0],Gdbus_data[1]);
			VAR_INFO_G(" Current OEM name    :: "ANSI_COLOR_MAGENTA"Not Availabe\n");
			VAR_INFO_G(" Current OEM No.     :: "ANSI_COLOR_MAGENTA"Not Availabe\n");
			VAR_INFO_G(" Current Variant No. :: "ANSI_COLOR_MAGENTA"%d\n",variant_id);
			VAR_INFO_Y(" ===============================\n\n");
		}
		else
		{
			#ifdef CURSOR_CTRL
			system("clear && printf '\e[3J'");
			system("printf '\e[?25h'");
			#else
			system("clear");
			#endif
			printf("\n\n");
			VAR_INFO_G("        Variant Tools V1.0      \n");
			VAR_INFO_Y(" ===============================\n");
			VAR_INFO_G(" WELCOME To Variant Changer tool\n");
			VAR_INFO_Y(" ===============================\n\n");
			VAR_INFO_G(" PP Software Version  :: "ANSI_COLOR_MAGENTA"%d.%d\n",Gdbus_data[2],Gdbus_data[3]);
			VAR_INFO_G(" Hardware Version     :: "ANSI_COLOR_MAGENTA"%d.%d\n\n",Gdbus_data[0],Gdbus_data[1]);
			VAR_INFO_G(" Current OEM name    :: "ANSI_COLOR_MAGENTA"%s",oem_info);
			VAR_INFO_G(" Current OEM No.     :: "ANSI_COLOR_MAGENTA"%d\n",oem);
			VAR_INFO_G(" Current Variant No. :: "ANSI_COLOR_MAGENTA"%d\n",variant_id);
			VAR_INFO_Y(" ===============================\n\n");
		}
		
		printf("Press <Enter> to proceed or <CTRL+C> to cancel\n");
		char c = getchar();
		if(c == '\n' && oem != -1 )
		{
			if ( update == 1 )
			{
				ret = pp_sw_update();
				if(ret < 0)
				{
					return ret;
				}
			}
			else if(info == 1)
			{
				init_ipc_test_app();
			}
			else
			{
				ret = Search_in_File(oem_info);
				if(ret == -1)
				{
					return ret;
				}
				else
				{
					set_variant();
				}
			}
			
			
		}
		else if(c == '\n')
		{
			if ( update == 1 )
			{
				ret = pp_sw_update();
				if(ret < 0)
				{
					return ret;
				}
			}
			else if(info == 1)
			{
				init_ipc_test_app();
			}
			else
			{
				//
			}
		}
		else
		{
			//
        }
	
	return 0;
}
