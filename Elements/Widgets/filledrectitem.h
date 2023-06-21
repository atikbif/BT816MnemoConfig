#ifndef FILLEDRECTITEM_H
#define FILLEDRECTITEM_H

#include "rectitem.h"
#include "plcconfig.h"

class FilledRectItem : public RectItem
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
    FilledRectItem(qreal _width, qreal _height, QObject *parent=nullptr);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateProperty(ElProperty prop);
    RectItem* clone();
};

#endif // FILLEDRECTITEM_H
