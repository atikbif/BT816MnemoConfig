#include "ellipseitem.h"
#include <QPainter>

EllipseItem::EllipseItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{

}

void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selectionMode==SelectionMode::None) painter->setOpacity(1.0);
    else painter->setOpacity(0.3);
    if(lineWidth) {
        painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),lineWidth));
        painter->drawEllipse(QRectF(0,0,width,height));
    }

    drawBorder(painter);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

RectItem *EllipseItem::clone()
{
    EllipseItem* res = new EllipseItem(width,height);
    res->lineWidth = lineWidth;
    res->setX(getX());
    res->setY(getY());
    for(const auto &pr:properties) res->updateProperty(pr);
    return res;
}
