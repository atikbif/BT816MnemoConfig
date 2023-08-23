#ifndef EDITABLESYSVARIABLES_H
#define EDITABLESYSVARIABLES_H

#include "plcconfig.h"
#include "QJsonObject"

class EditableSysVariables
{
    std::vector<SysVar> vars;
public:
    EditableSysVariables() = default;
    void setVars(const std::vector<SysVar> &v);
    std::vector<SysVar> getVars() const;
    void toJSON(QJsonObject &ob);
    void updateUserName(const std::vector<SysVar> &sysVars);
    void fromJSON(const QJsonObject &ob);
    void clear();
};

#endif // EDITABLESYSVARIABLES_H
