#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include "rectitem.h"

class CircleItem : public RectItem
{
    Q_OBJECT
public:
    CircleItem(qreal _width, qreal _height, QObject *parent=nullptr);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // RectItem interface
public:
    RectItem* clone();
};

#endif // CIRCLEITEM_H
