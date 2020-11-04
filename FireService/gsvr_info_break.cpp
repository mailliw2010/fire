#include "gsvr_info_break.h"
#include <QDebug>
#include <string.h>



infoModel::infoModel()
{
    currentData = new DevData;
    memset(currentData, 0, sizeof(infoModel::DevData));

    this->InfoData.ECEY = 0;
    this->FaultCount = 0;
    this->FaultProtect = 0;
    this->FaultPase = 0;
    this->FaultType = 0;
    this->FaultValue = 0;
    this->InfoData.IA = 0;
    this->InfoData.IB = 0;
    this->InfoData.IC = 0;
    this->InfoData.IE = 0;
    this->InfoData.IMAX = 0;
    this->InfoData.IsFaultState = 0;
    this->IsOk = 0;
    this->InfoData.IsOpenState = 1;
//    this->btn = nullptr;
    this->InfoData.IsWarState = 0;
    this->InfoData.OPENCOUNTS = 0;
    this->InfoData.P = 0;
    this->InfoData.PA = 0;
    this->InfoData.PB = 0;
    this->InfoData.PC = 0;
    this->InfoData.PF = 0;
    this->InfoData.PFA = 0;
    this->InfoData.PFB = 0;
    this->InfoData.PFC = 0;
    this->InfoData.Q = 0;
    this->InfoData.QA = 0;
    this->InfoData.QB = 0;
    this->InfoData.QC = 0;
    this->InfoData.isRunState = 0;
    this->InfoData.S = 0;
    this->InfoData.SA = 0;
    this->InfoData.SB = 0;
    this->InfoData.SC = 0;
    this->InfoData.TMP1 = 0;
    this->InfoData.TMP2 = 0;
    this->InfoData.TMP3 = 0;
    this->InfoData.TMP4 = 0;
    this->InfoData.UA = 0;
    this->InfoData.UB = 0;
    this->InfoData.UC = 0;
    this->InfoData.UMAX = 0;
    this->InfoData.UUNBAL = 0;
    this->devid = 0;
    this->index = 0;
}

infoModel::~infoModel()
{
   delete currentData;
   qDebug() << "deleting infoModel";


}



void infoModel::DataInfoHandle()   //analysis data to class infomodel
{

    if(currentData->openState == 0)
    {
        this->InfoData.IsOpenState = 1;   // 0 = open; 1 = close; different from datasheet   ;changed in 20200925: 1 = open; 0 = close
    }
    else {
        this->InfoData.IsOpenState = 0;
    }

    this->InfoData.IsWarState = 0;
    this->InfoData.IsFaultState = 0;
    this->InfoData.isRunState = 0;

    this->InfoData.IA = currentData->currentI_E_A*0.1;
    this->InfoData.IB = currentData->currentI_E_B*0.1;
    this->InfoData.IC = currentData->currentI_E_C*0.1;
    this->InfoData.IE = currentData->lossCurrVal;
    this->InfoData.UA = currentData->voltageV_E_A*0.1;
    this->InfoData.UB = currentData->voltageV_E_B*0.1;
    this->InfoData.UC = currentData->voltageV_E_C*0.1;
    this->InfoData.UUNBAL = currentData->unbalanceValOfTriPhase;
    this->InfoData.TMP1 = currentData->tempValT1;
    this->InfoData.TMP2 = currentData->tempValT2;
    this->InfoData.TMP3 = currentData->tempValT3;
    this->InfoData.TMP4 = currentData->tempValT4;
    this->InfoData.UMAX = currentData->maxVol*0.1;
    this->InfoData.IMAX = currentData->maxCurr*0.1;
    this->InfoData.PA = currentData->activePwA*0.01;
    this->InfoData.PB = currentData->activePwB*0.01;
    this->InfoData.PC = currentData->activePwC*0.01;
    //this->InfoData.P = currentData->activePwTol*0.01;
    this->InfoData.QA = currentData->reactivePwA*0.01;
    this->InfoData.QB = currentData->reactivePwB*0.01;
    this->InfoData.QC = currentData->reactivePwC*0.01;
    this->InfoData.Q = currentData->reactivePwTol*0.01;
    this->InfoData.SA = currentData->apparentPwA*0.01;
    this->InfoData.SB = currentData->apparentPwB*0.01;
    this->InfoData.SC = currentData->apparentPwC*0.01;
    this->InfoData.S = currentData->apparentPwTol*0.01;
    this->InfoData.PFA = currentData->factorPwA*0.01;
    this->InfoData.PFB = currentData->factorPwB*0.01;
    this->InfoData.PFC = currentData->factorPwC*0.01;
    this->InfoData.PF = currentData->factorPwTol*0.01;
    this->InfoData.ECEY = ((uint64_t)currentData->elecPw_HBit*65536 + (uint64_t)currentData->elecPw_LBit)/1000;
//    this->InfoData.ECEY = currentData->elecPw_LBit;
    this->InfoData.OPENCOUNTS = currentData->numOfSwitch;

}


void infoModel::DataSettingHandle()   //analysis data to class infomodel
{

    this->SetBData.OverProtectI = currentData->OverProtectI*0.1;
    this->SetBData.OverProtectITime = currentData->OverProtectITime;
    this->SetBData.OverProtectV = currentData->OverProtectV*0.1;
    this->SetBData.OverProtectVTime = currentData->OverProtectVTime;
    this->SetBData.UnderProtectV = currentData->UnderProtectV*0.1;
    this->SetBData.UnderProtectVTime = currentData->UnderProtectVTime;
    this->SetBData.LossPhaseProV = currentData->LossPhaseProV*0.1;
    this->SetBData.LossPhaseProTime = currentData->LossPhaseProTime;
    this->SetBData.OverProtectTemp = currentData->OverProtectTemp;
    this->SetBData.OverProtectTempTime = currentData->OverProtectTempTime;
    this->SetBData.OverValtageWarnVal = currentData->OverValtageWarnVal*0.1;
    this->SetBData.UnderValtageWarnVal = currentData->UnderValtageWarnVal*0.1;
    this->SetBData.OverTempWarnVal = currentData->OverTempWarnVal;

    this->SetBData.OverLongDelayProEn = currentData->OverLongDelayProEn;
    this->SetBData.OverProtectVEn = currentData->OverProtectVEn;
    this->SetBData.UnderProtectVEn = currentData->UnderProtectVEn;
    this->SetBData.LossPhaseProtectEn = currentData->LossPhaseProtectEn;
    this->SetBData.OverProtectTempEn = currentData->OverProtectTempEn;

    this->SetBData.correctI_A = currentData->correctI_A*0.1;
    this->SetBData.correctI_B = currentData->correctI_B*0.1;
    this->SetBData.correctI_C = currentData->correctI_C*0.1;
    this->SetBData.correctLosI = currentData->correctLosI*0.1;
    this->SetBData.correctVol_A = currentData->correctVol_A*0.1;
    this->SetBData.correctVol_B = currentData->correctVol_B*0.1;
    this->SetBData.correctVol_C = currentData->correctVol_C*0.1;
    this->SetBData.correctVol_T1 = currentData->correctVol_T1;
    this->SetBData.correctVol_T2 = currentData->correctVol_T2;
    this->SetBData.correctVol_T3 = currentData->correctVol_T3;
    this->SetBData.correctVol_T4 = currentData->correctVol_T4;

}



