#include "numberitem.h"
#include <QPainter>

int NumberItem::lastFontSize = 20;
int NumberItem::lastDigCount = 4;
int NumberItem::lastAfterPointDigCount = 1;
int NumberItem::lastVarType = 0;
int NumberItem::lastVarIndex = 1;

NumberItem::NumberItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{
    ElProperty pr("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("number"));
    properties.push_back(pr);

    fontSize = lastFontSize;
    digCount = lastDigCount;
    afterPointDigCount = lastAfterPointDigCount;

    pr = ElProperty("font_size",ElProperty::Type::INT_T);
    pr.setValue(fontSize);
    properties.push_back(pr);

    pr = ElProperty("dig_count",ElProperty::Type::INT_T);
    pr.setValue(digCount);
    properties.push_back(pr);

    pr = ElProperty("after_point",ElProperty::Type::INT_T);
    pr.setValue(afterPointDigCount);
    properties.push_back(pr);

    pr = ElProperty("text_align",ElProperty::Type::INT_T);
    switch(align) {
        case NumberItemAlign::Left:pr.setValue(0);break;
        case NumberItemAlign::Right:pr.setValue(1);break;
        case NumberItemAlign::Center:pr.setValue(2);break;
    }
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
    font.setPointSize(fontSize);
    painter->setFont(font);
    QTextOption opt;
    if(align==NumberItemAlign::Left) opt.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    else if(align==NumberItemAlign::Right) opt.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    else if(align==NumberItemAlign::Center) opt.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QString textValue = "";
    int num = 0;
    if(afterPointDigCount) {
        while(num<afterPointDigCount) {
            textValue = "0" + textValue;
            num++;
        }
        textValue = "." + textValue;
    }
    while(num<digCount) {num++;textValue = "0" + textValue;}
    painter->drawText(QRectF(indent,0,width-indent*2,height), textValue, opt);
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
    if(prop.getName()=="font_size") {
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
                if(*val==0) align = NumberItemAlign::Left;
                else if(*val==1) align = NumberItemAlign::Right;
                else if(*val==2) align = NumberItemAlign::Center;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="text_align";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="dig_count") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto tVal = prop.getValue();
            if(auto val = std::get_if<int>(&tVal)) {
                digCount = *val;
                lastDigCount = digCount;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="dig_count";});
                if(it!=properties.end()) {
                    it->setValue(*val);
                }
            }
        }
    }else if(prop.getName()=="after_point") {
        if(prop.getType()==ElProperty::Type::INT_T) {
            auto tVal = prop.getValue();
            if(auto val = std::get_if<int>(&tVal)) {
                afterPointDigCount = *val;
                lastAfterPointDigCount = afterPointDigCount;
                update();
                auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="after_point";});
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
