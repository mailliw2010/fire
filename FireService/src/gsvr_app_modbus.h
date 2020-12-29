#ifndef GSVR_APP_MODBUS_H
#define GSVR_APP_MODBUS_H
#include <string>
#include "modbus/modbus.h"
#include "gsvr_info_break.h"
#include<map>
class gmodbus
{

public:

    explicit gmodbus(std::string DeviceName_, int BaurdRate_, int DataBit_, int StopBit_);
    ~gmodbus();
    int initialize();
    int ModbusWriteOneRegister(int devid, int addr, uint16_t cmd);
    int ModbusWriteMultiRegister(int devid, int addr, int n, uint16_t* data);
    int ModbusReadMultiRegister(int devid, int addr, int num, uint16_t *reg);
    void ModbusCtrlSwitch(const int devid, const int cmd);  //0xff, open; 0x00, close
    int ModbusGetStatus(const int devid);   // -1: read error; 0: right
    int setDevkeyVal(std::map<uint8_t, infoModel*> *devctx);

    modbus_t *ctx;

private:
    gmodbus(const gmodbus&);
    gmodbus& operator=(const gmodbus&);
    std::map<uint8_t, infoModel*> *m_devkeyval;
    std::string DeviceName;
    int BaurdRate;
    int DataBit;
    int StopBit;







};



#endif // GSVR_APP_MODBUS_H
