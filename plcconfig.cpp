#include "plcconfig.h"

std::vector<SysVar> PLCConfig::getSysVarByType(SysVarType vType) const
{
    Q_UNUSED(vType)
    std::vector<SysVar> result;
    return result;
}

std::vector<DiscreteInp> PLCConfig::getDiscreteInputs() const
{
    std::vector<DiscreteInp> result;
    return result;
}

std::vector<AnalogueInp> PLCConfig::getAnalogueInputs() const
{
    std::vector<AnalogueInp> result;
    return result;
}

std::vector<DiscreteOutput> PLCConfig::getDiscreteOutputs() const
{
    std::vector<DiscreteOutput> result;
    return result;
}

PLCConfig::PLCConfig(const QString plcName)
{

}
