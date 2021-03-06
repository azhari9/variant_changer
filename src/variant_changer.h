#ifndef VARIANT_CHANGER_H_
#define VARIANT_CHANGER_H_

#include <stdio.h>  
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <sys/ioctl.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <dbus/dbus-glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <dlt.h>
#include <glib.h>
#include <string.h>
#include <memory.h>
#include <dirent.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include <assert.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//#define VAR_INFO_R(fmt, args...) printf(ANSI_COLOR_RED    "  " fmt "\n" ANSI_COLOR_RESET,##args)
//#define VAR_INFO_G(fmt, args...) printf(ANSI_COLOR_GREEN  "  " fmt "\n" ANSI_COLOR_RESET,##args)
//#define VAR_INFO_Y(fmt, args...) printf(ANSI_COLOR_YELLOW "  " fmt "\n" ANSI_COLOR_RESET,##args)

#define VAR_INFO(fmt, args...) printf("  " fmt ,##args)
#define VAR_INFO_G(fmt, args...) printf(ANSI_COLOR_GREEN  "  " fmt ANSI_COLOR_RESET,##args)
#define VAR_INFO_C(fmt, args...) printf(ANSI_COLOR_CYAN  "  " fmt ANSI_COLOR_RESET,##args)
#define VAR_INFO_M(fmt, args...) printf(ANSI_COLOR_MAGENTA  "  " fmt ANSI_COLOR_RESET,##args)
#define VAR_INFO_Y(fmt, args...) printf(ANSI_COLOR_YELLOW  "  " fmt ANSI_COLOR_RESET,##args)
#define VAR_INFO_R(fmt, args...) printf(ANSI_COLOR_RED  "  " fmt ANSI_COLOR_RESET,##args)
//#define DEBUG 
#ifndef DEBUG
#define CURSOR_CTRL 
#endif
#ifdef DEBUG 
#define FUNCTION_IN(fmt, args...) printf("FUNCTION_IN " "%s\n" fmt,__func__,##args)
#define FUNCTION_OUT(fmt, args...) printf("FUNCTION_OUT " "%s\n" fmt,__func__,##args)
#else
#define FUNCTION_IN(fmt, args...) printf(""fmt,##args)
#define FUNCTION_OUT(fmt, args...) printf(""fmt,##args)
#endif

extern void CreateDbusConnection_Type_String(char *string,char *msg_name);
extern void CreateDbusConnection_Type_Array(unsigned char arr[], int msg_length, char *msg_name);
extern unsigned char MonitorDbusConnection_Type_Array(unsigned char arr[], int msg_length, char *msg_name_s,unsigned char Buffer[],int bufferSize, char *msg_name);
extern unsigned char Monitor_Clock_DbusConnection_Type_Byte(unsigned char clock[],char *msg_name);
extern unsigned char Monitor_DbusConnection(unsigned char pBuffer[],int bufferSize,char *msg_name);
extern int pp_sw_update(void);
extern unsigned char Gdbus_data[5];
extern void init_ipc_test_app(void);
extern int variant_id;
extern char oem_info[30];
extern void pp_version(unsigned char version[]);
extern void msleep(int time);
extern void lcase(char s[]);
extern pthread_mutex_t lock;

typedef int (*FuncPtr)(unsigned char *);

#endif
