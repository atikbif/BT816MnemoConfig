#ifndef LCDCONFCREATOR_H
#define LCDCONFCREATOR_H

#include <QByteArray>
#include "plcconfig.h"

enum class ConfID {ConfAppl, ConfPassw, ConfAI, ConfDI, ConfDO, ConfNetReg, ConfClusterReg,
            ConfNetBit, ConfClusterBit, ConfCalculation, ConfCAN, ConfInpDescr, ConfOutputDescr,
            ConfBackgrImage, ConfMnemo};

struct AnalogInputConfig {
    float k;
    float b;
    uint16_t underLimit;
    uint16_t underAlarmLimit;
    uint16_t overLimit;
    uint16_t overAlarmLimit;
};

class LCDConfCreator
{
    PLCConfig plcConf;
    uint8_t canAddr = 1;
    QByteArray getApplicationConfig(uint32_t par);
    QByteArray getPasswordConfig(uint32_t par);
    QByteArray getAIConfig(uint32_t par);
    QByteArray getDIConfig(uint32_t par);
    QByteArray getDOConfig(uint32_t par);
    QByteArray getNetRegConfig(uint32_t par);
    QByteArray getClusterRegConfig(uint32_t par);
    QByteArray getNetBitConfig(uint32_t par);
    QByteArray getClusterBitConfig(uint32_t par);
    QByteArray getCalculationConfig(uint32_t par);
    QByteArray getCANConfig(uint32_t par);
    QByteArray getInputDescriptionConfig(uint32_t par);
    QByteArray getOutputDescriptionConfig(uint32_t par);
    QByteArray getBackgroundImageConfig(uint32_t par);
    QByteArray getMnemoConfig(uint32_t par, uint32_t backgroundAddr);
    void addEmptyByte(QByteArray &conf);
    AnalogInputConfig getAnalogInputConfig(AnalogueInp inp);
public:
    LCDConfCreator() = default;
    QByteArray createLCDConf();
    void setPLCConfig(const PLCConfig &conf);
    void setCanAddr(uint8_t value);
};

#endif // LCDCONFCREATOR_H
