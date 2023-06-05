#include "scalableimageitem.h"
#include <QPainter>
#include "scalimages.h"

int ScalableImageItem::lastIndex = 0;

ScalableImageItem::ScalableImageItem(qreal _width, qreal _height, QObject *parent):RectItem(_width,_height,parent)
{
    ElProperty pr("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("scal_image"));
    properties.push_back(pr);
    index = lastIndex;

    pr = ElProperty("image_index",ElProperty::Type::INT_T);
    pr.setValue(index);
    properties.push_back(pr);

}

void ScalableImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selectionMode==SelectionMode::None) painter->setOpacity(1.0);
    else painter->setOpacity(0.8);

    painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),lineWidth));
    QPixmap pix;
    pix.load(ScalImages::getImage(index));
    painter->drawPixmap(0,0,static_cast<int>(width),static_cast<int>(height),pix);
    if(lineWidth) painter->drawRect(QRectF(0,0,width,height));

    drawBorder(painter,false);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void ScalableImageItem::updateProperty(ElProperty prop)
{
    RectItem::updateProperty(prop);
    if(prop.getName()=="image_index") {
            if(prop.getType()==ElProperty::Type::INT_T) {
                auto inVal = prop.getValue();
                if(auto val = std::get_if<int>(&inVal)) {
                    index = *val;
                    lastIndex = index;
                    update();
                    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="image_index";});
                    if(it!=properties.end()) {
                        it->setValue(*val);
                    }
                }
            }
        }
}

RectItem *ScalableImageItem::clone()
{
    ScalableImageItem* res = new ScalableImageItem(width,height);
    res->lineWidth = lineWidth;
    res->setX(getX());
    res->setY(getY());
    for(const auto &pr:properties) res->updateProperty(pr);
    return res;
}
