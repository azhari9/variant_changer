/*!
 * PP Software update through IPC
 * Author : Muhammad Al Azhari 
*/
#include <variant_changer.h>

char  path1[] = "/run/media/sda1/";
char  path2[] = "/run/media/sdb1/";

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

int pp_sw_update_process(char p_string[], char software_update_req_name[])
{
	
	if( access( p_string, F_OK ) != -1 ) 
	{
		system("clear && printf '\e[3J'");
		VAR_INFO_G("          %s   exists \n\n",p_string);
		VAR_INFO_R("  START PP SW UPDATE, DO NOT TURN OFF YOUR UNIT!!!!!!! \n\n" );
		VAR_INFO_R("        IF YOU WANT to Cancel BU ON/OFF before 50%     \n\n\n" );
		CreateDbusConnection_Type_String(p_string,software_update_req_name);
		system("delete-snapshot");
		VAR_INFO_R("             PLEASE WAIT, UPGRADING IN PROGRESS!!!!!!! \n" );
		VAR_INFO_R("   PLEASE WAIT, UPGRADING IN PROGRESS!!!!!!!           \n" );
		VAR_INFO_R("             PLEASE WAIT, UPGRADING IN PROGRESS!!!!!!! \n" );
		VAR_INFO_R("  PLEASE WAIT, UPGRADING IN PROGRESS!!!!!!!            \n" );
		VAR_INFO_R("             PLEASE WAIT, UPGRADING IN PROGRESS!!!!!!!  \n" );
		VAR_INFO_R("DO NOT TURN OFF!! SYSTEM WILL AUTOMATICALLY REBOOT!!!!!!! \n" );
		printf(ANSI_COLOR_CYAN);
		system("dlt-receive -a localhost | grep \"Software Update Progress\" &");
		return 0;
	}
	else 
	{
		VAR_INFO_R("\n%s file doesn't exist!! \n",p_string) ;
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
		if (n < 0) 
		{
			n = scandir(path2, &namelist, parse_ext, alphasort);
			if (n < 0) {
				return -1;
			}
			else if ( n == 0 )
			{
				return 1;
			}
			else 
			{
				system("clear && printf '\e[3J'");
				VAR_INFO_Y("=========================================\n");
				VAR_INFO_C(" WELCOME TO PP SOFTWARE UPDATE INTERFACE\n");
				VAR_INFO_C("\n  CURRENT PP S/W VERSION:: "ANSI_COLOR_MAGENTA"%s : %d.%d\n\n",oem_info,Gdbus_data[2],Gdbus_data[3]);
				VAR_INFO_R(" USE AT YOUR OWN RISK!!!\n");
				VAR_INFO_Y("==========================================\n");
				VAR_INFO_C("_______Available ROM Files at %s_________\n\n",path2);
				while (n--) {
					VAR_INFO("        %d) %s\n",i, namelist[n]->d_name);
					free(namelist[n]);
					i++;
				}
				VAR_INFO_C("_______________________________________________________\n");
				free(namelist);
				return 0;
			}
		}
		else if ( n == 0 )
		{
			return 1;
		}
		else {
			system("clear && printf '\e[3J'");
			VAR_INFO_Y("=========================================\n");
			VAR_INFO_C(" WELCOME TO PP SOFTWARE UPDATE INTERFACE\n");
			VAR_INFO_C("\n  CURRENT PP S/W VERSION:: "ANSI_COLOR_MAGENTA"%s : %d.%d\n\n",oem_info,Gdbus_data[2],Gdbus_data[3]);
			VAR_INFO_R(" USE AT YOUR OWN RISK!!!\n");
			VAR_INFO_Y("==========================================\n");
			VAR_INFO_C("_______Available ROM Files at %s_________\n\n",path1 );
			while (n--) {
				VAR_INFO("        %d) %s\n",i, namelist[n]->d_name);
				free(namelist[n]);
				i++;
			}
			VAR_INFO_C("_______________________________________________________\n");
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
	char pasword[10],ch;
	int i;
	
	lcase(oem_info);
	
	system("clear && printf '\e[3J'");
	VAR_INFO_Y("=========================================\n");
	VAR_INFO_C(" WELCOME TO PP SOFTWARE UPDATE INTERFACE\n");
	VAR_INFO_C("\n  CURRENT PP S/W VERSION:: "ANSI_COLOR_MAGENTA"%s : %d.%d\n\n",oem_info,Gdbus_data[2],Gdbus_data[3]);
	VAR_INFO_R(" USE AT YOUR OWN RISK!!!\n");
	VAR_INFO_Y("==========================================\n\n");
	
	VAR_INFO_Y("Enter password: ");
	
	for(i=0;i<4;i++)
	{
		ch = getch();
		pasword[i] = ch;
		ch = '*' ;
		printf("%c",ch);
	}
	printf("\n");
	pasword[i] = '\0';
	
	if(pasword[0] != '0' || pasword[1] != '9' || pasword[2] != '8' || pasword[3] != '7')
	{
		VAR_INFO_R("\nYou don't have access to proceed!!\n");
		return -1;
	}
	else
	{
		system("clear && printf '\e[3J'");
	}

				VAR_INFO_Y("=========================================\n");
				VAR_INFO_C(" WELCOME TO PP SOFTWARE UPDATE INTERFACE\n");
				VAR_INFO_C("\n  CURRENT PP S/W VERSION: "ANSI_COLOR_MAGENTA"%s : %d.%d\n\n",oem_info,Gdbus_data[2],Gdbus_data[3]);
				VAR_INFO_R(" USE AT YOUR OWN RISK!!!\n");
				VAR_INFO_Y("==========================================\n");
				VAR_INFO_G("Please put your rom file at this directory /run/media/{sda1 or sdb1}/player.rom  \n\n");
				VAR_INFO_G("SW Update file and path option  \n");
				VAR_INFO_G("1) Proceed with default file name (player.rom)\n");
				VAR_INFO_G("2) Change file name (eg: PP4017X.rom)  \n\n");
				VAR_INFO_R("Make sure you insert correct file, otherwise system will never bootup!!  \n");
				VAR_INFO_Y("Enter Option : ");
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
						VAR_INFO_R("%s and %s not available\n"     ANSI_COLOR_RESET,path1,path2);
						sleep(3);
						return ret;
					}
					else if ( ret == 1)
					{
						VAR_INFO_R(".rom files not available at %s ond %s\n"     ANSI_COLOR_RESET,path1,path2);
						VAR_INFO_R("Please insert .rom files to your USB Stick\n"     ANSI_COLOR_RESET,path1,path2);
						sleep(3);
						return ret;
					}
					memset(file,'\0',sizeof(file));
					VAR_INFO_Y("Enter file name : "     ANSI_COLOR_RESET);
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