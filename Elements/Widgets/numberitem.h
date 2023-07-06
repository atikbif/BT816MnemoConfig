#ifndef NUMBERITEM_H
#define NUMBERITEM_H

#include "rectitem.h"
#include "typical_fonts.h"
#include "plcconfig.h"

enum class NumberDivider {Div1,Div10,Div100,Div1000};


class NumberItem : public RectItem
{
    std::vector<int> engFontSize;
    static NumberDivider lastDiv;
    static CyrFonts lastNumFont;
    static QString lastPattern;
    static AnalogueVarType lastLinkType;
    //static int lastVarType;
    //static int lastVarIndex;

    Q_OBJECT
    CyrFonts numFont;
    NumberDivider div;
    AnalogueVarType linkType;
    int linkIndex = 0;
    QString pattern;
public:
    NumberItem(qreal _width, qreal _height, QObject *parent=nullptr);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    RectItem* clone() override;
    void updateProperty(ElProperty prop) override;
};

#endif // NUMBERITEM_H
