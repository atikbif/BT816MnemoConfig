#ifndef SCALABLEIMAGEITEM_H
#define SCALABLEIMAGEITEM_H

#include "rectitem.h"

class ScalableImageItem : public RectItem
{
    Q_OBJECT
    int index;
    static int lastIndex;
public:
    ScalableImageItem(qreal _width, qreal _height, QObject *parent=nullptr);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateProperty(ElProperty prop);
    RectItem* clone();
};

#endif // SCALABLEIMAGEITEM_H
