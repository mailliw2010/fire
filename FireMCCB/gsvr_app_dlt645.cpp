#include "gsvr_app_dlt645.h"
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>
#include "globalFun.h"
#include "dlt645/dlt645-private.h"
#include "dlt645/dlt645-rtu-private.h"
gdlt645::gdlt645(QString DeviceName, int BaurdRate, int DataBit, int StopBit)
{

    ctx = dlt645_new_rtu(DeviceName.toLatin1().data(), BaurdRate, 'N', DataBit, StopBit);

}
gdlt645::~gdlt645()
{

    qDebug() << "deleting ctx... ";
    dlt645_free(ctx);


}

void gdlt645::setDevkeyVal(QMap<uint64_t, infoMCCB*> *devctx)
{
    m_devkeyval = devctx;

}

int gdlt645::initialize()
{

    /***Initial gdlt645-rtu**/
    uint8_t mccb_addr = 1;
    qDebug() << "Initing DLT645 Serial...";
    if (ctx == NULL) {
           fprintf(stderr, "Unable to allocate libmodbus context\n");
           return -1;
       }

    dlt645_set_debug(ctx, FALSE);    // set debug message
    dlt645_set_slave(ctx, mccb_addr);  // set default slave device id

    if (dlt645_connect(ctx) == -1) {
       fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));

       QMessageBox msgBox;
       QString errmesg = QString(QLatin1String(ctx->backend_data->device));
       errmesg.append(" Open Failed!");
       qCritical() << errmesg;
       msgBox.information(nullptr, "Err!",errmesg, "Close");
       return -1;
    }

    return 0;
}



int gdlt645::read_abcphase_voltage(const uint64_t devaddr, uint32_t* dest)
{
    uint32_t addr = 0x0201FF00;
    return read_registers(devaddr, addr, dest, ReadDataMulti);
}

int gdlt645::read_abcphase_current(const uint64_t devaddr, uint32_t* dest)
{
    uint32_t addr = 0x0202FF00;
    return read_registers(devaddr, addr, dest, ReadDataMulti);
}

int gdlt645::read_isopen_state(const uint64_t devaddr)
{
   uint32_t data;
   read_run_status(devaddr,&data);
   if((data & 0x30) == 0){    // is opened
       return 0;
   }
   return 1;   // is closed
}


int gdlt645::read_run_status(const uint64_t devaddr, uint32_t* dest)
{

    uint32_t addr = 0x04000501;
    return read_one_registers(devaddr, addr, dest);

}

int gdlt645::read_swtcnt_num(const uint64_t devaddr, uint32_t* dest)
{
    uint32_t swtcntadd = 0x00008103;
     return  read_one_registers(devaddr, swtcntadd, dest);    // swtich count needed

}



int gdlt645::read_one_registers(const uint64_t devaddr, uint32_t addr, uint32_t* dest)
{


    return read_registers(devaddr, addr, dest, ReadDataOne);
}

/**like time, manufactor name**/
int gdlt645::read_serial_registers(const uint64_t devaddr, uint32_t addr, uint8_t* dest)
{

    return read_registers(devaddr, addr, dest, ReadDataSerial);
}



// type: RegOne: <=4 Byte; RegOneLong: > 4Byte; RegMulti:FF
int gdlt645::read_registers(const uint64_t devaddr, uint32_t addr,
                                 void *dest, tRegType type)
{
    int rc = 0;
    dlt645_set_slave(ctx, devaddr);

#ifdef _DEBUG_TIME_ELAPSE
    /***Test More Time***/
    timeval start, end;
    gettimeofday(&start, nullptr);
#endif

    rc = dlt645_read_registers(ctx, addr, dest, type);     //read more registers!   declear as c, use DLT645_BEGIN_DECLS
    if(rc == -1)
    {
     qWarning() << " Error, to read! addr:" << addr;
     return -1;
    }

#ifdef _DEBUG_TIME_ELAPSE
    gettimeofday(&end, nullptr);
    printf("modbus_read_registers elapsed:%ld secs, %ld usecs\n;", end.tv_sec - start.tv_sec, \
    end.tv_usec - start.tv_usec);
#endif

    return 0;


}





int gdlt645::write_registers(const uint64_t devaddr, uint32_t addr, void *data, int len, tRegType type)
{
    int rc;
    ctx->backend->set_slave(ctx, devaddr);
    rc = dlt645_write_register(ctx, addr, data, len, type);

    return 0;
}


int gdlt645::dev_ctrl(const uint64_t devaddr, tCtrlType cmd, int tv)
{
    uint8_t data[2] = {0x00, 0x02};   // exectute right now!
    const uint32_t openaddr = 0x06010201;
    const uint32_t closeaddr = 0x06010101;
    uint32_t ctrladdr;


    if(cmd == MOpen){
        ctrladdr = openaddr;

    }
    else{
        ctrladdr = closeaddr;
    }

    if(write_registers(devaddr, ctrladdr, data, 2, WriteData) < 0){

        qWarning() << "Control MCCB Fails! ID:" << devaddr;
        return -1;

    }

    return 0;


}










