#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "rectitem.h"

enum class TextItemAlign {Left, Right, Center};

class TextItem : public RectItem
{
    Q_OBJECT
protected:
    TextItemAlign align = TextItemAlign::Left;
    QString textValue="АБВ";
    int fontSize;
    static int lastFontSize;
public:
    TextItem(qreal _width, qreal _height, QObject *parent=nullptr);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    RectItem* clone() override;
    void updateProperty(ElProperty prop) override;
};

#endif // TEXTITEM_H
