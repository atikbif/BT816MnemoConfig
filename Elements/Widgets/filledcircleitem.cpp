#include "filledcircleitem.h"
#include <QPainter>

ColorValue FilledCircleItem::lastBackColor = {0xAA,0xFF,0x7F};

FilledCircleItem::FilledCircleItem(qreal _width, qreal _height, QObject *parent): RectItem(_width,_height,parent)
{
    chMode = ChangeMode::Proportional;

    ElProperty pr = ElProperty("change",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(chMode));
    RectItem::updateProperty(pr);

    backColor = lastBackColor;
    pr = ElProperty("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("filled_circle"));
    properties.push_back(pr);

    pr = ElProperty("back_color",ElProperty::Type::STRING_T);
    QColor col(backColor.r,backColor.g,backColor.b);
    QString colStr = col.name(QColor::HexRgb);
    colStr = colStr.mid(1,6);
    pr.setValue(colStr);
    properties.push_back(pr);
}

void FilledCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selectionMode==SelectionMode::None) painter->setOpacity(1.0);
    else painter->setOpacity(0.8);
    painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),lineWidth));

    QPainterPath path;
    path.addEllipse(QRectF(0,0,width,height));
    painter->fillPath(path, QBrush(QColor(backColor.r,backColor.g,backColor.b)));
    if(lineWidth==0) {
        painter->setPen(QPen(QBrush(Qt::white),1));
        path.addEllipse(QRectF(0,0,width,height));
    }
    painter->drawPath(path);

    drawBorder(painter);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

RectItem *FilledCircleItem::clone()
{
    FilledCircleItem* res = new FilledCircleItem(width,height);
    res->lineWidth = lineWidth;
    res->setX(getX());
    res->setY(getY());
    for(const auto &pr:properties) res->updateProperty(pr);
    return res;
}

void FilledCircleItem::updateProperty(ElProperty prop)
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
    }
}
