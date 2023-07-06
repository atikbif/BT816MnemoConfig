#ifndef LAMPITEM_H
#define LAMPITEM_H

#include "rectitem.h"
#include "plcconfig.h"

class LampItem : public RectItem
{
    Q_OBJECT
    int onIndex;
    int offIndex;
    static int lastOnIndex;
    static int lastOffIndex;
    bool state = true;
    static DiscreteVarType lastLinkType;
    DiscreteVarType linkType;
    int linkIndex = 0;
public:
    LampItem(qreal _width, qreal _height, QObject *parent=nullptr);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    RectItem* clone() override;
    void updateProperty(ElProperty prop) override;
};

#endif // LAMPITEM_H
