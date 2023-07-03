#ifndef ELPROPERTY_H
#define ELPROPERTY_H

#include <QString>
#include <variant>

class ElProperty
{
public:
    enum class Type {BOOL_T, INT_T, DOUBLE_T, STRING_T};
private:
    QString name;
    Type type;
    std::variant<bool,int,double,QString> value;
public:
    ElProperty(const QString &name, Type type);
    Type getType() const;
    std::variant<bool,int,double,QString> getValue() const;
    QString getName() const;
    void setValue(bool v);
    void setValue(int v);
    void setValue(double v);
    void setValue(const QString &v);

    static QString getStringFromProperty(ElProperty pr);
    static int getIntFromProperty(ElProperty pr);
    static bool getBoolFromProperty(ElProperty pr);
};

#endif // ELPROPERTY_H
