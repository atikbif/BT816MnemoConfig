#include "filledrectpropertiespattern.h"


std::vector<ElProperty> FilledRectPropertiesPattern::getProperties()
{
    const QString backgrPropGroup = "цвет заливки";
    const QString colorPropGroup = "цвет контура";

    std::vector<ElProperty> res;
//    res.push_back(ElProperty("радиус",ElProperty::Type::INT_T));
//    res.push_back(ElProperty("заливка",ElProperty::Type::BOOL_T));
//    res.push_back(ElProperty("R",ElProperty::Type::INT_T));
//    res.push_back(ElProperty("G",ElProperty::Type::INT_T,true,backgrPropGroup));
//    res.push_back(ElProperty("B",ElProperty::Type::INT_T,true,backgrPropGroup));
//    res.push_back(ElProperty("R",ElProperty::Type::INT_T,true,colorPropGroup));
//    res.push_back(ElProperty("G",ElProperty::Type::INT_T,true,colorPropGroup));
//    res.push_back(ElProperty("B",ElProperty::Type::INT_T,true,colorPropGroup));
    return res;
}
