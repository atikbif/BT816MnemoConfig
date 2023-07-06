#include "textitem.h"
#include <QPainter>
#include <QFont>
#include <QTextOption>
#include <QDebug>

CyrFonts TextItem::lastNumFont=CyrFonts::Height14;

TextItem::TextItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{
    numFont = lastNumFont;
    ElProperty pr("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("text"));
    updateProperty(pr);

    pr = ElProperty("cyr_font_index",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(numFont));
    properties.push_back(pr);

    pr = ElProperty("text_value",ElProperty::Type::STRING_T);
    pr.setValue(textValue);
    properties.push_back(pr);
}

void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selectionMode==SelectionMode::None) painter->setOpacity(1.0);
    else painter->setOpacity(0.8);
    if(lineWidth) {
        painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),lineWidth));
        painter->drawRect(QRectF(0,0,width,height));
    }else painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),1));

    QFont font("IBM Plex Mono");

    font.setStyleStrategy(QFont::PreferMatch);
    font.setPixelSize(getCyrFontHeight(numFont));
    painter->setFont(font);
    painter->drawText(QRectF(0,0,width,height), textValue);
    drawBorder(painter);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

RectItem *TextItem::clone()
{
    TextItem* res = new TextItem(width,height);
    res->lineWidth = lineWidth;
    res->setX(getX());
    res->setY(getY());
    for(const auto &pr:properties) res->updateProperty(pr);
    return res;
}

void TextItem::updateProperty(ElProperty prop)
{
    RectItem::updateProperty(prop);
    if(prop.getName()=="text_value") {
        if(prop.getType()==ElProperty::Type::STRING_T) {
            auto tVal = prop.getValue();
            if(auto val = std::get_if<QString>(&tVal)) {
                textValue = *val;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="text_value";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="cyr_font_index") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto tVal = prop.getValue();
            if(auto val = std::get_if<int>(&tVal)) {
                numFont = static_cast<CyrFonts>(*val);
                lastNumFont = numFont;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="cyr_font_index";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }
}
