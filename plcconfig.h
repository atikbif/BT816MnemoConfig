#ifndef PLCCONFIG_H
#define PLCCONFIG_H

#include <QString>
#include <vector>
#include <memory>

enum class InpType {VType, IType};
enum class SensType {NC,U0_4__2V,I0__20mA,I4__20mA,I2__10mA,U0__2_5V};
enum class SysVarType {CLUSTER_BIT,CLUSTER_REG,NET_BIT,NET_REG};

struct Sensor {
    SensType sensType;
    QString highLimit;
    QString lowLimit;
    QString name;
    QString measureUnit;
};

struct Var {
    QString sysName;
    QString userName;
};

struct SysVar :public Var {
    SysVarType varType;
    QString userName;
    QString sysName;
    int num;
};

struct Input: public Var {
    int num;
    bool usedFlag;
};

struct DiscreteOutput: public Var {
    int num;
};

struct DiscreteInp : public Input {

};

struct AnalogueInp : public Input {
    InpType inpType;
    Sensor sensor;
};

class PLCConfig
{
    friend class JSONPLCConfigReader;

    int appCN;
    int appCheckSum;
    QString appName;
    QString appTime;
    QString appVersion;
    QString plcName;
    std::vector<DiscreteInp> dInput;
    std::vector<AnalogueInp> anInput;
    std::vector<DiscreteOutput> dOut;
    std::vector<SysVar> sysVar;
public:
    int getAppCN() const {return appCN;}
    int getAppCheckSum() const {return appCheckSum;}
    QString getAppName() const {return appName;}
    QString getAppTime() const {return appTime;}
    QString getAppVersion() const {return appVersion;}
    QString getPLCName() const {return plcName;}
    std::vector<SysVar> getSysVarByType(SysVarType vType) const;
    std::vector<DiscreteInp> getDiscreteInputs() const;
    std::vector<AnalogueInp> getAnalogueInputs() const;
    std::vector<DiscreteOutput> getDiscreteOutputs() const;
    PLCConfig(const QString plcName);
};

#endif // PLCCONFIG_H
