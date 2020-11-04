#ifndef GSVR_INFO_MCCB_H
#define GSVR_INFO_MCCB_H

#include <stdint.h>

class infoMCCB   // record status of switch
{
public:
    infoMCCB();
    ~infoMCCB();
    void DataInfoHandle();
    void DataSettingHandle();

                /***107 Registers Total*****/
    struct DevData {

            uint32_t openState;   // 00 = bihe; 0x03 = duankai;
            bool warnState;
            bool faultState;
            uint32_t runState;   // many
            uint32_t currentI_E_A;
            uint32_t currentI_E_B;
            uint32_t currentI_E_C;
            uint32_t lossCurrVal;
            uint32_t voltageV_E_A;
            uint32_t voltageV_E_B;
            uint32_t voltageV_E_C;
            uint32_t lossPhaseType;
            uint32_t restIVal;
            uint32_t lossNoDriveTime;
            uint32_t numOfSwitch;
        };

    typedef struct{
        // SetBVal, SetEn, SetCor

        double OverProtectI;
        double OverProtectITime;
        double OverProtectV;
        double OverProtectVTime;
        double UnderProtectV;
        double UnderProtectVTime;
        double LossPhaseProV;
        double LossPhaseProTime;
        double OverProtectTemp;
        double OverProtectTempTime;
        double OverValtageWarnVal;
        double UnderValtageWarnVal;
        double OverTempWarnVal;

        uint64_t OverLongDelayProEn;
        uint64_t OverProtectVEn;
        uint64_t UnderProtectVEn;
        uint64_t LossPhaseProtectEn;
        uint64_t OverProtectTempEn;
        uint64_t openOnStartUpEn;
        uint64_t closeByHandProEn;
        uint64_t closeByHandRemoteCloseEn;
        uint64_t tryJumpReCloseEn;

        double correctI_A;
        double correctI_B;
        double correctI_C;
        double correctLosI;
        double correctVol_A;
        double correctVol_B;
        double correctVol_C;

    }tSetBData;

    typedef struct{

        uint32_t IsOpenState;    // 1: bihe; 0: duankai
        uint32_t IsWarState;
        uint32_t IsFaultState;
        uint32_t isRunState;
        float IA;
        float IB;
        float IC;
        float ILoss;
        float UA;
        float UB;
        float UC;
        uint32_t PhaseType;
        uint32_t numOfSw;
        float RestCurrentVal;
        uint32_t TimeOfLossUnDrive;
    }tInfoData;

    // this is been set for SetBVal, SetBEn, SetBCor
    tSetBData SetBData;
    DevData currentData;   // All meta data from mccb!
    tInfoData InfoData;
    int IsOk;   // is device ok?  1: err; 0:ok

private:


};

#endif // INFOMODEL_H
