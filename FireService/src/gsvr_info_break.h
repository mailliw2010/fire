#ifndef GSVR_INFO_BREAK_H
#define GSVR_INFO_BREAK_H

#include <stdint.h>

class infoModel   // record status of switch
{
public:
    infoModel();
    ~infoModel();
    void DataInfoHandle();
    void DataSetEnHandle();

                /***107 Registers Total*****/
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
    DevData *currentData;   // All data from breaker!

    typedef struct{
        // SetBVal, SetEn, SetCor

        float OverProtectI;
        float OverProtectITime;
        float OverProtectV;
        float OverProtectVTime;
        float UnderProtectV;
        float UnderProtectVTime;
        float LossPhaseProV;
        float LossPhaseProTime;
        float OverProtectTemp;
        float OverProtectTempTime;
        float OverValtageWarnVal;
        float UnderValtageWarnVal;
        float OverTempWarnVal;

        uint32_t OverLongDelayProEn;
        uint32_t OverProtectVEn;
        uint32_t UnderProtectVEn;
        uint32_t LossPhaseProtectEn;
        uint32_t OverProtectTempEn;
        uint32_t openOnStartUpEn;
        uint32_t closeByHandProEn;
        uint32_t closeByHandRemoteCloseEn;
        uint32_t tryJumpReCloseEn;

        float correctI_A;
        float correctI_B;
        float correctI_C;
        float correctLosI;
        float correctVol_A;
        float correctVol_B;
        float correctVol_C;
        float correctVol_T1;
        float correctVol_T2;
        float correctVol_T3;
        float correctVol_T4;
    }tSetBData;


    typedef struct{

        uint32_t IsOpenState;

        /// <summary>
        /// 预警状态 1 - 有预警 0 - 无预警
        /// </summary>
        uint32_t IsWarState;
        /// <summary>
        /// 故障状态  1 - 有故障 0 - 无故障
        /// </summary>
        uint32_t IsFaultState;
        /// <summary>
        /// 00000-正常运行，00001-过载，00010-短路，00011-短瞬，00100-剩余电流，00101 - 过压，00110 - 欠压，00111 - 缺相， 01000 - 电流不平衡，
        /// 01001 - 电压不平衡，01010 - 温度，01011 - 指令试跳，01100 -按键试跳，
        /// 01101 - 按键合 / 分，11110 - 指令合 / 分， 10010 - 手柄合/分， 10000 - 自动合闸
        /// </summary>
        uint32_t isRunState;

        float IA;
        /// <summary>
        /// B 相电流有效值
        /// </summary>
        float IB;
        /// <summary>
        /// C 相电流有效值
        /// </summary>
        float IC;
        /// <summary>
        /// 漏电流有效值
        /// </summary>
        float IE;
        /// <summary>
        /// A 相电压有效值
        /// </summary>
        float UA;
        /// <summary>
        /// B 相电压有效值
        /// </summary>
        float UB;
        /// <summary>
        /// C 相电压有效值
        /// </summary>
        float UC;
        /// <summary>
        /// 三相电压不平衡度
        /// </summary>
        float UUNBAL;
        /// <summary>
        /// T1 温度值
        /// </summary>
        float TMP1;
        /// <summary>
        /// T2 温度值
        /// </summary>
        float TMP2;
        /// <summary>
        /// T3 温度值
        /// </summary>
        float TMP3 ;
        /// <summary>
        /// T4 温度值
        /// </summary>
        float TMP4;
        /// <summary>
        ///最大电压
        /// </summary>
        float UMAX;
        /// <summary>
        /// 最大电流
        /// </summary>
        float IMAX ;
        /// <summary>
        /// A 相有功功率
        /// </summary>
        float PA ;
        /// <summary>
        /// B 相有功功率
        /// </summary>
        float PB ;
        /// <summary>
        /// C 相有功功率
        /// </summary>
        float PC;
        /// <summary>
        ///  总有功功率
        /// </summary>
        float P;
        /// <summary>
        /// A 相无功功率
        /// </summary>
        float QA;
        /// <summary>
        /// B 相无功功率
        /// </summary>
        float QB;
        /// <summary>
        /// C 相无功功率
        /// </summary>
        float QC;
        /// <summary>
        /// 总无功功率
        /// </summary>
        float Q ;
        /// <summary>
        /// A 相视在功率
        /// </summary>
        float SA ;
        /// <summary>
        /// B 相视在功率
        /// </summary>
        float SB;
        /// <summary>
        /// C 相视在功率
        /// </summary>
        float SC;
        /// <summary>
        /// 总视在功率
        /// </summary>
        float S;
        /// <summary>
        /// A 相功率因数
        /// </summary>
        float PFA;
        /// <summary>
        /// B 相功率因数
        /// </summary>
        float PFB;
        /// <summary>
        /// C 相功率因数
        /// </summary>
        float PFC;
        /// <summary>
        ///总功率因数
        /// </summary>
        float PF;
        /// <summary>
        /// 电能 算法(data_2F*65536+data_30)/10000
        /// </summary>
        float ECEY;
        /// <summary>
        /// 开关次数统计
        /// </summary>
        float OPENCOUNTS;   // make to float, make it easy to calculate
    }tInfoData;

    int IsOk;  // OK:0; ERR:1 (read ERR or device does not exist!)
    int index;
    /// <summary>
    /// 设备ID
    /// </summary>
    int devid;

    // this is been set for SetBVal, SetBEn, SetBCor
    tSetBData SetBData;

    /// <summary>
    /// 状态
    /// </summary>
    /// <summary>
    /// 闸位状态 1 - 分闸状态 0 - 合闸状态
    /// </summary>


    /// <summary>
    /// A 相电流有效值
    /// </summary>
    ///
    ///
    tInfoData InfoData;
    // <summary>
    // 故障记录次数
    // </summary>
    uint16_t FaultCount;
    // <summary>
    // 故障类型
    // 0x00 无故障0x01 长延时故障0x02 短延时故障
    //0x03 瞬时故障0x04 剩余电流故障0x05 过压故障
    //0x06 欠压故障0x07 缺相故障0x0A 温度故障
    //0x0B 远程试验0x0C 按键试验

    //
    //
    // </summary>
    uint16_t FaultType;
    // <summary>
    // 故障数值
    // </summary>
    uint16_t FaultValue;
    // <summary>
    // 故障相别
    // //注2：相别类型 0x00 无相
    //0x01 A 相
    //0x02 B 相
    //0x04 C 相
    //0x08 N 相
    // </summary>
    uint16_t FaultPase;
    // <summary>
    // 故障 阀值
    // </summary>
    uint16_t FaultProtect;

private:


};

#endif // INFOMODEL_H
