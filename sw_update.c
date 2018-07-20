#include <variant_changer.h>
char  path1[] = "/run/media/sda1/";
char  path2[] = "/run/media/sdb1/";

int pp_sw_update_process(char p_string[], char software_update_req_name[])
{
	
	if( access( p_string, F_OK ) != -1 ) 
	{
		system("clear");
		  printf(ANSI_COLOR_GREEN "                                     %s   exists \n\n" ANSI_COLOR_RESET,p_string);
		  printf(ANSI_COLOR_RED "                                START PP SW UPDATE, DO NOT TURN OFF YOUR UNIT!!!!!!!    "     ANSI_COLOR_RESET "\n\n" );
		  printf(ANSI_COLOR_RED "                                     IF YOU WANT to Cancel BU ON/OFF before 50%    "     ANSI_COLOR_RESET "\n\n\n" );
		CreateDbusConnection_Type_String(p_string,software_update_req_name);
		system("delete-snapshot");
		  printf(ANSI_COLOR_RED "                                           PLEASE WAIT, UPGRADING IN PROGRESS!!!!!!!    "     ANSI_COLOR_RESET "\n" );
		  printf(ANSI_COLOR_RED "                                PLEASE WAIT, UPGRADING IN PROGRESS!!!!!!!    "     ANSI_COLOR_RESET "\n" );
		  printf(ANSI_COLOR_RED "                                           PLEASE WAIT, UPGRADING IN PROGRESS!!!!!!!    "     ANSI_COLOR_RESET "\n" );
		  printf(ANSI_COLOR_RED "                                PLEASE WAIT, UPGRADING IN PROGRESS!!!!!!!    "     ANSI_COLOR_RESET "\n" );
		  printf(ANSI_COLOR_RED "                                           PLEASE WAIT, UPGRADING IN PROGRESS!!!!!!!    "     ANSI_COLOR_RESET "\n" );
printf("\n\n\n\n"ANSI_COLOR_RED "                              DO NOT TURN OFF!! SYSTEM WILL AUTOMATICALLY REBOOT!!!!!!!    "     ANSI_COLOR_RESET "\n" );
		  printf(ANSI_COLOR_CYAN "   ");
		system("dlt-receive -a localhost | grep \"Software Update Progress\" &");
		return 0;
	}
	else 
	{
		printf(ANSI_COLOR_RED "\n\n\n                              %s file doesn't exist!! " ANSI_COLOR_RESET,p_string) ;
		return -1;
	}
}

static int parse_ext(const struct dirent *dir)
   {
     if(!dir)
       return 0;

     if(dir->d_type == DT_REG) { /* only deal with regular file */
         const char *ext = strrchr(dir->d_name,'.');
         if((!ext) || (ext == dir->d_name))
           return 0;
         else {
           if(strcmp(ext, ".rom") == 0)
             return 1;
         }
     }

     return 0;
   }


int list_files(void)
{
       struct dirent **namelist;
       int n,i=1;

       n = scandir(path1, &namelist, parse_ext, alphasort);
       if (n < 0) {
		   n = scandir(path2, &namelist, parse_ext, alphasort);
		   if (n < 0) {
			   return -1;
		   }
           else if ( n == 0 )
			{
					return 1;
			}
		   else {
				   printf( ANSI_COLOR_CYAN "                              __________________________Available ROM Files at %s___________________" ANSI_COLOR_RESET "\n\n",path2);
			   while (n--) {
				   printf("                                                       %d) %s\n",i, namelist[n]->d_name);
				   free(namelist[n]);
					i++;
			   }
				   printf( ANSI_COLOR_CYAN "                              _______________________________________________________________\n" ANSI_COLOR_RESET);
			   free(namelist);
			   return 0;
		   }
       }
       else if ( n == 0 )
        {
                return 1;
        }
       else {
               printf( ANSI_COLOR_CYAN "                              __________________________Available ROM Files at %s___________________" ANSI_COLOR_RESET "\n\n",path1 );
           while (n--) {
               printf("                                                       %d) %s\n",i, namelist[n]->d_name);
               free(namelist[n]);
                i++;
           }
               printf( ANSI_COLOR_CYAN "                              _______________________________________________________________" ANSI_COLOR_RESET "\n");
           free(namelist);
		   return 0;
       }

}

int pp_sw_update(void)
{
	char *softwareupdatereq_name = "R_SOFT_UPDATE_REQ";
	char *softwareupdatestatus_name = "R_SOFT_UPDATE_STATUS";
	
	//SW UPDATE
	char  file[255];
	char * path_combine1; 
	char * path_combine2;
	int sw_update_option;
	int ret;
	//-------
	

				system("clear");
				printf(ANSI_COLOR_GREEN "                              Please put your rom file at this directory /run/media/{sda1 or sdb1}/player.rom    "     ANSI_COLOR_RESET "\n\n");
				printf(ANSI_COLOR_GREEN "                              SW Update file and path option    "     ANSI_COLOR_RESET "\n");
				printf(ANSI_COLOR_GREEN "                              1) Proceed with default file name (player.rom)"     ANSI_COLOR_RESET "\n");
				printf(ANSI_COLOR_GREEN "                              2) Change file name (eg: PP4017X.rom)   "     ANSI_COLOR_RESET "\n\n");
				printf(ANSI_COLOR_RED "                              Make sure you insert correct file, otherwise system will never bootup!!   "     ANSI_COLOR_RESET "\n");
				printf(ANSI_COLOR_YELLOW "                              Enter Option : "     ANSI_COLOR_RESET);
				scanf("%d",&sw_update_option);
				
				if( sw_update_option == 1 )
				{
					strcpy(file,"player.rom");
					
					path_combine1 = (char *) malloc(1 + strlen(path1)+ strlen(file) ); 
					path_combine2 = (char *) malloc(1 + strlen(path2)+ strlen(file) );
					
					strcpy(path_combine1,path1);
					strcat(path_combine1,file);
					strcpy(path_combine2,path2);
					strcat(path_combine2,file);
					
					ret = pp_sw_update_process(path_combine1,softwareupdatereq_name);
					if( ret != 0)
					{
						ret = pp_sw_update_process(path_combine2,softwareupdatereq_name);
						if( ret != 0)
						{
							sleep(3);
							return ret;
							
						}
						else
						{
							exit(0);
							return ret;
						}
					}
					else
					{
						exit(0);
						return ret;
					}
				}
				else if ( sw_update_option == 2 )
				{
					printf("\n\n");
					ret = list_files();
					if ( ret == -1 )
					{
						printf(ANSI_COLOR_RED "                              %s and %s not available\n"     ANSI_COLOR_RESET,path1,path2);
						sleep(3);
						return ret;
					}
					else if ( ret == 1)
					{
						printf(ANSI_COLOR_RED "                              .rom files not available at %s ond %s\n"     ANSI_COLOR_RESET,path1,path2);
						printf(ANSI_COLOR_RED "                              Please insert .rom files to your USB Stick\n"     ANSI_COLOR_RESET,path1,path2);
						sleep(3);
						return ret;
					}
					memset(file,'\0',sizeof(file));
					printf(ANSI_COLOR_YELLOW "                              Enter file name : "     ANSI_COLOR_RESET);
					scanf("%s",file);
					printf("\n");
					
					path_combine1 = (char *) malloc(1 + strlen(path1)+ strlen(file) ); 
					path_combine2 = (char *) malloc(1 + strlen(path2)+ strlen(file) );
					
						strcpy(path_combine1,path1);
						strcat(path_combine1,file);
						strcpy(path_combine2,path2);
						strcat(path_combine2,file);
						
						ret = pp_sw_update_process(path_combine1,softwareupdatereq_name);
						if( ret != 0)
						{
							ret = pp_sw_update_process(path_combine2,softwareupdatereq_name);
							if( ret != 0)
							{
								sleep(2);
								return ret;
								
							}
							else
							{
								exit(0);
								
							}
						}
						else
						{
							exit(0);
							
						}
				
				}
				else
				{
					return 0;
				}
}