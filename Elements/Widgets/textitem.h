#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "rectitem.h"
#include "typical_fonts.h"

class TextItem : public RectItem
{
    Q_OBJECT
protected:
    QString textValue="АБВ";
    CyrFonts numFont;
    static CyrFonts lastNumFont;
public:
    TextItem(qreal _width, qreal _height, QObject *parent=nullptr);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    RectItem* clone() override;
    void updateProperty(ElProperty prop) override;
};

#endif // TEXTITEM_H
