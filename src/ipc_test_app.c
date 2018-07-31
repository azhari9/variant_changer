#include <variant_changer.h>

unsigned char gSP_INPUT[5];
unsigned char gILLU;
unsigned char gVariant;
unsigned char gModel;
unsigned char RTC_CLOCK[7];
unsigned char version[5];
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
static int thread_killer = 1;

void msleep(int time)
{
	time=time*1000;
	usleep(time);
}

void  INThandler(int sig) 
{
    thread_killer = 0;
	#ifdef CURSOR_CTRL
	system("printf '\e[?25h'");
	#endif
	exit(0);
}


void *speed_input_monitor( void *arg)
{
	FUNCTION_IN();
	char *resSP_INPUT = "S_SPEED_INPUT";
	int slen=1;
	
	while(thread_killer){
		
		MonitorDbusConnection_Type_Array(&gSP_INPUT[0],slen,resSP_INPUT);
		msleep(2);
		
	}
	#ifdef CURSOR_CTRL
	system("printf '\e[?25h'");
	#endif
	system("clear");
	FUNCTION_OUT();
}

void *rtc_clock( void *arg)
{
	FUNCTION_IN();
	char *resCLOCK = "S_GET_CLOCK_SET";
	int i;
	
	while(thread_killer){
		
		MonitorDbusConnection_Type_Byte(&RTC_CLOCK[0],resCLOCK);
		msleep(100);
		
	}
	#ifdef CURSOR_CTRL
	system("printf '\e[?25h'");
	#endif
	system("clear");
	FUNCTION_OUT();
}

void pp_version(void)
{
	FUNCTION_IN();
	char *setSoftwareVersion_name = "R_SOFTVERS";
	char *resSoftwareVersion_name = "S_SOFTVERS";
	int slen=5,i;
	unsigned char sw_ver[5];
	unsigned char Sdbus_data[5];
	
	Sdbus_data[0]=1;
	
	CreateDbusConnection_Type_Array(Sdbus_data,1,setSoftwareVersion_name);
	MonitorDbusConnection_Type_Array(&version[0],slen,resSoftwareVersion_name);
	
	// for(i = 0 ; i < 5; i++)
	// {
		// version[i] = sw_ver[i];
	// }
	
	FUNCTION_OUT();
}

void pp_variant(void)
{
	FUNCTION_IN();
	char *getVariant = "R_GET_VEHICLE_VARIANT";
	char *resVariant = "S_GET_VEHICLE_VARIANT";
	int i;
	unsigned char variant[5];
	unsigned char buff[5];
	
	buff[0]=1;
	
	CreateDbusConnection_Type_Array(buff,1,getVariant);
	MonitorDbusConnection_Type_Array(&variant[0],1,resVariant);
	
	gVariant=variant[0];
	
	
	FUNCTION_OUT();
}

void pp_model(void)
{
	FUNCTION_IN();
	char *getModel = "R_DM_GET_MODEL_VARIANT";
	char *resModel = "S_DM_GET_MODEL_VARIANT";
	int i;
	unsigned char model[5];
	unsigned char buff[5];
	
	buff[0]=1;
	
	CreateDbusConnection_Type_Array(buff,1,getModel);
	MonitorDbusConnection_Type_Array(&model[0],1,resModel);
	
	gModel=model[0];
	
	
	FUNCTION_OUT();
}

void *illumination_sts( void *arg)
{
	FUNCTION_IN();
	unsigned char illu_sts[5];
	char *setILLU = "R_ILLUMINATION";
	char *resILLU = "S_ILLUMINATION";
	int slen=1;
	unsigned char buff[5];
	buff[0]=1;
	
	while(thread_killer){
		
		CreateDbusConnection_Type_Array(buff,1,setILLU);
		MonitorDbusConnection_Type_Array(&illu_sts[0],slen,resILLU);
		if(illu_sts[0] == 1 || illu_sts[0] == 0)
			gILLU=illu_sts[0];
		msleep(10);
		
	}
	#ifdef CURSOR_CTRL
	system("printf '\e[?25h'");
	#endif
	system("clear");
	FUNCTION_OUT();
}

void *pp_monitor(void *arg)
{
	FUNCTION_IN();
	#ifdef CURSOR_CTRL
	system("clear && printf '\e[3J'");
	#else
	system("clear");
	#endif
	
	while(thread_killer){
		#ifdef CURSOR_CTRL
		system("printf '\e[?25l'");
		#else
		system("clear");
		#endif
		VAR_INFO_G("WELCOME TO PP INFORMATION INTERFACE\n\n");
		VAR_INFO_M("RTC Clock           :  "ANSI_COLOR_CYAN"%02d:%02d:%02d %02d/%02d/%d%d\b\n",RTC_CLOCK[0],RTC_CLOCK[1],RTC_CLOCK[2],RTC_CLOCK[3],RTC_CLOCK[4],RTC_CLOCK[5],RTC_CLOCK[6]);
		VAR_INFO_M("S/W Version         :  "ANSI_COLOR_CYAN"%d.%d\n",version[2],version[3]);
		VAR_INFO_M("H/W Version         :  "ANSI_COLOR_CYAN"%d.%d\n",version[0],version[1]);
		VAR_INFO_M("Variant id          :  "ANSI_COLOR_CYAN"%d\n",gVariant);
		VAR_INFO_M("Main Variant Model  :  "ANSI_COLOR_CYAN"%d\n",gModel);
		VAR_INFO_M("Illumination Status :  "ANSI_COLOR_CYAN"%d\n",gILLU);
		VAR_INFO_M("Nor Data Info       :  "ANSI_COLOR_CYAN"\n");
		VAR_INFO_M("USB1 Playtime       :  "ANSI_COLOR_CYAN"\n");
		VAR_INFO_M("USB2 Playtime       :  "ANSI_COLOR_CYAN"\n");
		VAR_INFO_M("Aux1 Detect status  :  "ANSI_COLOR_CYAN"\n");
		VAR_INFO_M("Aux1 Detect status  :  "ANSI_COLOR_CYAN"\n");
		//VAR_INFO_M(": "ANSI_COLOR_CYAN"\n",);
		//VAR_INFO_M(": "ANSI_COLOR_CYAN"\n",);
		VAR_INFO_G("===================================\n\n");
		msleep(100);
		#ifdef CURSOR_CTRL
		system("printf '\033[;H'");
		#endif
	}
	#ifdef CURSOR_CTRL
	system("printf '\e[?25h'");
	#endif
	system("clear");
	FUNCTION_OUT();
}

void init_ipc_test_app(void)
{
FUNCTION_IN();
	
	
	int a,b,c,d,e,f,g,h,i,ret;
	a=1;b=2;c=3;d=4;e=5;f=6;g=7;h=8;
	pthread_t tid;
	
	
	
	pp_version();
	pp_variant();
	pp_model();
	pthread_create(&tid, NULL, pp_monitor, (void *)&a);
	pthread_create(&tid, NULL, illumination_sts, (void *)&b);
	pthread_create(&tid, NULL, rtc_clock, (void *)&c);
	
	signal(SIGINT, INThandler);
	while(thread_killer)
	{
		sleep(2);
	};
	exit(0);
	#ifdef CURSOR_CTRL
	system("printf '\e[?25h'");
	#endif
	system("clear");
	
FUNCTION_OUT();
}