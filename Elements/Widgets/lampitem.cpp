#include "lampitem.h"
#include <QPainter>
#include "lampimages.h"
#include <QDebug>

int LampItem::lastOnIndex = 1;
int LampItem::lastOffIndex = 0;
int LampItem::lastVarType = 0;
int LampItem::lastVarIndex = 1;

LampItem::LampItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{
    chMode = ChangeMode::NoChange;

    ElProperty pr = ElProperty("change",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(chMode));
    RectItem::updateProperty(pr);

    pr = ElProperty("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("lamp"));
    properties.push_back(pr);
    onIndex = lastOnIndex;
    offIndex = lastOffIndex;

    pr = ElProperty("lamp_state",ElProperty::Type::BOOL_T);
    pr.setValue(state);
    properties.push_back(pr);

    pr = ElProperty("lamp_on_index",ElProperty::Type::INT_T);
    pr.setValue(onIndex);
    properties.push_back(pr);

    pr = ElProperty("lamp_off_index",ElProperty::Type::INT_T);
    pr.setValue(offIndex);
    properties.push_back(pr);

    varType = lastVarType;
    varIndex = lastVarIndex;

    pr = ElProperty("lamp_var_type",ElProperty::Type::INT_T);
    pr.setValue(varType);
    properties.push_back(pr);

    pr = ElProperty("lamp_var_index",ElProperty::Type::INT_T);
    pr.setValue(varIndex);
    properties.push_back(pr);
}

void LampItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selectionMode==SelectionMode::None) painter->setOpacity(1.0);
    else painter->setOpacity(0.8);

    painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),lineWidth));
    QPixmap pix;
    if(state) pix.load(LampImages::getImage(onIndex));
    else pix.load(LampImages::getImage(offIndex));
    painter->drawPixmap(0,0,static_cast<int>(width),static_cast<int>(height),pix);
    if(lineWidth) painter->drawRect(QRectF(0,0,width,height));

    drawBorder(painter,false);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

RectItem *LampItem::clone()
{
    LampItem *res = new LampItem(width,height);
    res->lineWidth = lineWidth;
    res->setX(getX());
    res->setY(getY());
    for(const auto &pr:properties) res->updateProperty(pr);
    return res;
}

void LampItem::updateProperty(ElProperty prop)
{
    RectItem::updateProperty(prop);
    if(prop.getName()=="lamp_state") {
        if(prop.getType()==ElProperty::Type::BOOL_T) {
            auto lVal = prop.getValue();
            if(auto val = std::get_if<bool>(&lVal)) {
                state = *val;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_state";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="lamp_on_index") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto onVal = prop.getValue();
            if(auto val = std::get_if<int>(&onVal)) {
                onIndex = *val;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_on_index";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="lamp_off_index") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto onVal = prop.getValue();
            if(auto val = std::get_if<int>(&onVal)) {
                offIndex = *val;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_off_index";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="lamp_var_type") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto vVal = prop.getValue();
            if(auto val = std::get_if<int>(&vVal)) {
                varType = *val;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_var_type";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="lamp_var_index") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto vVal = prop.getValue();
            if(auto val = std::get_if<int>(&vVal)) {
                varIndex = *val;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_var_index";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }
}
