#include "editablesysvariables.h"
#include <QJsonArray>

void EditableSysVariables::setVars(const std::vector<SysVar> &v)
{
    vars = v;
}

std::vector<SysVar> EditableSysVariables::getVars() const
{
    return vars;
}

void EditableSysVariables::toJSON(QJsonObject &ob)
{
    QJsonArray varArray;
    for(const auto &v:vars) {
        QJsonObject vOb;
        vOb["sys_name"] = v.sysName;
        vOb["user_name"] = v.userName;
        if(v.varType==SysVarType::CLUSTER_BIT) vOb["type"] = "CL_BIT";
        else if(v.varType==SysVarType::NET_BIT) vOb["type"] = "NET_BIT";
        else if(v.varType==SysVarType::CLUSTER_REG) vOb["type"] = "CL_REG";
        else if(v.varType==SysVarType::NET_REG) vOb["type"] = "NET_REG";
        vOb["index"] = v.num;
        varArray.push_back(vOb);
    }
    ob.insert("editable variables",varArray);
}

void EditableSysVariables::fromJSON(const QJsonObject &ob)
{
    vars.clear();
    if(ob.contains("editable variables") && ob["editable variables"].isArray()) {
        QJsonArray varArray = ob["editable variables"].toArray();
        for(const auto &v:varArray) {
            QJsonObject vOb = v.toObject();
            SysVar sVar;
            sVar.sysName = vOb["sys_name"].toString();
            sVar.userName = vOb["user_name"].toString();
            QString varType = vOb["type"].toString();
            if(varType=="CL_BIT") {
                sVar.varType = SysVarType::CLUSTER_BIT;
            }else if(varType=="NET_BIT") {
                sVar.varType = SysVarType::NET_BIT;
            }else if(varType=="CL_REG") {
                sVar.varType = SysVarType::CLUSTER_REG;
            }else if(varType=="NET_REG") {
                sVar.varType = SysVarType::NET_REG;
            }
            vars.push_back(sVar);
        }
    }
}

void EditableSysVariables::clear()
{
    vars.clear();
}
