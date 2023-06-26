#ifndef EDITABLESYSVARIABLES_H
#define EDITABLESYSVARIABLES_H

#include "plcconfig.h"

class EditableSysVariables
{
    std::vector<SysVar> vars;
public:
    EditableSysVariables() = default;
    void setVars(const std::vector<SysVar> &v);
    std::vector<SysVar> getVars() const;
};

#endif // EDITABLESYSVARIABLES_H
