#include "gsvr_info_mccb.h"
#include <QDebug>
#include <string.h>



infoMCCB::infoMCCB()
{
    memset(&currentData, 0, sizeof(infoMCCB::DevData));
    memset(&InfoData, 0, sizeof(infoMCCB::tInfoData));
}

infoMCCB::~infoMCCB()
{

   qDebug() << "deleting infoModel";


}



void infoMCCB::DataInfoHandle()   //analysis data to class infomodel
{

    if((currentData.runState & 0x30) == 0){    // is opende
        currentData.openState = 0;
        InfoData.IsOpenState = 1;
    }else {    // is closed
        currentData.openState = 1;
        InfoData.IsOpenState = 0;
    }

    if((currentData.runState & 0x80) == 0){
        InfoData.IsWarState = currentData.warnState = 0;
    }else {
        InfoData.IsWarState = currentData.warnState = 1;
    }


    this->InfoData.IsFaultState = currentData.faultState;


    this->InfoData.IA = currentData.currentI_E_A*0.1;
    this->InfoData.IB = currentData.currentI_E_B*0.1;
    this->InfoData.IC = currentData.currentI_E_C*0.1;
    this->InfoData.ILoss = currentData.lossCurrVal;
    this->InfoData.UA = currentData.voltageV_E_A*0.1;
    this->InfoData.UB = currentData.voltageV_E_B*0.1;
    this->InfoData.UC = currentData.voltageV_E_C*0.1;
    this->InfoData.PhaseType = currentData.lossPhaseType;
    this->InfoData.numOfSw = currentData.numOfSwitch;
    this->InfoData.RestCurrentVal = currentData.restIVal*0.1;
    this->InfoData.TimeOfLossUnDrive = currentData.lossNoDriveTime;

}


void infoMCCB::DataSettingHandle()   //analysis data to class infomodel
{



}



