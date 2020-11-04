#ifndef GLOBALFUN_H
#define GLOBALFUN_H
#include<stdint.h>


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



#define HIGH_LOW_SWAP16(A)	((unsigned short)(A) & 0xff00) >> 8 | ((unsigned short)(A) & 0xff) << 8
#define HIGH_LOW_SWAP32(A)	((unsigned int)(A) & 0xff000000) >> 24 | ((unsigned int)(A) & 0x00ff0000) >> 8 | ((unsigned int)(A) & 0x0000ff00) << 8 | ((unsigned int)(A) & 0x000000ff) << 24
// 0xff00ff00 00ff00ff
#define HIGH_LOW_SWAP64(A)  ((unsigned long)(A) & 0xff00000000000000) >> 56 | \
                            ((unsigned long)(A) & 0x00ff000000000000) >> 40 | \
                            ((unsigned long)(A) & 0x0000ff0000000000) >> 24 | \
                            ((unsigned long)(A) & 0x000000ff00000000) >> 8 | \
                            ((unsigned long)(A) & 0x00000000ff000000) << 8 | \
                            ((unsigned long)(A) & 0x0000000000ff0000) << 24 | \
                            ((unsigned long)(A) & 0x000000000000ff00) << 40 | \
                            ((unsigned long)(A) & 0x00000000000000ff) << 56




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



typedef enum{
    ReadDataSerial,  // read more than 4byte data, or ascii
    ReadDataOne,    // read one data
    ReadDataMulti,  // read abc_pthase sth like that
    ReadAddr,
    WriteData,
    WriteAddr,
    DevCtrl,

}tRegType;

typedef enum{
    MOpen,   //means he zha
    MClose

}tCtrlType;



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

#ifdef __cplusplus
extern "C"{
#endif


unsigned short CRC16_Modbus(unsigned char* pdata, int len);
unsigned short checksum_16(unsigned char *addr,int len);
unsigned char checksum_8(uint8_t *addr,int len);
void printbuf(unsigned char *Buffer, int Len);
char ConvertHex2Char(char ch);

void reverse_buffer(uint8_t *data, int len);

#ifdef __cplusplus
}
#endif


#endif // GLOBALFUN_H
