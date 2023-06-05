#ifndef NUMBERITEM_H
#define NUMBERITEM_H

#include "rectitem.h"

enum class NumberItemAlign {Left, Right, Center};

class NumberItem : public RectItem
{
    Q_OBJECT
    NumberItemAlign align = NumberItemAlign::Center;
    int fontSize;
    static int lastFontSize;

    int digCount;
    int afterPointDigCount;
    static int lastDigCount;
    static int lastAfterPointDigCount;
    int varType;
    int varIndex;
    static int lastVarType;
    static int lastVarIndex;
public:
    NumberItem(qreal _width, qreal _height, QObject *parent=nullptr);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    RectItem* clone() override;
    void updateProperty(ElProperty prop) override;
};

#endif // NUMBERITEM_H
