#include "textitem.h"
#include <QPainter>
#include <QFont>
#include <QTextOption>

int TextItem::lastFontSize=18;

TextItem::TextItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{
    fontSize = lastFontSize;
    ElProperty pr("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("text"));
    properties.push_back(pr);

    pr = ElProperty("font_size",ElProperty::Type::INT_T);
    pr.setValue(fontSize);
    properties.push_back(pr);

    pr = ElProperty("text_value",ElProperty::Type::STRING_T);
    pr.setValue(textValue);
    properties.push_back(pr);

    pr = ElProperty("text_align",ElProperty::Type::INT_T);
    switch(align) {
        case TextItemAlign::Left:pr.setValue(0);break;
        case TextItemAlign::Right:pr.setValue(1);break;
        case TextItemAlign::Center:pr.setValue(2);break;
    }
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
    QFont font("Courier New");
    font.setPointSize(fontSize);
    painter->setFont(font);
    QTextOption opt;
    if(align==TextItemAlign::Left) opt.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    else if(align==TextItemAlign::Right) opt.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    else if(align==TextItemAlign::Center) opt.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    painter->drawText(QRectF(indent,0,width-indent*2,height), textValue, opt);
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
    }else if(prop.getName()=="font_size") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto tVal = prop.getValue();
            if(auto val = std::get_if<int>(&tVal)) {
                fontSize = *val;
                lastFontSize = fontSize;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="font_size";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="text_align") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto tVal = prop.getValue();
            if(auto val = std::get_if<int>(&tVal)) {
                if(*val==0) align = TextItemAlign::Left;
                else if(*val==1) align = TextItemAlign::Right;
                else if(*val==2) align = TextItemAlign::Center;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="text_align";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }
}
