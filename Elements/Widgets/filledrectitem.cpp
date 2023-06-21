#include "filledrectitem.h"
#include <QPainter>
#include <QDebug>

ColorValue FilledRectItem::lastOnColor = {0x00,0xFF,0x00};
ColorValue FilledRectItem::lastOffColor = {0xA0,0xA0,0xA0};
DiscreteVarType FilledRectItem::lastLinkType = DiscreteVarType::DI;

FilledRectItem::FilledRectItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{
    onColor = lastOnColor;
    offColor = lastOffColor;
    linkType = lastLinkType;

    chMode = ChangeMode::WidthAndHeight;

    ElProperty pr = ElProperty("change",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(chMode));
    RectItem::updateProperty(pr);

    pr = ElProperty("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("filled_rect"));
    properties.push_back(pr);

    pr = ElProperty("on_color",ElProperty::Type::STRING_T);
    QColor col = QColor(onColor.r,onColor.g,onColor.b);
    QString colStr = col.name(QColor::HexRgb);
    colStr = colStr.mid(1,6);
    pr.setValue(colStr);
    properties.push_back(pr);

    pr = ElProperty("off_color",ElProperty::Type::STRING_T);
    col = QColor(offColor.r,offColor.g,offColor.b);
    colStr = col.name(QColor::HexRgb);
    colStr = colStr.mid(1,6);
    pr.setValue(colStr);
    properties.push_back(pr);

    pr = ElProperty("bool_state",ElProperty::Type::BOOL_T);
    pr.setValue(state);
    properties.push_back(pr);

    pr = ElProperty("link_bool_type",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(linkType));
    properties.push_back(pr);

    pr = ElProperty("link_bool_index",ElProperty::Type::INT_T);
    pr.setValue(linkIndex);
    properties.push_back(pr);
}

void FilledRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selectionMode==SelectionMode::None) painter->setOpacity(1.0);
    else painter->setOpacity(0.8);
    painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),lineWidth));

    QPainterPath path;
    path.addRect(QRectF(0,0,width,height));
    if(state) {
        painter->fillPath(path, QBrush(QColor(onColor.r,onColor.g,onColor.b)));
    }else {
        painter->fillPath(path, QBrush(QColor(offColor.r,offColor.g,offColor.b)));
    }

    if(lineWidth==0) {
        painter->setPen(QPen(QBrush(Qt::white),1));
        path.addRect(QRectF(0,0,width,height));
    }
    painter->drawPath(path);

    drawBorder(painter);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void FilledRectItem::updateProperty(ElProperty prop)
{
    RectItem::updateProperty(prop);
    if(prop.getName()=="off_color") {
        if(prop.getType()==ElProperty::Type::STRING_T) {
            auto colVal = prop.getValue();
            if(auto val = std::get_if<QString>(&colVal)) {
                bool res = false;
                long col = val->toLong(&res,16);
                if(res) {
                    int r = (col>>16)&0xFF;
                    int g = (col>>8)&0xFF;
                    int b = col&0xFF;
                    offColor = {r,g,b};
                    lastOffColor = offColor;
                    update();
                    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="off_color";});
                    if(it!=properties.end()) {
                        it->setValue(*val);
                    }
                }
            }
        }
    }else if(prop.getName()=="on_color") {
        if(prop.getType()==ElProperty::Type::STRING_T) {
            auto colVal = prop.getValue();
            if(auto val = std::get_if<QString>(&colVal)) {
                bool res = false;
                long col = val->toLong(&res,16);
                if(res) {
                    int r = (col>>16)&0xFF;
                    int g = (col>>8)&0xFF;
                    int b = col&0xFF;
                    onColor = {r,g,b};
                    lastOnColor = onColor;
                    update();
                    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="on_color";});
                    if(it!=properties.end()) {
                        it->setValue(*val);
                    }
                }
            }
        }
    }else if(prop.getName()=="bool_state") {
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

RectItem *FilledRectItem::clone()
{
    FilledRectItem *res = new FilledRectItem(width,height);
    res->lineWidth = lineWidth;
    res->setX(getX());
    res->setY(getY());
    for(const auto &pr:properties) res->updateProperty(pr);
    return res;
}
