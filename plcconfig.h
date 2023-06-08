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
    QString high_limit;
    QString low_limit;
    QString name;
    QString measure_unit;
};

struct Var {
    QString sysName;
    QString userName;
};

struct SysVar :public Var {
    SysVarType varType;
    int num;
};

struct Input: public Var {
    int num;
    bool usedFlag;
};

struct DiscreteOutput: public Var {
    int num;
    QString faultUserName;
};

struct DiscreteInp : public Input {
    QString shortCircUserName;
    QString breakUserName;
    QString faultUserName;
};

struct AnalogueInp : public Input {
    InpType inpType;
    Sensor sensor;
    QString overUserName;
    QString underUserName;
    QString alarmUserName;
};

class PLCConfig
{
    int app_cn;
    int app_checksum;
    QString app_name;
    QString app_time;
    QString app_version;
    QString plcName;
    std::vector<std::shared_ptr<Input>> input;
    std::vector<DiscreteOutput> out;
    std::vector<SysVar> sysVar;
public:
    std::vector<SysVar> getSysVarByType(SysVarType vType) const;
    std::vector<DiscreteInp> getDiscreteInputs() const;
    std::vector<AnalogueInp> getAnalogueInputs() const;
    std::vector<DiscreteOutput> getDiscreteOutputs() const;
    PLCConfig(const QString plcName);
};

#endif // PLCCONFIG_H
