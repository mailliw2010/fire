#include "globalFun.h"
#include <QDateTime>
#include<iostream>
#include<QMutex>


//unsigned char sndUartData[MAXUARTSIZE];
//int sndUartLen = 0;
tUartMesg UartMesg;    // define a buffer to save UartMesg




unsigned short checksum_16(unsigned short *addr,int len)
{
    unsigned short chksum;
    unsigned int sum = 0;

    while(len > 1)
    {
        sum += *addr++;
        len -= 2;
    }

    if(len == 1)
        sum += *(unsigned char*)addr;

    sum = (sum>>16) + (sum & 0xffff);
    sum += (sum>>16);
    chksum = ~sum;
    return (chksum);
}








unsigned short CRC16_Modbus(unsigned char* pdata, int len)
{
    unsigned short crc=0xFFFF;
    int i, j;
    for ( j=0; j<len;j++)
    {
        crc=crc^pdata[j];
        for(i=0; i<8; i++)
        {
           if( ( crc&0x0001) >0)
            {
                crc=crc>>1;
                crc=crc^ 0xa001;
            }
           else
               crc=crc>>1;
        }
    }
    return crc;
}


char ConvertHex2Char(char ch)
{
    if((ch >='0') && (ch <= '9'))
        return ch - 0x30;
    else if((ch >= 'A') && (ch <= 'Z'))
        return ch -'A' + 10;
    else if((ch >= 'a') && (ch <= 'z'))
        return ch -'a' + 10;
    else
        return ch - ch;


}




