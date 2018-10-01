/*!
 * IPC test application
 * Author : Muhammad Al Azhari 
*/
#include <variant_changer.h>

unsigned char gSP_INPUT[5];
unsigned char gILLU;
unsigned char gVariant;
unsigned char gModel;
unsigned char RTC_CLOCK[7];
unsigned char version[5];
unsigned char one_time[6];
unsigned char NorData[256];
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
static int thread_killer = 1;
FuncPtr gpTestFP=NULL;
int subscribeCB (FuncPtr pFunc);

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

int subscribeCB(FuncPtr pFunc)
{
    if (pFunc != NULL)
    {
        gpTestFP = pFunc;
    }
}

int callback(unsigned char* buf)
{
	gILLU = buf[0];
	printf("ILLU SUBSCRIBE = %d \n",gILLU);
}


void *speed_input_monitor( void *arg)
{
	FUNCTION_IN();
	char *resSP_INPUT = "S_SPEED_INPUT";
	int slen=1;
	
	while(thread_killer){
		

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
		
		Monitor_Clock_DbusConnection_Type_Byte(&RTC_CLOCK[0],resCLOCK);
		msleep(100);
		
	}
	#ifdef CURSOR_CTRL
	system("printf '\e[?25h'");
	#endif
	system("clear");
	FUNCTION_OUT();
}


void *illu_monitor( void *arg)
{
	FUNCTION_IN();
	unsigned char buff[5];
	buff[0]=1;
	
	
	char *setILLU = "R_GET_ILLUMINATION";
	char *resILLU = "S_GET_ILLUMINATION";
	unsigned char illu[5];
	
	while(thread_killer){

	MonitorDbusConnection_Type_Array(buff,1,setILLU,illu,1,resILLU);
	one_time[5] = illu[0];
	
	}
	
	
	FUNCTION_OUT();
}

void *ParkingBreak_monitor( void *arg)
{
	FUNCTION_IN();
	unsigned char buff[5];
	buff[0]=1;
	
	
	char *getParkingBreak = "R_GET_PARKING_BREAK";
	char *resParkingBreak = "S_GET_PARKING_BREAK";
	unsigned char park_break[5];
	
	while(thread_killer){

	MonitorDbusConnection_Type_Array(buff,1,getParkingBreak,park_break,1,resParkingBreak);
	one_time[2] = park_break[0];
	
	}
	
	
	FUNCTION_OUT();
}

void *Aux_monitor( void *arg)
{
	FUNCTION_IN();
	unsigned char buff[5];
	buff[0]=1;
	
	
	char *getAux1Sts = "R_GET_AUX1_STATUS";
	char *resAux1Sts = "S_GET_AUX1_STATUS";
	unsigned char aux1[5];
	
	char *getAux2Sts = "S_GET_AUX2_STATUS";
	char *resAux2Sts = "S_GET_AUX2_STATUS";
	unsigned char aux2[5];
	
	while(thread_killer){

	MonitorDbusConnection_Type_Array(buff,1,getAux1Sts,aux1,1,resAux1Sts);
	one_time[3] = aux1[0];
	
	MonitorDbusConnection_Type_Array(buff,1,getAux2Sts,aux2,1,resAux2Sts);
	one_time[4] = aux2[0]; 
	
	}
	
	
	FUNCTION_OUT();
}

void *Amp_monitor( void *arg)
{
	FUNCTION_IN();
	unsigned char buff[5];
	buff[0]=1;
	
	
	char *getAmpStanby = "R_GET_AMP_STANDBY";
	char *resAmpStanby = "S_GET_AMP_STANDBY";
	unsigned char amp_stnby[5];
	
	char *getAmpMute = "R_GET_AMP_MUTE";
	char *resAmpMute = "S_GET_AMP_MUTE";
	unsigned char amp_mute[5];
	
	
	
	while(thread_killer){

	MonitorDbusConnection_Type_Array(buff,1,getAmpStanby,amp_stnby,1,resAmpStanby);
	one_time[0] = amp_stnby[0];
	
	MonitorDbusConnection_Type_Array(buff,1,getAmpMute,amp_mute,1,resAmpMute);
	one_time[1] = amp_mute[0];
	
	}
	
	
	FUNCTION_OUT();
}
void pp_version(unsigned char version[])
{
	FUNCTION_IN();
	char *setSoftwareVersion_name = "R_SOFTVERS";
	char *resSoftwareVersion_name = "S_SOFTVERS";
	int slen=5;
	unsigned char sw_ver[5];
	unsigned char Sdbus_data[5];
	
	Sdbus_data[0]=1;
	
	MonitorDbusConnection_Type_Array(Sdbus_data,1,setSoftwareVersion_name,version,slen,resSoftwareVersion_name);
	
	FUNCTION_OUT();
}

void pp_variant(void)
{
	FUNCTION_IN();
	char *getVariant = "R_GET_VEHICLE_VARIANT";
	char *resVariant = "S_GET_VEHICLE_VARIANT";
	unsigned char variant[5];
	unsigned char buff[5];
	buff[0]=1;
	
	MonitorDbusConnection_Type_Array(buff,1,getVariant,variant,1,resVariant);
	gVariant=variant[0];
	
	
	FUNCTION_OUT();
}


void pp_nordata(void)
{
	FUNCTION_IN();
	char *getNorData = "R_DM_GET_NORDATA";
	char *resNorData = "S_DM_GET_NORDATA";
	int i;
	unsigned char variant[5];
	unsigned char buff[5];
	buff[0]=1;
	
	MonitorDbusConnection_Type_Array(buff,1,getNorData,NorData,256,resNorData);
	
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
	
	MonitorDbusConnection_Type_Array(buff,1,getModel,model,1,resModel);
	
	gModel=model[0];
	
	
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
	int i,max_nor_data;
	
	max_nor_data = sizeof(NorData);
	
	while(thread_killer){
		#ifdef CURSOR_CTRL
		system("printf '\e[?25l'");
		#else
		system("clear");
		#endif
		VAR_INFO_G("WELCOME TO PP INFORMATION INTERFACE\n\n");
		VAR_INFO_M("RTC Clock              :  "ANSI_COLOR_CYAN"%02d:%02d:%02d %02d/%02d/%d%d\b\n",RTC_CLOCK[0],RTC_CLOCK[1],RTC_CLOCK[2],RTC_CLOCK[3],RTC_CLOCK[4],RTC_CLOCK[5],RTC_CLOCK[6]);
		VAR_INFO_M("S/W Version            :  "ANSI_COLOR_CYAN"%d.%d\n",version[2],version[3]);
		VAR_INFO_M("H/W Version            :  "ANSI_COLOR_CYAN"%d.%d\n",version[0],version[1]);
		VAR_INFO_M("Variant id (IPC | PIN) :  "ANSI_COLOR_CYAN"%d | %d\n",gVariant,variant_id);
		VAR_INFO_M("Main Variant Model     :  "ANSI_COLOR_CYAN"%d\n",gModel);
		VAR_INFO_M("Illumination Status    :  "ANSI_COLOR_CYAN"%d\n",one_time[5]);
		VAR_INFO_M("Amplifier Stdby status :  "ANSI_COLOR_CYAN"%d\n",one_time[0]);
		VAR_INFO_M("Amplifier Mute status  :  "ANSI_COLOR_CYAN"%d\n",one_time[1]);
		VAR_INFO_M("Parking Break status   :  "ANSI_COLOR_CYAN"%d\n",one_time[2]);
		VAR_INFO_M("Aux1 Detect status     :  "ANSI_COLOR_CYAN"%d\n",one_time[3]);
		VAR_INFO_M("Aux1 Detect status     :  "ANSI_COLOR_CYAN"%d\n",one_time[4]);
		VAR_INFO_M("USB1 Playtime          :  "ANSI_COLOR_CYAN"%d\n");
		VAR_INFO_M("USB2 Playtime          :  "ANSI_COLOR_CYAN"\n");
		VAR_INFO_G("======= <CTRL+C> to cancel =======\n\n");
		msleep(1);
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
	pthread_t tid,tid2,tid3,tid4,tid5,tid6,tid7;
	int error;
	
	pp_version(&version[0]);
	pp_variant();
	pp_model();
	
	subscribeCB(&callback);
	
	signal(SIGINT, INThandler);
	
	error = pthread_create(&tid, NULL, &pp_monitor, NULL);
	if (error != 0)
		printf("\nThread can't be created :[%s]", strerror(error));
	
	error = pthread_create(&tid2, NULL, &rtc_clock, NULL);
	if (error != 0)
		printf("\nThread can't be created :[%s]", strerror(error));
	
	error = pthread_create(&tid3, NULL, &illu_monitor, NULL);
	 if (error != 0)
		 printf("\nThread can't be created :[%s]", strerror(error));
	 
	 error = pthread_create(&tid4, NULL, &Amp_monitor, NULL);
	 if (error != 0)
		 printf("\nThread can't be created :[%s]", strerror(error));
	
	error = pthread_create(&tid5, NULL, &ParkingBreak_monitor, NULL);
	 if (error != 0)
		 printf("\nThread can't be created :[%s]", strerror(error));
	
	error = pthread_create(&tid6, NULL, &Aux_monitor, NULL);
	 if (error != 0)
		 printf("\nThread can't be created :[%s]", strerror(error));
	
	
	pthread_join(tid, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);
    pthread_join(tid6, NULL);
	pthread_mutex_destroy(&lock);
	
	exit(0);
	#ifdef CURSOR_CTRL
	system("printf '\e[?25h'");
	#endif
	system("clear");
	
FUNCTION_OUT();
}