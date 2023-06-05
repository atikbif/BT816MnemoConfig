#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include <QObject>
#include <QVBoxLayout>
#include <vector>
#include <map>
#include "elproperty.h"
#include <QSpinBox>

class ViewSpinbox: public QSpinBox
{
    Q_OBJECT
public:
    explicit ViewSpinbox(QWidget *parent=nullptr):QSpinBox(parent){
        connect(this,&QSpinBox::editingFinished,[this](){emit valChanged(value());});
    }
    void stepBy(int steps) {
        QSpinBox::stepBy(steps);
        emit valChanged(value());
    }
signals:
    void valChanged(int value);
};

class PropertiesView : public QObject
{
    Q_OBJECT
    QVBoxLayout *layout = nullptr;
    std::map<QString, QWidget*> widgets;

    void clearLayout(QLayout* l);
    QString getStringFromProperty(ElProperty pr);
    int getIntFromProperty(ElProperty pr);
    bool getBoolFromProperty(ElProperty pr);
public:
    explicit PropertiesView(QObject *parent = nullptr);
    QLayout * getLayout() {return layout;}
    void setProperties(const std::vector<ElProperty> &properties);

    void updateRect(int x, int y, int w, int h, int lw);
    void clearProperties();
signals:
    void update_x(int value);
    void update_y(int value);
    void update_width(int value);
    void update_height(int value);
    void update_line_width(int value);
    void updateProperty(const ElProperty &prop);
};

#endif // PROPERIESVIEW_H
