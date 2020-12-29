#ifndef GLOBALFUN_H
#define GLOBALFUN_H


/************ [Infomation]****************
the file is used for share c and c++ file!
so we just define variable in format c!
******************************************/


/************ [Modbus Configure]****************
This is Modbus Configure.

***********************************************/
/**Device Total Number**/
#define MAXDEVNUM   21
/**Modbus Service ID**/
#define SERVER_ID   0X08
#define MAXUARTSIZE 256
extern int ResponseTimeOut;



/************ [Mqtt Configure]****************
This is Mqtt Configure. C++/QT about.

***********************************************/
// See Public File "mqttpublic.h"


/************ [Global  Variable]****************
This is Mqtt Configure. C++/QT about.

***********************************************/
typedef struct{
    unsigned char sndUartData[MAXUARTSIZE];
    int sndUartLen;
    unsigned char recvUartData[256];
    int recvUartLen;


}tUartMesg;

#ifdef __cplusplus
extern "C"{
#endif

extern tUartMesg UartMesg;   // in c.file, use extern tUartMesg UartMesg; in cpp.file, use "extern "C" { extern tUartMesg UartMesg }

#ifdef __cplusplus
}
#endif


//  if compiled by windows, set to support chinese.
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif





/**if _DEBUG_TIME is defined, print uart write time/read registers time**/
//#define _DEBUG_TIME_ELAPSE


/**if _ARM_LINUX is defined, program is running in board! fonts and uart will be changed**/
//#define _ARM_LINUX_

/**if _DEBUG_SHOWMENU_ is defined,
 * window menu will be exposed
 * action button can be use to debug.
**/
//#define _DEBUG_SHOWMENU_

void printbuf(unsigned char *Buffer, int Len);
unsigned short CRC16_Modbus(unsigned char* pdata, int len);
unsigned short checksum_16(unsigned char *addr,int len);
char ConvertHex2Char(char ch);



#endif // GLOBALFUN_H
