#include "filledrectitem.h"
#include <QPainter>
#include <QDebug>

ColorValue FilledRectItem::lastBackColor = {0xAA,0xFF,0x7F};
int FilledRectItem::lastRadius = 0;
bool FilledRectItem::lastFill = true;

FilledRectItem::FilledRectItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{
    backColor = lastBackColor;
    ElProperty pr("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("filled_rect"));
    properties.push_back(pr);

    pr = ElProperty("back_color",ElProperty::Type::STRING_T);
    QColor col(backColor.r,backColor.g,backColor.b);
    QString colStr = col.name(QColor::HexRgb);
    colStr = colStr.mid(1,6);
    pr.setValue(colStr);
    properties.push_back(pr);

    radius = lastRadius;
    pr = ElProperty("rect_radius",ElProperty::Type::INT_T);
    pr.setValue(radius);
    properties.push_back(pr);

    fill = lastFill;
    pr = ElProperty("fill",ElProperty::Type::BOOL_T);
    pr.setValue(fill);
    properties.push_back(pr);
}

void FilledRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selectionMode==SelectionMode::None) painter->setOpacity(1.0);
    else painter->setOpacity(0.8);
    painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),lineWidth));

    QPainterPath path;
    path.addRoundedRect(QRectF(0,0,width,height), radius, radius);
    if(fill) painter->fillPath(path, QBrush(QColor(backColor.r,backColor.g,backColor.b)));
    if(lineWidth==0) {
        painter->setPen(QPen(QBrush(Qt::white),1));
        path.addRoundedRect(QRectF(0,0,width,height), radius, radius);
    }
    painter->drawPath(path);

    drawBorder(painter);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void FilledRectItem::updateProperty(ElProperty prop)
{
    RectItem::updateProperty(prop);
    if(prop.getName()=="back_color") {
        if(prop.getType()==ElProperty::Type::STRING_T) {
            auto colVal = prop.getValue();
            if(auto val = std::get_if<QString>(&colVal)) {
                bool res = false;
                long col = val->toLong(&res,16);
                if(res) {
                    int r = (col>>16)&0xFF;
                    int g = (col>>8)&0xFF;
                    int b = col&0xFF;
                    backColor = {r,g,b};
                    lastBackColor = backColor;
                    update();
                    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="back_color";});
                    if(it!=properties.end()) {
                        it->setValue(*val);
                    }
                }
            }
        }
    }else if(prop.getName()=="rect_radius") {
        auto radVal = prop.getValue();
        if(auto val = std::get_if<int>(&radVal)) {
            radius = *val;
            lastRadius = radius;
            update();
            auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="rect_radius";});
            if(it!=properties.end()) {
                it->setValue(*val);
            }
        }
    }else if(prop.getName()=="fill") {
        auto fillVal = prop.getValue();
        if(auto val = std::get_if<bool>(&fillVal)) {
            fill = *val;
            lastFill = fill;
            update();
            auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="fill";});
            if(it!=properties.end()) {
                it->setValue(*val);
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
