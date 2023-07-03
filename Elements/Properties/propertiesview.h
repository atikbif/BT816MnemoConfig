#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include <QObject>
#include <QVBoxLayout>
#include <vector>
#include <map>
#include "elproperty.h"
#include <QSpinBox>
#include "plcconfig.h"
#include <QFormLayout>

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
    PLCConfig plc;

    void clearLayout(QLayout* l);

    void drawXYProperties(QFormLayout *fLayout);
    void drawWidthHeightProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawColorProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawOnColorProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawOffColorProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawLinkBoolTypeProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawLinkBoolIndexProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawTextValueProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawEngFontProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawNumPatternProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawNumDividerProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawLinkAnalogueTypeProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawLinkAnalogueIndexProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawCyrFontProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawLampStateProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawLampOnIndexProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);
    void drawLampOffIndexProperties(QFormLayout *fLayout,const std::vector<ElProperty> &properties);

public:
    explicit PropertiesView(QObject *parent = nullptr);
    QLayout * getLayout() {return layout;}
    void setProperties(const std::vector<ElProperty> &properties);

    void updateRect(int x, int y, int w, int h, int lw);
    void clearProperties();
    void setPLCConfig(const PLCConfig &plcConf);
signals:
    void update_x(int value);
    void update_y(int value);
    void update_width(int value);
    void update_height(int value);
    void update_line_width(int value);
    void updateProperty(const ElProperty &prop);
};

#endif // PROPERIESVIEW_H
