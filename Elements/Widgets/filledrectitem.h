#ifndef FILLEDRECTITEM_H
#define FILLEDRECTITEM_H

#include "rectitem.h"

class FilledRectItem : public RectItem
{
    Q_OBJECT
protected:
    ColorValue backColor;
    static ColorValue lastBackColor;
    int radius;
    static int lastRadius;
    bool fill;
    static bool lastFill;
public:
    FilledRectItem(qreal _width, qreal _height, QObject *parent=nullptr);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateProperty(ElProperty prop);
    RectItem* clone();
};

#endif // FILLEDRECTITEM_H
