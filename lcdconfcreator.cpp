#include "lcdconfcreator.h"
#include <array>
#include "checksum.h"
#include <QDebug>

QByteArray LCDConfCreator::getApplicationConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getPasswordConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getAIConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getDIConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getDOConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getNetRegConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getClusterRegConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getNetBitConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getClusterBitConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getCalculationConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getCANConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getInputDescriptionConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getOutputDescriptionConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getBackgroundImageConfig(uint32_t par)
{
    Q_UNUSED(par)
    QByteArray res;
    res.append(1);
    return res;
}

QByteArray LCDConfCreator::getMnemoConfig(uint32_t par, uint32_t backgroundAddr)
{
    Q_UNUSED(par)
    Q_UNUSED(backgroundAddr)
    QByteArray res;
    res.append(1);
    return res;
}

void LCDConfCreator::addEmptyByte(QByteArray &conf)
{
    conf.append('\0');
}

QByteArray LCDConfCreator::createLCDConf()
{
    uint16_t idNum = 0;
    uint16_t headelLengthOffset = 0;
    QByteArray res;
    std::array<uint8_t,4> confFileID = {0x0D,0x0A,0x0A,0x0D};
    for(auto id:confFileID) {
        res.push_back(id);
    }
    uint32_t headerAddr = 0;
    uint32_t dataStartPos = 4096;
    uint32_t dataOffset = dataStartPos;

    QByteArray dataArray;

    headelLengthOffset = res.count();

    // length
    res.append('\0');
    res.append('\0');

    QByteArray applConf = getApplicationConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfAppl);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(applConf);
    dataOffset += applConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray passwConf = getPasswordConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfPassw);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(passwConf);
    dataOffset += passwConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray aiConf = getAIConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfAI);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(aiConf);
    dataOffset += aiConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray diConf = getDIConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfDI);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(diConf);
    dataOffset += diConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray doConf = getDOConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfDO);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(doConf);
    dataOffset += doConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray netRegConf = getNetRegConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfNetReg);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(netRegConf);
    dataOffset += netRegConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray clRegConf = getClusterRegConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfClusterReg);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(clRegConf);
    dataOffset += clRegConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray netBitConf = getNetBitConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfNetBit);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(netBitConf);
    dataOffset += netBitConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray clBitConf = getClusterBitConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfClusterBit);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(clBitConf);
    dataOffset += clBitConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray calcConf = getCalculationConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfCalculation);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(calcConf);
    dataOffset += calcConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray canConf = getCANConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfCAN);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(canConf);
    dataOffset += canConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray inpDescrConf = getInputDescriptionConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfInpDescr);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(inpDescrConf);
    dataOffset += inpDescrConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray outDescrConf = getOutputDescriptionConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfOutputDescr);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(outDescrConf);
    dataOffset += outDescrConf.count();
    while((dataOffset%4096)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray backgroundImageConf = getBackgroundImageConfig(0);
    idNum = static_cast<uint16_t>(ConfID::ConfBackgrImage);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(backgroundImageConf);
    dataOffset += backgroundImageConf.count();
    while((dataOffset%4096)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    QByteArray mnemoConf = getMnemoConfig(0,0);
    idNum = static_cast<uint16_t>(ConfID::ConfMnemo);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    res.append('\0'); // par
    res.append('\0');
    res.append('\0');
    res.append('\0');
    res.append(static_cast<char>((((headerAddr+dataOffset)>>24)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>16)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>8)&0xFF)));
    res.append(static_cast<char>((((headerAddr+dataOffset)>>0)&0xFF)));
    dataArray.append(mnemoConf);
    dataOffset += mnemoConf.count();
    while((dataOffset%4)!=0) {
        addEmptyByte(dataArray);
        dataOffset++;
    }
    quint16 crc = static_cast<quint16>(CheckSum::getCRC16(res));
    res.push_back((crc>>8)&0xFF);
    res.push_back(crc&0xFF);

    uint16_t length = res.count();

    res[headelLengthOffset] = static_cast<char>(length>>8);
    res[headelLengthOffset+1] = static_cast<char>(length&0xFF);

    while(res.count()<dataStartPos) {
        addEmptyByte(res);
    }
    res.append(dataArray);
    return res;
}
