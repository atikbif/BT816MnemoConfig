#include "lcdconfcreator.h"
#include <array>
#include "checksum.h"
#include <QDebug>
#include <QRegExp>
#include "checksum.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>

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
    res.append(static_cast<char>(inputs.size()));

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
        if(aiUserNameUTF8.count()>=ai_user_name.size()) {
            aiUserNameUTF8.resize(static_cast<int>(ai_user_name.size()-2));
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
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfDI);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    // input cnt
    std::vector<DiscreteInp> inputs = plcConf.getDiscreteInputs();
    res.append(static_cast<char>(inputs.size()));

    for(const auto &di:inputs) {
        res.append(static_cast<char>(di.usedFlag));

        std::array<char,40> di_sys_name;
        for(char &v:di_sys_name) v = 0;
        QByteArray diSysNameUTF8 = di.sysName.toUtf8();
        if(diSysNameUTF8.count()>=di_sys_name.size()) {
            diSysNameUTF8.resize(static_cast<int>(di_sys_name.size()-2));
        }
        std::copy(diSysNameUTF8.begin(),diSysNameUTF8.end(),di_sys_name.begin());
        for(char v:di_sys_name) {
            res.append(v);
        }

        std::array<char,40> di_user_name;
        for(char &v:di_user_name) v = 0;
        QByteArray diUserNameUTF8 = di.userName.toUtf8();
        if(diUserNameUTF8.count()>=di_user_name.size()) {
            diUserNameUTF8.resize(static_cast<int>(di_user_name.size()-2));
        }
        std::copy(diUserNameUTF8.begin(),diUserNameUTF8.end(),di_user_name.begin());
        for(char v:di_user_name) {
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

QByteArray LCDConfCreator::getDOConfig(uint32_t par)
{
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfDO);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    // input cnt
    std::vector<DiscreteOutput> outs = plcConf.getDiscreteOutputs();
    res.append(static_cast<char>(outs.size()));

    for(const auto &dout:outs) {

        std::array<char,40> do_sys_name;
        for(char &v:do_sys_name) v = 0;
        QByteArray doSysNameUTF8 = dout.sysName.toUtf8();
        if(doSysNameUTF8.count()>=do_sys_name.size()) {
            doSysNameUTF8.resize(static_cast<int>(do_sys_name.size()-2));
        }
        std::copy(doSysNameUTF8.begin(),doSysNameUTF8.end(),do_sys_name.begin());
        for(char v:do_sys_name) {
            res.append(v);
        }

        std::array<char,40> do_user_name;
        for(char &v:do_user_name) v = 0;
        QByteArray doUserNameUTF8 = dout.userName.toUtf8();
        if(doUserNameUTF8.count()>=do_user_name.size()) {
            doUserNameUTF8.resize(static_cast<int>(do_user_name.size()-2));
        }
        std::copy(doUserNameUTF8.begin(),doUserNameUTF8.end(),do_user_name.begin());
        for(char v:do_user_name) {
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

QByteArray LCDConfCreator::getNetRegConfig(uint32_t par)
{
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfNetReg);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    std::vector<SysVar> vars = plcConf.getSysVarByType(SysVarType::NET_REG);
    uint16_t varCnt = static_cast<uint16_t>(vars.size());
    res.append(static_cast<char>(varCnt>>8));
    res.append(static_cast<char>(varCnt&0xFF));

    for(const auto &var:vars) {
        std::array<char,40> sys_name;
        for(char &v:sys_name) v = 0;
        QByteArray sysNameUTF8 = var.sysName.toUtf8();
        if(sysNameUTF8.count()>=sys_name.size()) {
            sysNameUTF8.resize(static_cast<int>(sys_name.size()-2));
        }
        std::copy(sysNameUTF8.begin(),sysNameUTF8.end(),sys_name.begin());
        for(char v:sys_name) {
            res.append(v);
        }

        std::array<char,40> user_name;
        for(char &v:user_name) v = 0;
        QByteArray userNameUTF8 = var.userName.toUtf8();
        if(userNameUTF8.count()>=user_name.size()) {
            userNameUTF8.resize(static_cast<int>(user_name.size()-2));
        }
        std::copy(userNameUTF8.begin(),userNameUTF8.end(),user_name.begin());
        for(char v:user_name) {
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

QByteArray LCDConfCreator::getClusterRegConfig(uint32_t par)
{
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfClusterReg);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    std::vector<SysVar> vars = plcConf.getSysVarByType(SysVarType::CLUSTER_REG);
    uint16_t varCnt = static_cast<uint16_t>(vars.size());
    res.append(static_cast<char>(varCnt>>8));
    res.append(static_cast<char>(varCnt&0xFF));

    for(const auto &var:vars) {
        std::array<char,40> sys_name;
        for(char &v:sys_name) v = 0;
        QByteArray sysNameUTF8 = var.sysName.toUtf8();
        if(sysNameUTF8.count()>=sys_name.size()) {
            sysNameUTF8.resize(static_cast<int>(sys_name.size()-2));
        }
        std::copy(sysNameUTF8.begin(),sysNameUTF8.end(),sys_name.begin());
        for(char v:sys_name) {
            res.append(v);
        }

        std::array<char,40> user_name;
        for(char &v:user_name) v = 0;
        QByteArray userNameUTF8 = var.userName.toUtf8();
        if(userNameUTF8.count()>=user_name.size()) {
            userNameUTF8.resize(static_cast<int>(user_name.size()-2));
        }
        std::copy(userNameUTF8.begin(),userNameUTF8.end(),user_name.begin());
        for(char v:user_name) {
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

QByteArray LCDConfCreator::getNetBitConfig(uint32_t par)
{
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfNetBit);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    std::vector<SysVar> vars = plcConf.getSysVarByType(SysVarType::NET_BIT);
    uint16_t varCnt = static_cast<uint16_t>(vars.size());
    res.append(static_cast<char>(varCnt>>8));
    res.append(static_cast<char>(varCnt&0xFF));

    for(const auto &var:vars) {
        std::array<char,40> sys_name;
        for(char &v:sys_name) v = 0;
        QByteArray sysNameUTF8 = var.sysName.toUtf8();
        if(sysNameUTF8.count()>=sys_name.size()) {
            sysNameUTF8.resize(static_cast<int>(sys_name.size()-2));
        }
        std::copy(sysNameUTF8.begin(),sysNameUTF8.end(),sys_name.begin());
        for(char v:sys_name) {
            res.append(v);
        }

        std::array<char,40> user_name;
        for(char &v:user_name) v = 0;
        QByteArray userNameUTF8 = var.userName.toUtf8();
        if(userNameUTF8.count()>=user_name.size()) {
            userNameUTF8.resize(static_cast<int>(user_name.size()-2));
        }
        std::copy(userNameUTF8.begin(),userNameUTF8.end(),user_name.begin());
        for(char v:user_name) {
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

QByteArray LCDConfCreator::getClusterBitConfig(uint32_t par)
{
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfClusterBit);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    std::vector<SysVar> vars = plcConf.getSysVarByType(SysVarType::CLUSTER_BIT);
    uint16_t varCnt = static_cast<uint16_t>(vars.size());
    res.append(static_cast<char>(varCnt>>8));
    res.append(static_cast<char>(varCnt&0xFF));

    for(const auto &var:vars) {
        std::array<char,40> sys_name;
        for(char &v:sys_name) v = 0;
        QByteArray sysNameUTF8 = var.sysName.toUtf8();
        if(sysNameUTF8.count()>=sys_name.size()) {
            sysNameUTF8.resize(static_cast<int>(sys_name.size()-2));
        }
        std::copy(sysNameUTF8.begin(),sysNameUTF8.end(),sys_name.begin());
        for(char v:sys_name) {
            res.append(v);
        }

        std::array<char,40> user_name;
        for(char &v:user_name) v = 0;
        QByteArray userNameUTF8 = var.userName.toUtf8();
        if(userNameUTF8.count()>=user_name.size()) {
            userNameUTF8.resize(static_cast<int>(user_name.size()-2));
        }
        std::copy(userNameUTF8.begin(),userNameUTF8.end(),user_name.begin());
        for(char v:user_name) {
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

QByteArray LCDConfCreator::getCalculationConfig(uint32_t par)
{
    union {
        float a;
        unsigned char bytes[4];
    } float_converter;

    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfCalculation);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    std::vector<AnalogueInp> inputs = plcConf.getAnalogueInputs();

    uint16_t varCnt = static_cast<uint16_t>(inputs.size());
    res.append(static_cast<char>(varCnt>>8));
    res.append(static_cast<char>(varCnt&0xFF));

    int aiNum = 0;

    for(const auto &ai:inputs) {
        res.append(static_cast<char>(aiNum&0xFF));  // index
        res.append('\0'); // link type (RAW AI)
        res.append('\0');   // divider/precision
        AnalogInputConfig aiConf = getAnalogInputConfig(ai);
        float_converter.a = aiConf.k;
        res.append(static_cast<char>(float_converter.bytes[0]));
        res.append(static_cast<char>(float_converter.bytes[1]));
        res.append(static_cast<char>(float_converter.bytes[2]));
        res.append(static_cast<char>(float_converter.bytes[3]));
        float_converter.a = aiConf.b;
        res.append(static_cast<char>(float_converter.bytes[0]));
        res.append(static_cast<char>(float_converter.bytes[1]));
        res.append(static_cast<char>(float_converter.bytes[2]));
        res.append(static_cast<char>(float_converter.bytes[3]));
        aiNum++;
    }

    int crc = CheckSum::getCRC16(res);

    res.push_back(static_cast<char>(crc>>8));
    res.push_back(static_cast<char>(crc&0xFF));

    int length = res.count();

    res[lengthOffset] = length>>8;
    res[lengthOffset+1] = length &0xFF;

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
    const int lengthOffset = 2;
    const int varCntOffset = 6;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfInpDescr);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    uint16_t varCnt = 0;
    res.append(static_cast<char>(varCnt>>8));
    res.append(static_cast<char>(varCnt&0xFF));

    std::vector<AnalogueInp> anInputs = plcConf.getAnalogueInputs();
    std::vector<DiscreteInp> dinInputs = plcConf.getDiscreteInputs();

    if(anInputs.size()==dinInputs.size()) {
        int inpCnt = static_cast<int>(anInputs.size());
        for(int i=0;i<inpCnt;i++) {
            if(!dinInputs.at(i).userName.isEmpty()) {
                res.push_back('\0'); // input type (di)
                res.push_back(static_cast<char>(i)); // input num
                std::array<char,40> input_user_name;
                for(char &v:input_user_name) v = 0;
                QByteArray inputUserNameUTF8 = dinInputs.at(i).userName.toUtf8();
                if(inputUserNameUTF8.count()>=input_user_name.size()) {
                    inputUserNameUTF8.resize(static_cast<int>(input_user_name.size()-2));
                }
                std::copy(inputUserNameUTF8.begin(),inputUserNameUTF8.end(),input_user_name.begin());
                for(char v:input_user_name) {
                    res.append(v);
                }
                varCnt++;
            }else if(!anInputs.at(i).userName.isEmpty()) {
                res.push_back(1); // input type (ai)
                res.push_back(static_cast<char>(i)); // input num
                std::array<char,40> input_user_name;
                for(char &v:input_user_name) v = 0;
                QByteArray inputUserNameUTF8 = anInputs.at(i).userName.toUtf8();
                if(inputUserNameUTF8.count()>=input_user_name.size()) {
                    inputUserNameUTF8.resize(static_cast<int>(input_user_name.size()-2));
                }
                std::copy(inputUserNameUTF8.begin(),inputUserNameUTF8.end(),input_user_name.begin());
                for(char v:input_user_name) {
                    res.append(v);
                }
                varCnt++;
            }
        }
    }

    res[varCntOffset] = static_cast<char>(varCnt>>8);
    res[varCntOffset+1] = static_cast<char>(varCnt&0xFF);

    int crc = CheckSum::getCRC16(res);

    res.push_back(static_cast<char>(crc>>8));
    res.push_back(static_cast<char>(crc&0xFF));

    int length = res.count();

    res[lengthOffset] = length>>8;
    res[lengthOffset+1] = length &0xFF;

    return res;
}

QByteArray LCDConfCreator::getOutputDescriptionConfig(uint32_t par)
{
    const int lengthOffset = 2;
    const int varCntOffset = 6;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfOutputDescr);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    uint16_t varCnt = 0;
    res.append(static_cast<char>(varCnt>>8));
    res.append(static_cast<char>(varCnt&0xFF));

    std::vector<DiscreteOutput> outs = plcConf.getDiscreteOutputs();
    res.append(static_cast<char>(outs.size()));

    int doNum = 0;
    for(const auto &dout:outs) {
        if(!dout.userName.isEmpty()) {

            res.append(static_cast<char>(doNum));

            std::array<char,40> do_user_name;
            for(char &v:do_user_name) v = 0;
            QByteArray doUserNameUTF8 = dout.userName.toUtf8();
            if(doUserNameUTF8.count()>=do_user_name.size()) {
                doUserNameUTF8.resize(static_cast<int>(do_user_name.size()-2));
            }
            std::copy(doUserNameUTF8.begin(),doUserNameUTF8.end(),do_user_name.begin());
            for(char v:do_user_name) {
                res.append(v);
            }
            varCnt++;
        }
        doNum++;
    }

    res[varCntOffset] = static_cast<char>(varCnt>>8);
    res[varCntOffset+1] = static_cast<char>(varCnt&0xFF);

    int crc = CheckSum::getCRC16(res);

    res.push_back(static_cast<char>(crc>>8));
    res.push_back(static_cast<char>(crc&0xFF));

    int length = res.count();

    res[lengthOffset] = length>>8;
    res[lengthOffset+1] = length &0xFF;

    return res;
}

QByteArray LCDConfCreator::getBackgroundImageConfig(uint32_t par)
{
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfBackgrImage);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    uint32_t imageLength = 0;

    QByteArray imageData;

    if(!backgroundImageFileName.isEmpty()) {
        if(QFile::exists(backgroundImageFileName)) {
            QString path = QFileInfo(backgroundImageFileName).dir().absolutePath();
            QString fName = QFileInfo(backgroundImageFileName).baseName();
            fName += "_800x480_COMPRESSED_RGBA_ASTC_10x10_KHR.raw";
            fName = path + "/conversion/" + fName;
            if(QFile::exists(fName)) {
                QFile imageFile = QFile(fName);
                if(imageFile.open(QFile::ReadOnly)) {
                    imageData = imageFile.readAll();
                }
            }
        }
    }

    imageLength = static_cast<uint32_t>(imageData.count());

    res.append((imageLength>>24)&0xFF);
    res.append((imageLength>>16)&0xFF);
    res.append((imageLength>>16)&0xFF);
    res.append((imageLength>>0)&0xFF);

    res.append(imageData);


    int crc = CheckSum::getCRC16(res);

    res.push_back(static_cast<char>(crc>>8));
    res.push_back(static_cast<char>(crc&0xFF));

    int length = res.count();

    res[lengthOffset] = length>>8;
    res[lengthOffset+1] = length &0xFF;

    return res;
}

QByteArray LCDConfCreator::getMnemoConfig(uint32_t par, uint32_t backgroundAddr)
{
    const int lengthOffset = 2;
    Q_UNUSED(par)
    QByteArray res;
    uint16_t idNum = static_cast<uint16_t>(ConfID::ConfMnemo);
    res.append(static_cast<char>(idNum>>8));
    res.append(static_cast<char>(idNum&0xFF));
    // length
    res.append('\0');
    res.append('\0');

    // version
    res.append('\0');
    res.append(1);

    std::vector<uint8_t> mnemoData;

    uint16_t mnemo_key = 0x15A2;
    mnemoData.push_back(static_cast<uint8_t>(mnemo_key>>8));
    mnemoData.push_back(static_cast<uint8_t>(mnemo_key&0xFF));

    uint16_t itemCnt = 0;

    if(!backgroundImageFileName.isEmpty()) {
        itemCnt++;
    }

    itemCnt+=static_cast<uint16_t>(graphicsItems.size());
    itemCnt+=static_cast<uint16_t>(textItems.size());

    mnemoData.push_back(static_cast<uint8_t>(itemCnt>>8));
    mnemoData.push_back(static_cast<uint8_t>(itemCnt&0xFF));

    int headerOffset = static_cast<int>(mnemoData.size());

    // place for items offset address

    for(int i=0;i<static_cast<int>(itemCnt);i++) {
        mnemoData.push_back(0);
        mnemoData.push_back(0);
    }

    uint16_t dataOffset = static_cast<uint16_t>(mnemoData.size());

    int itemNum = 0;

    // add background image item
    if(!backgroundImageFileName.isEmpty()) {

        mnemoData[headerOffset+itemNum*2] = dataOffset>>8;
        mnemoData[headerOffset+itemNum*2+1] = dataOffset&0xFF;

        // get background item data
        std::vector<uint8_t> itemData = getBackgroundItemData(backgroundAddr);
        for(int i=0;i<itemData.size();i++) mnemoData.push_back(itemData.at(i));
        dataOffset+=static_cast<uint16_t>(itemData.size());
        itemNum++;
    }

    for(int i=0;i<graphicsItems.size();i++) {
        mnemoData[headerOffset+itemNum*2] = dataOffset>>8;
        mnemoData[headerOffset+itemNum*2+1] = dataOffset&0xFF;

        // get item data
        std::vector<uint8_t> itemData = getItemMnemoData(graphicsItems.at(i));

        for(int j=0;j<itemData.size();i++) mnemoData.push_back(itemData.at(j));
        dataOffset+=static_cast<uint16_t>(itemData.size());
        itemNum++;
    }

    for(int i=0;i<textItems.size();i++) {
        mnemoData[headerOffset+itemNum*2] = dataOffset>>8;
        mnemoData[headerOffset+itemNum*2+1] = dataOffset&0xFF;

        // get item data
        std::vector<uint8_t> itemData = getItemMnemoData(textItems.at(i));

        for(int j=0;j<itemData.size();i++) mnemoData.push_back(itemData.at(j));
        dataOffset+=static_cast<uint16_t>(itemData.size());
        itemNum++;
    }

    for(uint8_t v:mnemoData) {
        res.push_back(static_cast<char>(v));
    }

    int crc = CheckSum::getCRC16(res);

    res.push_back(static_cast<char>(crc>>8));
    res.push_back(static_cast<char>(crc&0xFF));

    int length = res.count();

    res[lengthOffset] = length>>8;
    res[lengthOffset+1] = length &0xFF;

    return res;
}

void LCDConfCreator::addEmptyByte(QByteArray &conf)
{
    conf.append('\0');
}

AnalogInputConfig LCDConfCreator::getAnalogInputConfig(AnalogueInp inp)
{
    AnalogInputConfig res;
    res.k = 1;
    res.b = 0;
    res.overLimit = 65535;
    res.overAlarmLimit = 65535;
    res.underLimit = 0;
    res.underAlarmLimit = 0;

    switch (inp.sensor.sensType) {
        case SensType::NC:
        res.k = static_cast<float>(255.0/2000);
        res.b = 0;
        break;
    case SensType::I0__20mA:
        res.k = static_cast<float>(255.0/20000);
        res.b = 0;
        break;
    case SensType::I2__10mA:
        res.k = static_cast<float>(255.0/800);
        res.b = static_cast<float>(-255.0/4);
        res.underAlarmLimit = 1000;
        res.underLimit = 1700;
        res.overLimit = 10250;
        res.overAlarmLimit = 11000;
        break;
    case SensType::I4__20mA:
        res.k = static_cast<float>(255.0/16000);
        res.b = static_cast<float>(-255.0/4);
        res.underAlarmLimit = 2000;
        res.underLimit = 3400;
        res.overLimit = 20500;
        res.overAlarmLimit = 22000;
        break;
    case SensType::U0_4__2V:
        res.k = static_cast<float>(255.0/1600);
        res.b = static_cast<float>(-255.0/4);
        res.underAlarmLimit = 200;
        res.underLimit = 340;
        res.overLimit = 2050;
        res.overAlarmLimit = 2200;
        break;
    case SensType::U0__2_5V:
        res.k = static_cast<float>(255.0/2500);
        res.b = 0;
        break;
    }
    return res;
}

std::vector<uint8_t> LCDConfCreator::getItemMnemoData(RectItem *item)
{
    std::vector<uint8_t> res;

    auto properties = item->getProperties();

    QString itemType;

    for(const auto &pr:properties) {
        if((pr.getType()==ElProperty::Type::STRING_T)) {
            if(pr.getName()=="type") {
                auto prVal = pr.getValue();
                if(auto val = std::get_if<QString>(&prVal)) {
                    itemType = val->toStdString().c_str();
                }
                break;
            }
        }
    }

    if(itemType=="text") {

    }else if(itemType=="filled_rect") {

    }else if(itemType=="filled_circle") {

    }else if(itemType=="lamp") {

    }else if(itemType=="number") {

    }
    return res;
}

std::vector<uint8_t> LCDConfCreator::getBackgroundItemData(uint32_t addr)
{
    std::vector<uint8_t> res;

    uint16_t backgroundImageId = 2;

    res.push_back(static_cast<uint8_t>(backgroundImageId>>8));
    res.push_back(static_cast<uint8_t>(backgroundImageId&0xFF));

    res.push_back(0x01); // version

    res.push_back(static_cast<uint8_t>((addr>>24)&0xFF));
    res.push_back(static_cast<uint8_t>((addr>>16)&0xFF));
    res.push_back(static_cast<uint8_t>((addr>>8)&0xFF));
    res.push_back(static_cast<uint8_t>((addr>>0)&0xFF));

    return res;
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

void LCDConfCreator::setBackgroundImage(const QString &value)
{
    backgroundImageFileName = value;
}

void LCDConfCreator::setGraphicsItems(std::vector<RectItem *> items)
{
    graphicsItems = items;
}

void LCDConfCreator::setTextItems(std::vector<RectItem *> items)
{
    textItems = items;
}
