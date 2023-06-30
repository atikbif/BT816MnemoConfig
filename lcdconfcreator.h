#ifndef LCDCONFCREATOR_H
#define LCDCONFCREATOR_H

#include <QByteArray>

enum class ConfID {ConfAppl, ConfPassw, ConfAI, ConfDI, ConfDO, ConfNetReg, ConfClusterReg,
            ConfNetBit, ConfClusterBit, ConfCalculation, ConfCAN, ConfInpDescr, ConfOutputDescr,
            ConfBackgrImage, ConfMnemo};

class LCDConfCreator
{
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
public:
    LCDConfCreator() = default;
    QByteArray createLCDConf();
};

#endif // LCDCONFCREATOR_H
