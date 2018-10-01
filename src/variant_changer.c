/*!
 * Variant changer through IPC
 * Author : Muhammad Al Azhari 
*/
#include <variant_changer.h>
unsigned char Gdbus_data[5];
pthread_mutex_t lock;

void Usage(char *filename) {
	printf("Usage: %s <file> <string>\n", filename);
	printf("version 1.0 Copyright(c) sorserve.rnd.clarion.com.my\n", filename);
}

char *reserve;
int variant_id;
char oem_info[30];
int line_num = 1;

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

void lcase(char s[]) {
   FUNCTION_IN();
   int c = 0;
 
   while (s[c] != '\0') {
      if (s[c] >= 'A' && s[c] <= 'Z') {
         s[c] = s[c] + 32;
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
	unsigned char buff[5]={1,0,0,0,0};
	unsigned char version[5];
	int rlen=5,glen=1;
	int update = 0;
	int info = 0 ;
	
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
	
	MonitorDbusConnection_Type_Array(buff,glen,setSoftwareVersion_name,Gdbus_data,rlen,resSoftwareVersion_name);
	oem = oem_variant(oem_info);
	variant_id = get_variant();

	oem_info[strcspn(oem_info, "\n")] = 0; //remove \n in string
	
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
			VAR_INFO_G(" Current OEM name    :: "ANSI_COLOR_MAGENTA"%s\n",oem_info);
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
				if (pthread_mutex_init(&lock, NULL) != 0)
				{
					printf("\n mutex init has failed\n");
					return 1;
				}
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
