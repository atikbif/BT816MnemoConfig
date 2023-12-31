#include "plcconfig.h"

std::vector<SysVar> PLCConfig::getSysVarByType(SysVarType vType) const
{
    Q_UNUSED(vType)
    std::vector<SysVar> result;
    if(vType==SysVarType::CLUSTER_BIT) {
        for(const auto&v:sysVar) {
            if(v.varType==SysVarType::CLUSTER_BIT) result.push_back(v);
        }
    }else if(vType==SysVarType::NET_BIT) {
        for(const auto&v:sysVar) {
            if(v.varType==SysVarType::NET_BIT) result.push_back(v);
        }
    }else if(vType==SysVarType::CLUSTER_REG) {
        for(const auto&v:sysVar) {
            if(v.varType==SysVarType::CLUSTER_REG) result.push_back(v);
        }
    }else if(vType==SysVarType::NET_REG) {
        for(const auto&v:sysVar) {
            if(v.varType==SysVarType::NET_REG) result.push_back(v);
        }
    }
    return result;
}

std::vector<SysVar> PLCConfig::getAllSysVar() const
{
    return sysVar;
}

std::vector<Var> PLCConfig::getDiscreteVarByType(DiscreteVarType vType) const
{

    std::vector<Var> result;
    if(vType==DiscreteVarType::DI) {
        for(const auto&v:dInput) {
            result.push_back(v);
        }
    }else if(vType==DiscreteVarType::DO) {
        for(const auto&v:dOut) {
            result.push_back(v);
        }
    }else if(vType==DiscreteVarType::CLUSTER_BIT) {
        for(const auto&v:sysVar) {
            if(v.varType==SysVarType::CLUSTER_BIT) result.push_back(v);
        }
    }else if(vType==DiscreteVarType::NET_BIT) {
        for(const auto&v:sysVar) {
            if(v.varType==SysVarType::NET_BIT) result.push_back(v);
        }
    }
    return result;
}

std::vector<Var> PLCConfig::getAnalogueVarByType(AnalogueVarType vType) const
{
    std::vector<Var> result;
    if(vType==AnalogueVarType::RAW_AI) {
        for(const auto&input:anInput) {
            Var v;
            v.sysName = input.sysName + " RAW";
            v.userName = input.userName;
            result.push_back(v);
        }
    }else if(vType==AnalogueVarType::CALCULATED_AI) {
        for(const auto&input:anInput) {
            Var v;
            v.sysName = input.sysName;
            v.userName = input.userName;
            result.push_back(v);
        }
    }else if(vType==AnalogueVarType::CLUSTER_REG) {
        for(const auto&v:sysVar) {
            if(v.varType==SysVarType::CLUSTER_REG) result.push_back(v);
        }
    }else if(vType==AnalogueVarType::NET_REG) {
        for(const auto&v:sysVar) {
            if(v.varType==SysVarType::NET_REG) result.push_back(v);
        }
    }
    return result;
}

std::vector<DiscreteInp> PLCConfig::getDiscreteInputs() const
{
    return dInput;
}

std::vector<AnalogueInp> PLCConfig::getAnalogueInputs() const
{
    return anInput;
}

std::vector<DiscreteOutput> PLCConfig::getDiscreteOutputs() const
{
    return dOut;
}

std::vector<AlarmInfoVar> PLCConfig::getAlarmInfoVar() const
{
    return infoVar;
}

void PLCConfig::setAlarmInfoVar(const std::vector<AlarmInfoVar> &vars)
{
    infoVar = vars;
}

PLCConfig::PLCConfig(const QString plcName)
{
    if(plcName.contains("PC21")) {
        for(int i=0;i<14;i++) {
            DiscreteInp di;
            di.num = i+1;
            di.sysName = "DI" + QString::number(i+1);
            di.usedFlag = false;

            Sensor s;
            s.sensType = SensType::NC;
            AnalogueInp ai;
            ai.inpType = InpType::IType;
            ai.sensor = s;
            ai.num = i+1;
            ai.sysName = "AI" + QString::number(i+1);
            ai.usedFlag = false;

            dInput.push_back(di);
            anInput.push_back(ai);
        }

        for(int i=0;i<6;i++) {
            DiscreteOutput dout;
            dout.num = i+1;
            dout.sysName = "DO" + QString::number(i+1);
            dOut.push_back(dout);
        }

        for(int i=0;i<64;i++) {
            SysVar v;
            v.num = i;
            v.varType = SysVarType::CLUSTER_REG;
            v.sysName = "CLREG" + QString::number(17+i);
            sysVar.push_back(v);
        }

        for(int i=0;i<28*8;i++) {
            SysVar v;
            v.num = i;
            v.varType = SysVarType::CLUSTER_BIT;
            v.sysName = "CLBIT" + QString::number(17+i);
            sysVar.push_back(v);
        }

        for(int i=0;i<128;i++) {
            SysVar v;
            v.num = i;
            v.varType = SysVarType::NET_BIT;
            v.sysName = "NBIT" + QString::number(257+i);
            sysVar.push_back(v);
        }

        for(int i=0;i<128;i++) {
            SysVar v;
            v.num = i;
            v.varType = SysVarType::NET_REG;
            v.sysName = "NREG" + QString::number(97+i);
            sysVar.push_back(v);
        }
    }
}
