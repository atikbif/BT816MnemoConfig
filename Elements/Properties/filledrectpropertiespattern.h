#ifndef FILLEDRECTPROPERTIESPATTERN_H
#define FILLEDRECTPROPERTIESPATTERN_H

#include "elproperty.h"
#include <vector>


class FilledRectPropertiesPattern
{
public:
    FilledRectPropertiesPattern()=default;
    static std::vector<ElProperty> getProperties();
};

#endif // FILLEDRECTPROPERTIESPATTERN_H
