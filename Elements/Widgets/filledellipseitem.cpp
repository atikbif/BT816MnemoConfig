#include "filledellipseitem.h"
#include <QPainter>

ColorValue FilledEllipseItem::lastBackColor = {0xAA,0xFF,0x7F};
bool FilledEllipseItem::lastFill = true;


FilledEllipseItem::FilledEllipseItem(qreal _width, qreal _height, QObject *parent): RectItem(_width,_height,parent)
{
    backColor = lastBackColor;
    ElProperty pr("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("filled_ellipse"));
    properties.push_back(pr);

    pr = ElProperty("back_color",ElProperty::Type::STRING_T);
    QColor col(backColor.r,backColor.g,backColor.b);
    QString colStr = col.name(QColor::HexRgb);
    colStr = colStr.mid(1,6);
    pr.setValue(colStr);
    properties.push_back(pr);

    fill = lastFill;
    pr = ElProperty("fill",ElProperty::Type::BOOL_T);
    pr.setValue(fill);
    properties.push_back(pr);
}

void FilledEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selectionMode==SelectionMode::None) painter->setOpacity(1.0);
    else painter->setOpacity(0.8);
    painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),lineWidth));

    QPainterPath path;
    path.addEllipse(QRectF(0,0,width,height));
    if(fill) painter->fillPath(path, QBrush(QColor(backColor.r,backColor.g,backColor.b)));
    if(lineWidth==0) {
        painter->setPen(QPen(QBrush(Qt::white),1));
        path.addEllipse(QRectF(0,0,width,height));
    }
    painter->drawPath(path);

    drawBorder(painter);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

RectItem *FilledEllipseItem::clone()
{
    FilledEllipseItem* res = new FilledEllipseItem(width,height);
    res->lineWidth = lineWidth;
    res->setX(getX());
    res->setY(getY());
    for(const auto &pr:properties) res->updateProperty(pr);
    return res;
}

void FilledEllipseItem::updateProperty(ElProperty prop)
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
