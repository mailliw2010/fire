#include "globalFun.h"
#include<iostream>
#include<QMutex>


tUartMesg UartMesg;    // define a buffer to save UartMesg
int ResponseTimeOut;
QMutex uart_mutex;  // global



unsigned short checksum_16(uint8_t *addr,int len)
{
    uint32_t chksum = 0;

    while(len > 0)
    {
        chksum += *(addr++);
        len--;
    }

    return chksum;
}


unsigned char checksum_8(uint8_t *addr,int len)
{
    uint32_t chksum = 0;

    while(len > 0)
    {
        chksum += *(addr++);
        len--;
    }

    return chksum;
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


void printbuf(unsigned char *Buffer, int Len)
{
    int i;
//    printf("len = %d(0x%x):\n",Len,Len);
    for(i = 0; i < Len; i++)
        printf("%02x ",Buffer[i]);
//    printf("\n");
    return;

}

void reverse_buffer(unsigned char *data, int len)
{
    uint8_t tmp;
    for(int i = 0; i < len/2; i++)
    {
        tmp = data[i];
        data[i] = data[len - 1 - i];
        data[len - 1 - i] = tmp;
    }

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


