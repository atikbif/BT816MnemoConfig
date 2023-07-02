#include "lampitem.h"
#include <QPainter>
#include "lampimages.h"
#include <QDebug>

int LampItem::lastOnIndex = 1;
int LampItem::lastOffIndex = 0;
DiscreteVarType LampItem::lastLinkType = DiscreteVarType::DI;

LampItem::LampItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{
    chMode = ChangeMode::NoChange;

    linkType = lastLinkType;

    ElProperty pr = ElProperty("change",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(chMode));
    RectItem::updateProperty(pr);

    pr = ElProperty("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("lamp"));
    updateProperty(pr);
    onIndex = lastOnIndex;
    offIndex = lastOffIndex;

    pr = ElProperty("bool_state",ElProperty::Type::BOOL_T);
    pr.setValue(state);
    properties.push_back(pr);

    pr = ElProperty("lamp_on_index",ElProperty::Type::INT_T);
    pr.setValue(onIndex);
    properties.push_back(pr);

    pr = ElProperty("lamp_off_index",ElProperty::Type::INT_T);
    pr.setValue(offIndex);
    properties.push_back(pr);

    pr = ElProperty("link_bool_type",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(linkType));
    properties.push_back(pr);

    pr = ElProperty("link_bool_index",ElProperty::Type::INT_T);
    pr.setValue(linkIndex);
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
    if(prop.getName()=="bool_state") {
        if(prop.getType()==ElProperty::Type::BOOL_T) {
            auto lVal = prop.getValue();
            if(auto val = std::get_if<bool>(&lVal)) {
                state = *val;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="bool_state";});
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
                lastOnIndex = onIndex;
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
                lastOffIndex = offIndex;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_off_index";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="link_bool_type") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto lVal = prop.getValue();
            if(auto val = std::get_if<int>(&lVal)) {
                linkType = static_cast<DiscreteVarType>(*val);
                lastLinkType = linkType;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_bool_type";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="link_bool_index") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto lVal = prop.getValue();
            if(auto val = std::get_if<int>(&lVal)) {
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_bool_index";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }
}
