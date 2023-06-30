#include "lcdconfcreator.h"
#include <array>
#include "checksum.h"
#include <QDebug>
#include <QRegExp>
#include "checksum.h"

QByteArray LCDConfCreator::getApplicationConfig(uint32_t par)
{
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfAppl);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    res.append(static_cast<char>(plcConf.getAppCN()>>8));
    res.append(static_cast<char>(plcConf.getAppCN()&0xFF));
    QString vers = plcConf.getAppVersion();
    QRegExp versExp = QRegExp("^(\\d+)\\D(\\d+)$");

    quint8 majorVers = 0;
    quint8 minorVers = 0;

    if(versExp.indexIn(vers)!=-1) {
        majorVers = versExp.cap(1).toInt()&0xFF;
        minorVers = versExp.cap(2).toInt()&0xFF;
    }

    res.append(majorVers);
    res.append(minorVers);

    QString appTime = plcConf.getAppTime();
    QRegExp timeExp = QRegExp("^(\\d+)\\D(\\d+)\\D(\\d+)\\D(\\d+)\\D(\\d+)\\D(\\d+)$");

    uint8_t app_date = 1;
    uint8_t app_month = 1;
    uint8_t app_year = 0;
    uint8_t app_hour = 0;
    uint8_t app_min = 0;
    uint8_t app_sec = 0;

    if(timeExp.indexIn(appTime)!=-1) {
        app_date = timeExp.cap(1).toInt() & 0xFF;
        app_month = timeExp.cap(2).toInt() & 0xFF;
        app_year = timeExp.cap(3).toInt() % 100;

        app_hour = timeExp.cap(4).toInt() & 0xFF;
        app_min = timeExp.cap(5).toInt() & 0xFF;
        app_sec = timeExp.cap(6).toInt() & 0xFF;
    }

    res.append(app_date);
    res.append(app_month);
    res.append(app_year);
    res.append(app_hour);
    res.append(app_min);
    res.append(app_sec);

    quint16 app_crc = plcConf.getAppCheckSum();

    res.append(static_cast<char>(app_crc>>8));
    res.append(static_cast<char>(app_crc&0xFF));

    std::array<char,40> app_name;

    for(char &v:app_name) v = 0;

    QByteArray appNameUTF8 = plcConf.getAppName().toUtf8();
    if(appNameUTF8.count()>=app_name.size()) {
        appNameUTF8.resize(static_cast<int>(app_name.size()-2));
    }

    std::copy(appNameUTF8.begin(),appNameUTF8.end(),app_name.begin());

    for(char v:app_name) {
        res.append(v);
    }

    int crc = CheckSum::getCRC16(res);

    res.push_back(static_cast<char>(crc>>8));
    res.push_back(static_cast<char>(crc&0xFF));

    int length = res.count();

    res[lengthOffset] = length>>8;
    res[lengthOffset+1] = length &0xFF;

    return res;
}

QByteArray LCDConfCreator::getPasswordConfig(uint32_t par)
{
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfPassw);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    // password value

    res.append(1);
    res.append(1);
    res.append(1);
    res.append(1);
    res.append(1);
    res.append(1);

    // password rewrite flag

    res.append(1);

    int crc = CheckSum::getCRC16(res);

    res.push_back(static_cast<char>(crc>>8));
    res.push_back(static_cast<char>(crc&0xFF));

    int length = res.count();

    res[lengthOffset] = length>>8;
    res[lengthOffset+1] = length &0xFF;

    return res;
}

QByteArray LCDConfCreator::getAIConfig(uint32_t par)
{
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfAI);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    // input cnt
    std::vector<AnalogueInp> inputs = plcConf.getAnalogueInputs();
    res.append(inputs.size());

    for(const auto &ai:inputs) {
        res.append(static_cast<char>(ai.inpType));
        res.append(static_cast<char>(ai.usedFlag));

        std::array<char,40> ai_sys_name;
        for(char &v:ai_sys_name) v = 0;
        QByteArray aiSysNameUTF8 = ai.sysName.toUtf8();
        if(aiSysNameUTF8.count()>=ai_sys_name.size()) {
            aiSysNameUTF8.resize(static_cast<int>(ai_sys_name.size()-2));
        }
        std::copy(aiSysNameUTF8.begin(),aiSysNameUTF8.end(),ai_sys_name.begin());
        for(char v:ai_sys_name) {
            res.append(v);
        }

        std::array<char,40> ai_user_name;
        for(char &v:ai_user_name) v = 0;
        QByteArray aiUserNameUTF8 = ai.userName.toUtf8();
        if(aiUserNameUTF8.count()>=ai_sys_name.size()) {
            aiUserNameUTF8.resize(static_cast<int>(ai_sys_name.size()-2));
        }
        std::copy(aiUserNameUTF8.begin(),aiUserNameUTF8.end(),ai_user_name.begin());
        for(char v:ai_user_name) {
            res.append(v);
        }

        res.append(static_cast<char>(ai.sensor.sensType));

        std::array<char,20> meas_unit;
        for(char &v:meas_unit) v = 0;
        QByteArray aiMeasUnitUTF8 = ai.sensor.measureUnit.toUtf8();
        if(aiMeasUnitUTF8.count()>=meas_unit.size()) {
            aiMeasUnitUTF8.resize(static_cast<int>(meas_unit.size()-2));
        }
        std::copy(aiMeasUnitUTF8.begin(),aiMeasUnitUTF8.end(),meas_unit.begin());
        for(char v:meas_unit) {
            res.append(v);
        }
    }

    int crc = CheckSum::getCRC16(res);

    res.push_back(static_cast<char>(crc>>8));
    res.push_back(static_cast<char>(crc&0xFF));

    int length = res.count();

    res[lengthOffset] = length>>8;
    res[lengthOffset+1] = length &0xFF;

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
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfCAN);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    res.append(canAddr);

    int crc = CheckSum::getCRC16(res);

    res.push_back(static_cast<char>(crc>>8));
    res.push_back(static_cast<char>(crc&0xFF));

    int length = res.count();

    res[lengthOffset] = length>>8;
    res[lengthOffset+1] = length &0xFF;

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

    while(res.count()<static_cast<int>(dataStartPos)) {
        addEmptyByte(res);
    }
    res.append(dataArray);
    return res;
}

void LCDConfCreator::setPLCConfig(const PLCConfig &conf)
{
    plcConf = conf;
}

void LCDConfCreator::setCanAddr(uint8_t value)
{
    canAddr = value;
}
