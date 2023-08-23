#include "messagevariables.h"
#include <QJsonArray>

void MessageVariables::toJSON(QJsonObject &ob, const std::vector<AlarmInfoVar> &vars)
{
    QJsonArray varArray;
    for(const auto &v:vars) {
        QJsonObject vOb;
        vOb["sys_name"] = v.var.sysName;
        vOb["user_name"] = v.var.userName;
        if(v.var.varType==SysVarType::CLUSTER_BIT) vOb["type"] = "CL_BIT";
        else if(v.var.varType==SysVarType::NET_BIT) vOb["type"] = "NET_BIT";
        else if(v.var.varType==SysVarType::CLUSTER_REG) vOb["type"] = "CL_REG";
        else if(v.var.varType==SysVarType::NET_REG) vOb["type"] = "NET_REG";
        vOb["index"] = v.var.num;
        vOb["message_type"] = AlarmInfoVar::getMessageTypeString(v.messageType);
        varArray.push_back(vOb);
    }
    ob.insert("message variables",varArray);
}

void MessageVariables::fromJSON(const QJsonObject &ob, std::vector<AlarmInfoVar> &vars)
{
    vars.clear();
    if(ob.contains("message variables") && ob["message variables"].isArray()) {
        QJsonArray varArray = ob["message variables"].toArray();
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
            AlarmInfoVar messageVar;
            QString messageType = vOb["message_type"].toString();
            if(messageType==AlarmInfoVar::getMessageTypeString(MessageType::InfoData)) messageVar.messageType = MessageType::InfoData;
            else if(messageType==AlarmInfoVar::getMessageTypeString(MessageType::WarningData)) messageVar.messageType = MessageType::WarningData;
            else if(messageType==AlarmInfoVar::getMessageTypeString(MessageType::AlarmData)) messageVar.messageType = MessageType::AlarmData;
            else messageVar.messageType = MessageType::InfoData;
            messageVar.var = sVar;
            vars.push_back(messageVar);
        }
    }
}

void MessageVariables::updateUserName(std::vector<AlarmInfoVar> &vars, const std::vector<SysVar> &sysVars)
{
    for(auto &v:vars) {
        auto it = std::find_if(sysVars.cbegin(),sysVars.cend(),[v](const SysVar &var){return ((v.var.varType==var.varType)&&(v.var.sysName==var.sysName));});
        if(it!=sysVars.cend()) {
            v.var.userName = it->userName;
        }
    }
}
