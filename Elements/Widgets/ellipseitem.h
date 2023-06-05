#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H

#include "rectitem.h"

class EllipseItem : public RectItem
{
    Q_OBJECT
public:
    EllipseItem(qreal _width, qreal _height, QObject *parent=nullptr);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // RectItem interface
public:
    RectItem* clone();
};

#endif // ELLIPSEITEM_H
