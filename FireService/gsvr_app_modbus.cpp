#include "gsvr_app_modbus.h"
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>
#include "globalFun.h"
#include "modbus/modbus-private.h"
#include "modbus/modbus-rtu-private.h"
gmodbus::gmodbus(QString DeviceName_, int BaurdRate_, int DataBit_, int StopBit_):
    DeviceName(DeviceName_),BaurdRate(BaurdRate_),DataBit(DataBit_),StopBit(StopBit_)
{

    ctx = modbus_new_rtu(DeviceName.toLatin1().data(), BaurdRate, 'N', DataBit, StopBit);

}
gmodbus::~gmodbus()
{

    qDebug() << "deleting ctx... ";
    modbus_free(ctx);


}

int gmodbus::setDevkeyVal(QMap<uint8_t, infoModel*> *devctx)
{
    m_devkeyval = devctx;

}

int gmodbus::initialize()
{

    /***Initial modbus-rtu**/

    qDebug() << "Initing Modbus Serial...";
    if (ctx == NULL) {
           fprintf(stderr, "Unable to allocate libmodbus context\n");
           return -1;
       }


    modbus_set_debug(ctx, FALSE);    // set debug message
    modbus_set_slave(ctx, SERVER_ID);  // set default slave device id

    if (modbus_connect(ctx) == -1) {
       fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));

       QMessageBox msgBox;
       QString errmesg = QString(QLatin1String(ctx->backend_data->device));
       errmesg.append(" Open Failed!");
       qCritical() << errmesg;
       msgBox.information(nullptr, "Err!",errmesg, "Close");
       return -1;
    }


}


int gmodbus::ModbusWriteOneRegister(int devid, int addr, uint16_t cmd)
{
    modbus_set_slave(ctx, devid);   // set devid
    int rc = modbus_write_register(ctx, addr, cmd);   // there is already have response jude,if rc==-1, no response or recv format error
//    ASSERT_TRUE(rc == 1, "");  // judge whether send success

    /**no need to wait for response,because the functiong modbus_write_register() is already realized****/

    return rc;



}


int gmodbus::ModbusReadMultiRegister(int devid, int addr, int num, uint16_t *reg)
{

      // no more than 125
    modbus_set_slave(ctx, devid);

#ifdef _DEBUG_TIME_ELAPSE
    /***Test More Time***/
    timeval start, end;
    gettimeofday(&start, nullptr);
#endif
    int rc = modbus_read_registers(ctx, addr, num, reg);     //read more registers!
    if(rc == -1)
    {
     qWarning() << " Error, to read!";
     return -1;
    }

#ifdef _DEBUG_TIME_ELAPSE
    gettimeofday(&end, nullptr);
    printf("modbus_read_registers elapsed:%ld secs, %ld usecs\n;", end.tv_sec - start.tv_sec, \
    end.tv_usec - start.tv_usec);
#endif

    return 0;


}


void gmodbus::ModbusCtrlSwitch(const int devid, const int cmd)    // set 0x06
{

    auto it = m_devkeyval->find((uint8_t)devid);
    if(it == m_devkeyval->end())   // if no keys in contain, return;
    {
        qWarning() << "Not found key in Map:" << devid;
        return;
    }

    if(cmd == 0xff)
    {
        if(it.value()->currentData->openState == 1)
        {
            /* if rc == 0:no response;
               if rc < 0: err problems;
                   rc == -1: response mesg err/crc err(device response 0x0080 seems err);
                   rc == -2, lengh error;
            */
            if(ModbusWriteOneRegister(devid, 0x0080, 0x0155)  == 0)
            {
                qDebug() << "No Response! maybe devid doesnot exist!\n";

                return;
            }
            ModbusWriteOneRegister(devid, 0x0081, 0x01AA);
            qDebug() << "Open device " << devid << "successful!";
            it.value()->InfoData.IsOpenState = 0;


        }

    }
    if(cmd == 0x00)
    {
        if(it.value()->currentData->openState == 0)
        {
            /* if rc == 0:no response;
               if rc < 0: err problems;
                   rc == -1: response mesg err/crc err(device response 0x0080 seems err);
                   rc == -2, lengh error;
            */
            if(ModbusWriteOneRegister(devid, 0x0080, 0x0255) == 0)
            {
                printf("No Response! maybe devid doesnot exist!\n");

                /**unclose function***/
                return;
            }
            ModbusWriteOneRegister(devid, 0x0081, 0x02AA);
            qDebug() << "Close device " << devid << "successful!";
            it.value()->InfoData.IsOpenState = 1;

        }


    }


}



int gmodbus::ModbusGetStatus(const int devid)
{

    QMap<uint8_t, infoModel*>::iterator it = m_devkeyval->find((uint8_t)devid);
    if(it == m_devkeyval->end())   // if no keys in contain, return;
    {
        qWarning() << "Not found devid in Map:" << devid;
        return -1;
    }

    qDebug() << "Getting Info from:"<< devid;

    // number =  0x32(50)    costs about 160ms;
    if(ModbusReadMultiRegister(devid, 0x00, 0x32, (uint16_t *)it.value()->currentData) < 0)   // if rc == -1, no read data
    {
        qDebug() << "No Response! Read Error!\n";
        return -1;
        /**unopen function***/


    }



    /***No need to report this data*****/
    // number = 0x35(53)    costs about 160ms;
//    if(ModbusReadMultiRegister(devid, 0x40, 0x35, (uint16_t *)it.value()->currentData + 0x32) < 0)   // if rc == -1, no read data
//    {
//        qDebug() << "No Response! Read Error!\n";
//        return -1;
//        /**unopen function***/


//    }

//    // number = 0x4(4)  costs about 70ms;
//    if(ModbusReadMultiRegister(devid, 0x100, 4, (uint16_t *)it.value()->currentData + 0x32 + 0x35) < 0)   // if rc == -1, no read data
//    {
//        qDebug() << "No Response! Read Error!\n";
//        return -1;
//        /**unopen function***/


//    }


    return 1;

}





