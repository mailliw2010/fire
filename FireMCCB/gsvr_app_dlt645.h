#ifndef GSVR_APP_DLT645_H
#define GSVR_APP_DLT645_H

#include "dlt645/dlt645.h"
#include <QString>
#include "gsvr_info_mccb.h"
#include "globalFun.h"
class gdlt645
{

public:

    typedef enum{
        RunOk,
        Special,
        Rest,
        Sudden,
        LackZero,
        Over,
        Short,
        LackPhase,
        UnderVoltage,
        OverVoltage,
        UnderGround,
        NoPower,
        TimeTest,
        Remote,
        KeyTest,
        CloseLock,
        InterSenseFault,
        CloseFatal,
        HandMake,
        SetChange,
        RemoteCut,
        CurrentMove
    }tRunStatus;


    explicit gdlt645(QString DeviceName, int BaurdRate, int DataBit, int StopBit);
    ~gdlt645();
    int initialize();

    int read_registers(const uint64_t devaddr, uint32_t addr, void *dest, tRegType type);
    int read_abcphase_voltage(const uint64_t devaddr, uint32_t* dest);
    int read_abcphase_current(const uint64_t devaddr, uint32_t* dest);
    int read_run_status(const uint64_t devaddr, uint32_t* dest);
    int read_isopen_state(const uint64_t devaddr);
    int read_swtcnt_num(const uint64_t devaddr, uint32_t* dests);

    int read_one_registers(const uint64_t devaddr, uint32_t addr, uint32_t* dest);
    int read_serial_registers(const uint64_t devaddr, uint32_t addr, uint8_t* dest);
    int write_registers(const uint64_t devaddr, uint32_t addr, void *data, int len, tRegType type);
    int dev_ctrl(const uint64_t devaddr, tCtrlType cmd, int tv = 0);

    void setDevkeyVal(QMap<uint64_t, infoMCCB*> *devctx);
    dlt645_t *ctx;

private:
    QMap<uint64_t, infoMCCB*> *m_devkeyval;







};



#endif // GSVR_APP_DLT645_H
