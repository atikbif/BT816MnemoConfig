#ifndef FILLEDELLIPSEITEM_H
#define FILLEDELLIPSEITEM_H

#include "rectitem.h"

class FilledEllipseItem : public RectItem
{
    Q_OBJECT
protected:
    ColorValue backColor;
    static ColorValue lastBackColor;
    bool fill;
    static bool lastFill;
public:
    FilledEllipseItem(qreal _width, qreal _height, QObject *parent=nullptr);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    RectItem* clone();
    void updateProperty(ElProperty prop);
};

#endif // FILLEDELLIPSEITEM_H
