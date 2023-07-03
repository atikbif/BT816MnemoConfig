#include "elproperty.h"

ElProperty::ElProperty(const QString &name, ElProperty::Type type):
    name(name),type(type)
{
    switch(type) {
    case Type::INT_T:value=0;break;
    case Type::BOOL_T:value=false;break;
    case Type::DOUBLE_T:value=0.0;break;
    case Type::STRING_T:value="";break;
    }
}

ElProperty::Type ElProperty::getType() const
{
    return type;
}

std::variant<bool, int, double, QString> ElProperty::getValue() const
{
    return value;
}

QString ElProperty::getName() const
{
    return name;
}

void ElProperty::setValue(bool v)
{
    if(type==Type::BOOL_T) value = v;
}

void ElProperty::setValue(int v)
{
    if(type==Type::INT_T) value = v;
}

void ElProperty::setValue(double v)
{
    if(type==Type::DOUBLE_T) value = v;
}

void ElProperty::setValue(const QString &v)
{
    if(type==Type::STRING_T) value = v;
}

QString ElProperty::getStringFromProperty(ElProperty pr)
{
    if(pr.getType()==ElProperty::Type::STRING_T) {
        auto prVal = pr.getValue();
        if(auto val = std::get_if<QString>(&prVal)) {
            return *val;
        }
    }
    return "";
}

int ElProperty::getIntFromProperty(ElProperty pr)
{
    if(pr.getType()==ElProperty::Type::INT_T) {
        auto prVal = pr.getValue();
        if(auto val = std::get_if<int>(&prVal)) {
            return *val;
        }
    }
    return 0;
}

bool ElProperty::getBoolFromProperty(ElProperty pr)
{
    if(pr.getType()==ElProperty::Type::BOOL_T) {
        auto prVal = pr.getValue();
        if(auto val = std::get_if<bool>(&prVal)) {
            return *val;
        }
    }
    return false;
}
