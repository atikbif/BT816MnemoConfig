#include "numberitem.h"
#include <QPainter>

NumberDivider NumberItem::lastDiv = NumberDivider::Div1;
CyrFonts NumberItem::lastNumFont = CyrFonts::Height22;
QString NumberItem::lastPattern = "000";
AnalogueVarType NumberItem::lastLinkType = AnalogueVarType::RAW_AI;

NumberItem::NumberItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{
    ElProperty pr("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("number"));
    updateProperty(pr);

    engFontSize.push_back(16);
    engFontSize.push_back(20);
    engFontSize.push_back(25);
    engFontSize.push_back(28);
    engFontSize.push_back(36);
    engFontSize.push_back(49);

    numFont = lastNumFont;
    div = lastDiv;
    pattern = lastPattern;
    linkType = lastLinkType;


    pr = ElProperty("cyr_font_index",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(numFont));
    properties.push_back(pr);

    pr = ElProperty("num_div",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(div));
    properties.push_back(pr);

    pr = ElProperty("num_pattern",ElProperty::Type::STRING_T);
    pr.setValue(pattern);
    properties.push_back(pr);

    pr = ElProperty("link_analogue_type",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(linkType));
    properties.push_back(pr);

    pr = ElProperty("link_analogue_index",ElProperty::Type::INT_T);
    pr.setValue(linkIndex);
    properties.push_back(pr);
}

void NumberItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selectionMode==SelectionMode::None) painter->setOpacity(1.0);
    else painter->setOpacity(0.8);
    if(lineWidth) {
        painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),lineWidth));
        painter->drawRect(QRectF(0,0,width,height));
    }else painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),1));
    QFont font("IBM Plex Mono");
    font.setPixelSize(getCyrFontHeight(numFont));
    painter->setFont(font);

    painter->drawText(QRectF(0,0,width,height), pattern);
    drawBorder(painter);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

RectItem *NumberItem::clone()
{
    NumberItem* res = new NumberItem(width,height);
    res->lineWidth = lineWidth;
    res->setX(getX());
    res->setY(getY());
    for(const auto &pr:properties) res->updateProperty(pr);
    return res;
}

void NumberItem::updateProperty(ElProperty prop)
{
    RectItem::updateProperty(prop);
    if(prop.getName()=="cyr_font_index") {
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
    }else if(prop.getName()=="num_div") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto tVal = prop.getValue();
            if(auto val = std::get_if<int>(&tVal)) {
                div = static_cast<NumberDivider>(*val);
                lastDiv = div;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="num_div";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="num_pattern") {
        if(prop.getType()==ElProperty::Type::STRING_T) {
            auto tVal = prop.getValue();
            if(auto val = std::get_if<QString>(&tVal)) {
                pattern = *val;
                lastPattern = pattern;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="num_pattern";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="link_analogue_type") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto lVal = prop.getValue();
            if(auto val = std::get_if<int>(&lVal)) {
                linkType = static_cast<AnalogueVarType>(*val);
                lastLinkType = linkType;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_analogue_type";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="link_analogue_index") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto lVal = prop.getValue();
            if(auto val = std::get_if<int>(&lVal)) {
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_analogue_index";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }
}
