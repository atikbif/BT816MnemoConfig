#include "numberitem.h"
#include <QPainter>

NumberDivider NumberItem::lastDiv = NumberDivider::Div1;
EngFonts NumberItem::lastNumFont = EngFonts::Height20;
int NumberItem::lastVarType = 0;
int NumberItem::lastVarIndex = 1;
QString NumberItem::lastPattern = "000";

NumberItem::NumberItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{
    ElProperty pr("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("number"));
    properties.push_back(pr);

    engFontSize.push_back(16);
    engFontSize.push_back(20);
    engFontSize.push_back(25);
    engFontSize.push_back(28);
    engFontSize.push_back(36);
    engFontSize.push_back(49);

    numFont = lastNumFont;
    div = lastDiv;
    pattern = lastPattern;



    pr = ElProperty("eng_font_index",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(numFont));
    properties.push_back(pr);

    pr = ElProperty("num_div",ElProperty::Type::INT_T);
    pr.setValue(static_cast<int>(div));
    properties.push_back(pr);

    pr = ElProperty("num_pattern",ElProperty::Type::STRING_T);
    pr.setValue(pattern);
    properties.push_back(pr);

    varType = lastVarType;
    varIndex = lastVarIndex;

    pr = ElProperty("num_var_type",ElProperty::Type::INT_T);
    pr.setValue(varType);
    properties.push_back(pr);

    pr = ElProperty("num_var_index",ElProperty::Type::INT_T);
    pr.setValue(varIndex);
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
    QFont font("Times");
    //font.setBold(true);
    font.setPointSize(getEngFontHeight(numFont));
    painter->setFont(font);

    painter->drawText(QRectF(indent,0,width-indent*2,height), pattern);
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
    if(prop.getName()=="eng_font_index") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto tVal = prop.getValue();
            if(auto val = std::get_if<int>(&tVal)) {
                numFont = static_cast<EngFonts>(*val);
                lastNumFont = numFont;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="eng_font_index";});
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
            if(auto val = std::get_if<int>(&tVal)) {
                pattern = *val;
                lastPattern = pattern;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="num_pattern";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="num_var_type") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto vVal = prop.getValue();
            if(auto val = std::get_if<int>(&vVal)) {
                varType = *val;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="num_var_type";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="num_var_index") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto vVal = prop.getValue();
            if(auto val = std::get_if<int>(&vVal)) {
                varIndex = *val;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="num_var_index";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }
}
