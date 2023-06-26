#include "editablesysvariables.h"

void EditableSysVariables::setVars(const std::vector<SysVar> &v)
{
    vars = v;
}

std::vector<SysVar> EditableSysVariables::getVars() const
{
    return vars;
}
