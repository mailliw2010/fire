#ifndef GLOBALFUN_H
#define GLOBALFUN_H

#define MAXDEVNUM   21
#define SERVER_ID   0X08

#define MAXUARTSIZE 256


#define REMOTE_CLIENT_ID     "111111"    // remote server client ID



/**if _ARM_LINUX is defined, program is running in board! fonts and uart will be changed**/
//#define _ARM_LINUX_

/**if _DEBUG_MODE_ is defined,
 * window menu will be exposed
 * action button can be use to debug.
**/
#define _DEBUG_MODE_

typedef struct{
    unsigned char sndUartData[MAXUARTSIZE];
    int sndUartLen;
    unsigned char recvUartData[256];
    int recvUartLen;


}tUartMesg;


extern tUartMesg UartMesg;
//extern unsigned char sndUartData[MAXUARTSIZE];
//extern int sndUartLen;

unsigned short CRC16_Modbus(unsigned char* pdata, int len);
unsigned short checksum_16(unsigned short *addr,int len);
char ConvertHex2Char(char ch);










#endif // GLOBALFUN_H
