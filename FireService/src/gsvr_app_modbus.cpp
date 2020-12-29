#include <stdio.h>
#include "gsvr_app_modbus.h"
#include "globalFun.h"
#include "modbus/modbus-private.h"
#include "modbus/modbus-rtu-private.h"
#include "gsvr_app_log.h"
gmodbus::gmodbus(std::string DeviceName_, int BaurdRate_, int DataBit_, int StopBit_):
    DeviceName(DeviceName_),BaurdRate(BaurdRate_),DataBit(DataBit_),StopBit(StopBit_)
{

    ctx = modbus_new_rtu(DeviceName.data(), BaurdRate, 'N', DataBit, StopBit);

}
gmodbus::~gmodbus()
{

    LOG(INFO) << "deleting ctx... ";
    modbus_free(ctx);


}

int gmodbus::setDevkeyVal(std::map<uint8_t, infoModel*> *devctx)
{
    m_devkeyval = devctx;

}

int gmodbus::initialize()
{

    /***Initial modbus-rtu**/

    LOG(INFO) << "Initing Modbus Serial...";
    if (ctx == NULL) {
           fprintf(stderr, "Unable to allocate libmodbus context\n");
           return -1;
       }


    modbus_set_debug(ctx, TRUE);    // set debug message
    modbus_set_slave(ctx, SERVER_ID);  // set default slave device id

    if (modbus_connect(ctx) == -1) {
       fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));


       LOG(INFO) << " Open Failed!";

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


int gmodbus::ModbusWriteMultiRegister(int devid, int addr, int n, uint16_t* data)
{
    modbus_set_slave(ctx, devid);   // set devid
    int rc = modbus_write_registers(ctx, addr, n, data);   // there is already have response jude,if rc==-1, no response or recv format error
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
     LOG(INFO) << " Error, to read!";
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
        LOG(INFO) << "Not found key in Map:" << devid;
        return;
    }

    if(cmd == 0xff)
    {
//        if(it->second->currentData->openState == 1)      // control them always!
        {
            /* if rc == 0:no response;
               if rc < 0: err problems;
                   rc == -1: response mesg err/crc err(device response 0x0080 seems err);
                   rc == -2, lengh error;
            */
            if(ModbusWriteOneRegister(devid, 0x0080, 0x0155)  == 0)
            {
                LOG(INFO) << "No Response! maybe devid doesnot exist!\n";

                return;
            }
            ModbusWriteOneRegister(devid, 0x0081, 0x01AA);
            LOG(INFO) << "Open device " << devid << "successful!";
            it->second->InfoData.IsOpenState = 0;


        }

    }
    if(cmd == 0x00)
    {
//        if(it->second->currentData->openState == 0)      // control them always!
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
            LOG(INFO) << "Close device " << devid << "successful!";
            it->second->InfoData.IsOpenState = 1;

        }


    }


}



int gmodbus::ModbusGetStatus(const int devid)
{

    std::map<uint8_t, infoModel*>::iterator it = m_devkeyval->find((uint8_t)devid);
    if(it == m_devkeyval->end())   // if no keys in contain, return;
    {
        LOG(INFO) << "Not found devid in Map:" << devid;
        return -1;
    }

    LOG(INFO) << "Getting Info from:"<< devid;

    // number =  0x32(50)    costs about 160ms;
    if(ModbusReadMultiRegister(devid, 0x00, 0x32, (uint16_t *)it->second->currentData) < 0)   // if rc == -1, no read data
    {
        LOG(INFO) << "No Response! Read Error!\n";
        return -1;
        /**unopen function***/


    }



    /***No need to report this data*****/
    // number = 0x35(53)    costs about 160ms;
//    if(ModbusReadMultiRegister(devid, 0x40, 0x35, (uint16_t *)it->second->currentData + 0x32) < 0)   // if rc == -1, no read data
//    {
//        qDebug() << "No Response! Read Error!\n";
//        return -1;
//        /**unopen function***/


//    }

//    // number = 0x4(4)  costs about 70ms;
//    if(ModbusReadMultiRegister(devid, 0x100, 4, (uint16_t *)it->second->currentData + 0x32 + 0x35) < 0)   // if rc == -1, no read data
//    {
//        qDebug() << "No Response! Read Error!\n";
//        return -1;
//        /**unopen function***/


//    }


    return 1;

}





