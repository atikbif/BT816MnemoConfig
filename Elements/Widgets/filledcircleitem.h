#ifndef FILLEDCIRCLEITEM_H
#define FILLEDCIRCLEITEM_H

#include "rectitem.h"
#include "plcconfig.h"

class FilledCircleItem : public RectItem
{
    Q_OBJECT
protected:
    ColorValue onColor;
    ColorValue offColor;
    static ColorValue lastOnColor;
    static ColorValue lastOffColor;
    static DiscreteVarType lastLinkType;
    bool state = true;
    DiscreteVarType linkType;
    int linkIndex;
public:
    FilledCircleItem(qreal _width, qreal _height, QObject *parent=nullptr);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    RectItem* clone();
    void updateProperty(ElProperty prop);
};

#endif // FILLEDCIRCLEITEM_H
