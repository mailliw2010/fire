#ifndef GSVR_INFO_MCCB_H
#define GSVR_INFO_MCCB_H

#include <stdint.h>
class infomccb
{
public:
    infomccb();
    ~infomccb();
    void DataInfoHandle();
    void DataSettingHandle();


    struct DevData {
             /****0x00-0x31:50******/
            uint16_t devComid;
            uint16_t baurd;
            uint16_t reserved_0x0002;
            uint16_t companyId;
            uint16_t devType;
            uint16_t versionNum;
            uint16_t createTime;
            uint16_t reserved_0x0007;
            uint16_t currentI;
            uint16_t voltageV;
            uint16_t devID_0;
            uint16_t devID_1;
            uint16_t devID_2;
            uint16_t devID_3;
            uint16_t devID_4;
            uint16_t devID_5;

            /**from here to 0x31 , the datas must upload to server**/
            uint16_t openState;   // 0 = open; 1 = close; different from datasheet
            uint16_t currentI_E_A;
            uint16_t currentI_E_B;
            uint16_t currentI_E_C;
            uint16_t lossCurrVal;
            uint16_t voltageV_E_A;
            uint16_t voltageV_E_B;
            uint16_t voltageV_E_C;
            uint16_t unbalanceValOfTriPhase;
            uint16_t tempValT1;
            uint16_t tempValT2;
            uint16_t tempValT3;
            uint16_t tempValT4;
            uint16_t maxVol;
            uint16_t maxCurr;
            uint16_t activePwA;
            uint16_t activePwB;
            uint16_t activePwC;
            ///  总有功功率
            uint16_t activePwTol;
            uint16_t reactivePwA;
            uint16_t reactivePwB;
            uint16_t reactivePwC;
            uint16_t reactivePwTol;
            uint16_t apparentPwA;
            uint16_t apparentPwB;
            uint16_t apparentPwC;
            uint16_t apparentPwTol;
            uint16_t factorPwA;
            uint16_t factorPwB;
            uint16_t factorPwC;
            uint16_t factorPwTol;
            uint16_t elecPw_HBit;
            uint16_t elecPw_LBit;
            uint16_t numOfSwitch;

            /****0x40-0x63:42******/
            /*SetValue**/
            uint16_t OverProtectI;
            uint16_t OverProtectITime;
            uint16_t OverProtectV;
            uint16_t OverProtectVTime;
            uint16_t UnderProtectV;
            uint16_t UnderProtectVTime;
            uint16_t LossPhaseProV;
            uint16_t LossPhaseProTime;
            uint16_t OverProtectTemp;
            uint16_t OverProtectTempTime;
            uint16_t OverValtageWarnVal;
            uint16_t UnderValtageWarnVal;
            uint16_t OverTempWarnVal;

            uint16_t reserved_0x004D;
            uint16_t reserved_0x004E;
            uint16_t reserved_0x004F;
            /*Enable**/
            uint16_t OverLongDelayProEn;
            uint16_t OverProtectVEn;
            uint16_t UnderProtectVEn;
            uint16_t LossPhaseProtectEn;
            uint16_t OverProtectTempEn;

            uint16_t openOnStartUpEn;
            uint16_t closeByHandProEn;
            uint16_t closeByHandRemoteCloseEn;
            uint16_t tryJumpReCloseEn;
            uint16_t TriOrSinglePhaEdit;
            uint16_t reserved_0x005A;
            uint16_t reserved_0x005B;
            uint16_t reserved_0x005C;
            uint16_t reserved_0x005D;
            uint16_t reserved_0x005E;
            uint16_t reserved_0x005F;
            uint16_t setTkeyEn;
            uint16_t lossElecActVal;
            uint16_t lossElecWARVal;
            uint16_t overTempWarVal;
            uint16_t reserved_0x0064;
            uint16_t reserved_0x0065;
            uint16_t reserved_0x0066;
            uint16_t reserved_0x0067;
            uint16_t reserved_0x0068;
            uint16_t reserved_0x0069;



            /****0x6A-0x74:11******/
            /*Correct**/
            uint16_t correctI_A;
            uint16_t correctI_B;
            uint16_t correctI_C;
            uint16_t correctLosI;
            uint16_t correctVol_A;
            uint16_t correctVol_B;
            uint16_t correctVol_C;
            uint16_t correctVol_T1;
            uint16_t correctVol_T2;
            uint16_t correctVol_T3;
            uint16_t correctVol_T4;

            /****0x0100-0x0104:5******/
            uint16_t ErrNum;
            uint16_t ErrType;
            uint16_t ErrVal;
            uint16_t ErrPhase;
            uint16_t ErrProtect;
        };
    DevData *currentData;


};


#endif // GSVR_INFO_MCCB_H
