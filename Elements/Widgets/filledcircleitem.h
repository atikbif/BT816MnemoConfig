#ifndef FILLEDCIRCLEITEM_H
#define FILLEDCIRCLEITEM_H

#include "rectitem.h"

class FilledCircleItem : public RectItem
{
    Q_OBJECT
protected:
    ColorValue backColor;
    static ColorValue lastBackColor;
    bool fill;
    static bool lastFill;
public:
    FilledCircleItem(qreal _width, qreal _height, QObject *parent=nullptr);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    RectItem* clone();
    void updateProperty(ElProperty prop);
};

#endif // FILLEDCIRCLEITEM_H
