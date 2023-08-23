#ifndef MESSAGEVARIABLES_H
#define MESSAGEVARIABLES_H

#include "plcconfig.h"
#include <QJsonObject>

class MessageVariables
{
public:
    MessageVariables() = default;
    static void toJSON(QJsonObject &ob, const std::vector<AlarmInfoVar> &vars);
    static void fromJSON(const QJsonObject &ob, std::vector<AlarmInfoVar> &vars);
    static void updateUserName(std::vector<AlarmInfoVar> &vars, const std::vector<SysVar> &sysVars);
};

#endif // MESSAGEVARIABLES_H
