#include "rectitem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QDebug>
#include <QJsonArray>

ColorValue RectItem::lastBorderColor = {0x00,0x00,0x00};

QRectF RectItem::getPointRect(MovePoint pointType) const
{
    switch(pointType) {
        case MovePoint::TopLeft:return QRectF(-indent,-indent,indent*2,indent*2);
        case MovePoint::TopRight:return QRectF(width-indent,-indent,indent*2,indent*2);
        case MovePoint::BottomLeft:return QRectF(-indent,height-indent,indent*2,indent*2);
        case MovePoint::BottomRight:return QRectF(width-indent,height-indent,indent*2,indent*2);
        case MovePoint::LeftMiddle:return QRectF(-indent,height/2-indent,indent*2,indent*2);
        case MovePoint::RightMiddle:return QRectF(width-indent,height/2-indent,indent*2,indent*2);
        case MovePoint::TopMiddle:return QRectF(width/2-indent,-indent,indent*2,indent*2);
        case MovePoint::BottomMiddle:return QRectF(width/2-indent,height-indent,indent*2,indent*2);
        case MovePoint::All:return QRectF();
    }
    return QRectF();
}

QRectF RectItem::getMousePointRect(MovePoint pointType) const
{
    switch(pointType) {
        case MovePoint::TopLeft:return QRectF(-indent*2,-indent*2,indent*4,indent*4);
        case MovePoint::TopRight:return QRectF(width-indent*2,-indent*2,indent*4,indent*4);
        case MovePoint::BottomLeft:return QRectF(-indent*2,height-indent*2,indent*4,indent*4);
        case MovePoint::BottomRight:return QRectF(width-indent*2,height-indent*2,indent*4,indent*4);
        case MovePoint::LeftMiddle:return QRectF(-indent*2,height/2-indent*2,indent*4,indent*4);
        case MovePoint::RightMiddle:return QRectF(width-indent*2,height/2-indent*2,indent*4,indent*4);
        case MovePoint::TopMiddle:return QRectF(width/2-indent*2,-indent*2,indent*4,indent*4);
        case MovePoint::BottomMiddle:return QRectF(width/2-indent*2,height-indent*2,indent*4,indent*4);
        case MovePoint::All:return QRectF();
    }
    return QRectF();
}

QRectF RectItem::internalRect() const
{
    return QRectF(0,0,width,height);
}

void RectItem::drawBorder(QPainter *painter, bool zeroWidth)
{
    QBrush br = painter->brush();
    if(selectionMode==SelectionMode::Single) {
        painter->setOpacity(0.6);
        painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),1,Qt::DashLine));
    }else if(selectionMode==SelectionMode::Multi){
        painter->setOpacity(0.6);
        painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),1,Qt::DashLine));
    }else if(lineWidth==0) {
        if(zeroWidth) painter->setOpacity(0.3);
        else painter->setOpacity(0);
        painter->setPen(QPen(QBrush(Qt::blue),1,Qt::DotLine));
    }else return;
    painter->drawRect(QRectF(0,0,width,height));
    if(selectionMode==SelectionMode::Single) {
        painter->setOpacity(0.8);
        painter->setPen(QPen(QBrush(Qt::blue),1));
        painter->fillRect(getPointRect(MovePoint::TopLeft),QBrush(Qt::white));
        painter->drawRect(getPointRect(MovePoint::TopLeft));
        painter->fillRect(getPointRect(MovePoint::TopRight),QBrush(Qt::white));
        painter->drawRect(getPointRect(MovePoint::TopRight));
        painter->fillRect(getPointRect(MovePoint::BottomLeft),QBrush(Qt::white));
        painter->drawRect(getPointRect(MovePoint::BottomLeft));
        painter->fillRect(getPointRect(MovePoint::BottomRight),QBrush(Qt::white));
        painter->drawRect(getPointRect(MovePoint::BottomRight));
        if(chMode==ChangeMode::WidthAndHeight) {
            painter->fillRect(getPointRect(MovePoint::LeftMiddle),QBrush(Qt::white));
            painter->drawRect(getPointRect(MovePoint::LeftMiddle));
            painter->fillRect(getPointRect(MovePoint::RightMiddle),QBrush(Qt::white));
            painter->drawRect(getPointRect(MovePoint::RightMiddle));
            painter->fillRect(getPointRect(MovePoint::TopMiddle),QBrush(Qt::white));
            painter->drawRect(getPointRect(MovePoint::TopMiddle));
            painter->fillRect(getPointRect(MovePoint::BottomMiddle),QBrush(Qt::white));
            painter->drawRect(getPointRect(MovePoint::BottomMiddle));
        }
    }
    painter->setBrush(br);
}

qreal RectItem::checkLeftDXMove(QPointF point, qreal dx)
{
    qreal result = 0;
    if(dx<0) {if(mapFromScene(point).x()<=0) result=dx;}
    else if(width-dx>=indent*3+dist) {if(mapFromScene(point).x()>0) result=dx;}
    return result;
}

qreal RectItem::checkRightDXMove(QPointF point, qreal dx)
{
    qreal result = 0;
    if(dx>=0) {if(mapFromScene(point).x()>=width) result=dx;}
    else if(width-abs(dx)>=indent*3+dist) {if(mapFromScene(point).x()<width) result=dx;}
    return result;
}

qreal RectItem::checkTopDYMove(QPointF point, qreal dy)
{
    qreal result = 0;
    if(dy<0) {if(mapFromScene(point).y()<=0) result=dy;}
    else if(height-dy>=indent*3+dist) {if(mapFromScene(point).y()>0) result=dy;}
    return result;
}

qreal RectItem::checkBottomDYMove(QPointF point, qreal dy)
{
    qreal result = 0;
    if(dy>=0) {if(mapFromScene(point).y()>=height) result=dy;}
    else if(height-abs(dy)>=indent*3+dist) {if(mapFromScene(point).y()<=height) result=dy;}
    return result;
}

RectGeometry RectItem::changeGeometryFromPoint(RectGeometry rect, qreal dx, qreal dy, MovePoint pointType)
{
    if(chMode==ChangeMode::WidthAndHeight) {
        switch(pointType) {
            case MovePoint::BottomLeft:
                rect.width-=dx;
                rect.height+=dy;
                rect.x+=dx;
                break;
            case MovePoint::BottomMiddle:
                rect.height+=dy;
                break;
            case MovePoint::BottomRight:
                rect.width+=dx;
                rect.height+=dy;
                break;
            case MovePoint::LeftMiddle:
                rect.width-=dx;
                rect.x+=dx;
                break;
            case MovePoint::TopLeft:
                rect.width-=dx;
                rect.height-=dy;
                rect.x+=dx;
                rect.y+=dy;
                break;
            case MovePoint::TopMiddle:
                rect.height-=dy;
                rect.y+=dy;
                break;
            case MovePoint::TopRight:
                rect.width+=dx;
                rect.height-=dy;
                rect.y+=dy;
                break;
            case MovePoint::RightMiddle:
                rect.width+=dx;
                break;
            case MovePoint::All:
                rect.x+=dx;
                rect.y+=dy;
                break;
        }
    }else if(chMode==ChangeMode::Proportional) {
        qreal coeff = 1;
        switch(pointType) {
            case MovePoint::BottomLeft:
                if(abs(dx)>abs(dy)) {
                    coeff = (rect.width-dx)/startWidth;
                    dx = rect.width - startWidth*coeff;
                    dy = startHeight*coeff - rect.height;
                    rect.width = round(coeff*startWidth);
                    rect.height = round(coeff*startHeight);
                    rect.x+=round(dx);
                }else {
                    coeff = (rect.height+dy)/startHeight;
                    dx = round(rect.width - startWidth*coeff);
                    dy = round(startHeight*coeff - rect.height);
                    rect.width = round(coeff*startWidth);
                    rect.height = round(coeff*startHeight);
                    rect.x+=round(dx);
                }
                break;
            case MovePoint::BottomRight:
                if(abs(dx)>abs(dy)) {
                    coeff = (rect.width+dx)/startWidth;
                    dx = startWidth*coeff - rect.width;
                    dy = startHeight*coeff - rect.height;
                    rect.width = round(coeff*startWidth);
                    rect.height = round(coeff*startHeight);
                }else {
                    coeff = (rect.height+dy)/startHeight;
                    dx = round(startWidth*coeff - rect.width);
                    dy = round(startHeight*coeff - rect.height);
                    rect.width = round(coeff*startWidth);
                    rect.height = round(coeff*startHeight);
                }
                break;
            case MovePoint::TopLeft:
                if(abs(dx)>abs(dy)) {
                    coeff = (rect.width-dx)/startWidth;
                    dx = rect.width - startWidth*coeff;
                    dy = rect.height - startHeight*coeff;
                    rect.width = round(coeff*startWidth);
                    rect.height = round(coeff*startHeight);
                    rect.x+=round(dx);
                    rect.y+=round(dy);
                }else {
                    coeff = (rect.height-dy)/startHeight;
                    dx = round(rect.width - startWidth*coeff);
                    dy = round(rect.height - startHeight*coeff);
                    rect.width = round(coeff*startWidth);
                    rect.height = round(coeff*startHeight);
                    rect.x+=round(dx);
                    rect.y+=round(dy);
                }
                break;
            case MovePoint::TopRight:
                if(abs(dx)>abs(dy)) {
                    coeff = (rect.width+dx)/startWidth;
                    dx = startWidth*coeff - rect.width;
                    dy = rect.height - startHeight*coeff;
                    rect.width = round(coeff*startWidth);
                    rect.height = round(coeff*startHeight);
                    rect.y+=round(dy);
                }else {
                    coeff = (rect.height-dy)/startHeight;
                    dx = round(rect.width + startWidth*coeff);
                    dy = round(rect.height - startHeight*coeff);
                    rect.width = round(coeff*startWidth);
                    rect.height = round(coeff*startHeight);
                    rect.y+=round(dy);
                }
                break;
            case MovePoint::All:
                rect.x+=dx;
                rect.y+=dy;
                break;
            default:
                break;
        }
    }else if(chMode==ChangeMode::NoChange) {
        rect.x+=dx;
        rect.y+=dy;
    }
    return rect;
}

RectItem::RectItem(qreal width, qreal height, QObject *parent):
    QObject(parent),width(width),height(height),startWidth(width), startHeight(height)
{
    setAcceptHoverEvents(true);
    borderColor = lastBorderColor;
    ElProperty pr("type",ElProperty::Type::STRING_T);
    pr.setValue(QString("rect"));
    properties.push_back(pr);

    pr = ElProperty("line_color",ElProperty::Type::STRING_T);
    QColor col(borderColor.r,borderColor.g,borderColor.b);
    QString colStr = col.name(QColor::HexRgb);
    colStr = colStr.mid(1,6);
    pr.setValue(colStr);
    properties.push_back(pr);

}

QRectF RectItem::boundingRect() const
{
    return QRectF(-indent*2-lineWidth,-indent*2-lineWidth,width+indent*4+2*lineWidth,height+indent*4+2*lineWidth);
}

void RectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selectionMode==SelectionMode::None) painter->setOpacity(1.0);
    else painter->setOpacity(0.8);
    if(lineWidth) {
        painter->setPen(QPen(QBrush(QColor(borderColor.r,borderColor.g,borderColor.b)),lineWidth));
        painter->drawRect(QRectF(0,0,width,height));
    }

    drawBorder(painter);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void RectItem::setSelection(SelectionMode value)
{
    if(value!=selectionMode) {
        prepareGeometryChange();
        selectionMode = value;
        update(boundingRect());
    }

}

void RectItem::updateCaughtPos(QPointF point, qreal dx, qreal dy, MovePoint pointType)
{
    qreal dx_offset = 0;
    qreal dy_offset = 0;
    RectGeometry rect;
    rect.x = static_cast<int>((pos()).x());
    rect.y = static_cast<int>((pos()).y());
    rect.height = height;
    rect.width = width;
    if(pointType==MovePoint::TopLeft) {
        prepareGeometryChange();
        dx_offset = checkLeftDXMove(point,dx);
        dy_offset = checkTopDYMove(point,dy);
    }else if(pointType==MovePoint::TopRight) {
        prepareGeometryChange();
        dx_offset = checkRightDXMove(point,dx);
        dy_offset = checkTopDYMove(point,dy);
    }else if(pointType==MovePoint::BottomRight) {
        prepareGeometryChange();
        dx_offset = checkRightDXMove(point,dx);
        dy_offset = checkBottomDYMove(point,dy);
    }else if(pointType==MovePoint::BottomLeft) {
        prepareGeometryChange();
        dx_offset = checkLeftDXMove(point,dx);
        dy_offset = checkBottomDYMove(point,dy);
    }else if(pointType==MovePoint::LeftMiddle) {
        prepareGeometryChange();
        dx_offset = checkLeftDXMove(point,dx);
    }else if(pointType==MovePoint::RightMiddle) {
        prepareGeometryChange();
        dx_offset = checkRightDXMove(point,dx);
    }else if(pointType==MovePoint::TopMiddle) {
        prepareGeometryChange();
        dy_offset = checkTopDYMove(point,dy);
    }else if(pointType==MovePoint::BottomMiddle) {
        prepareGeometryChange();
        dy_offset = checkBottomDYMove(point,dy);
    }else if(pointType==MovePoint::All) {
        dx_offset = dx;
        dy_offset = dy;
    }
    rect = changeGeometryFromPoint(rect,dx_offset,dy_offset,pointType);
    setPos(QPointF(rect.x,rect.y));
    width = rect.width;
    height = rect.height;
    int x = static_cast<int>((pos()).x());
    int y = static_cast<int>((pos()).y());
    emit changeRect(x,y,static_cast<int>(width),static_cast<int>(height), lineWidth);
    update();
}

RectItem *RectItem::clone()
{
    RectItem* res = new RectItem(width,height);
    res->lineWidth = lineWidth;
    res->setX(getX());
    res->setY(getY());
    for(const auto &pr:properties) res->updateProperty(pr);
    return res;
}

void RectItem::updateProperty(ElProperty prop)
{    
    if(prop.getName()=="line_color") {
        //
        if(prop.getType()==ElProperty::Type::STRING_T) {
            auto colVal = prop.getValue();
            if(auto val = std::get_if<QString>(&colVal)) {
                bool res = false;
                long col = val->toLong(&res,16);
                if(res) {
                    int r = (col>>16)&0xFF;
                    int g = (col>>8)&0xFF;
                    int b = col&0xFF;
                    borderColor = {r,g,b};
                    lastBorderColor = borderColor;
                    update();
                    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="line_color";});
                    if(it!=properties.end()) {
                        it->setValue(*val);
                    }
                }
            }
        }
    }
}

void RectItem::updateGeometry(int x, int y, int w, int h)
{
    prepareGeometryChange();
    setPos(x,y);
    width = w;
    height = h;
    update();
}

void RectItem::updateWidth(int value)
{
    prepareGeometryChange();
    if(value<indent*3+dist) width = indent*3+dist;
    else width = value;
    update();
}

void RectItem::updateHeight(int value)
{
    prepareGeometryChange();
    if(value<indent*3+dist) height = indent*3+dist;
    else height = value;
    update();
}

void RectItem::updateLineWidth(int value)
{
    prepareGeometryChange();
    lineWidth=value;
    update();
    auto it = std::find_if(properties.begin(),properties.end(),[](const ElProperty &pr){return pr.getName()=="line_width";});
    if(it!=properties.end()) it->setValue(lineWidth);
}

int RectItem::getLineWidth() const
{
    return lineWidth;
}

void RectItem::write(QJsonObject &json)
{
    QJsonArray props;
    for(const ElProperty &pr:properties) {
        QJsonObject property;
        if(pr.getName()=="type") {
            QString value = "";
            auto var = pr.getValue();
            if(auto val = std::get_if<QString>(&var)) value = *val;
            json["item"] = value;
            continue;
        }
        property["name"] = pr.getName();
        if(pr.getType()==ElProperty::Type::INT_T) {
            property["type"]="int";
            int value = 0;
            auto var = pr.getValue();
            if(auto val = std::get_if<int>(&var)) value = *val;
            property["value"] = value;
        }else if(pr.getType()==ElProperty::Type::BOOL_T) {
            property["type"]="bool";
            bool value = false;
            auto var = pr.getValue();
            if(auto val = std::get_if<bool>(&var)) value = *val;
            property["value"] = value;
        }else if(pr.getType()==ElProperty::Type::STRING_T) {
            property["type"]="string";
            QString value = "";
            auto var = pr.getValue();
            if(auto val = std::get_if<QString>(&var)) value = *val;
            property["value"] = value;
        }else if(pr.getType()==ElProperty::Type::DOUBLE_T) {
            property["type"]="double";
            double value = 0;
            auto var = pr.getValue();
            if(auto val = std::get_if<double>(&var)) value = *val;
            property["value"] = value;
        }
        props.append(property);
    }
    json["properties"] = props;
    json["x"] = static_cast<int>(pos().x());
    json["y"] = static_cast<int>(pos().y());
    json["width"] = static_cast<int>(width);
    json["height"] = static_cast<int>(height);
    json["line"] = lineWidth;
    json["z"] = static_cast<int>(zValue());
}

void RectItem::read(const QJsonObject &json)
{
    Q_UNUSED(json)
}

void RectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    bool caughtFlag = false;
    if((selectionMode==SelectionMode::Single) && (chMode!=ChangeMode::NoChange)) {
        if(getMousePointRect(MovePoint::TopLeft).contains(event->pos())) {
            caughtFlag = true;
            setCursor(QCursor(Qt::SizeFDiagCursor));
        }
        else if(getMousePointRect(MovePoint::BottomRight).contains(event->pos())) {
            caughtFlag = true;
            setCursor(QCursor(Qt::SizeFDiagCursor));
        }
        else if(getMousePointRect(MovePoint::TopRight).contains(event->pos())) {
            caughtFlag = true;
            setCursor(QCursor(Qt::SizeBDiagCursor));
        }
        else if(getMousePointRect(MovePoint::BottomLeft).contains(event->pos())) {
            caughtFlag = true;
            setCursor(QCursor(Qt::SizeBDiagCursor));
        }
        if(chMode==ChangeMode::WidthAndHeight) {
            if(getMousePointRect(MovePoint::TopMiddle).contains(event->pos())) {
                caughtFlag = true;
                setCursor(QCursor(Qt::SizeVerCursor));
            }
            else if(getMousePointRect(MovePoint::BottomMiddle).contains(event->pos())) {
                caughtFlag = true;
                setCursor(QCursor(Qt::SizeVerCursor));
            }
            else if(getMousePointRect(MovePoint::LeftMiddle).contains(event->pos())) {
                caughtFlag = true;
                setCursor(QCursor(Qt::SizeHorCursor));
            }
            else if(getMousePointRect(MovePoint::RightMiddle).contains(event->pos())) {
                caughtFlag = true;
                setCursor(QCursor(Qt::SizeHorCursor));
            }
        }
    }
    if(!caughtFlag) setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}

void RectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverLeaveEvent(event);
}

void RectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    bool caughtFlag = false;
    if(event->button() & Qt::LeftButton) {
        QPointF point = event->pos();
        if(selectionMode==SelectionMode::Single) {
            if(chMode!=ChangeMode::NoChange) {
                if(getMousePointRect(MovePoint::TopLeft).contains(point)) {
                    caughtFlag = true;
                    emit setItemCaught(this, MovePoint::TopLeft);
                }else if(getMousePointRect(MovePoint::BottomRight).contains(point)) {
                    caughtFlag = true;
                    emit setItemCaught(this, MovePoint::BottomRight);
                }
                else if(getMousePointRect(MovePoint::TopRight).contains(point)) {
                    caughtFlag = true;
                    emit setItemCaught(this, MovePoint::TopRight);
                }
                else if(getMousePointRect(MovePoint::BottomLeft).contains(point)) {
                    caughtFlag = true;
                    emit setItemCaught(this, MovePoint::BottomLeft);
                }
                if(chMode==ChangeMode::WidthAndHeight) {
                    if(getMousePointRect(MovePoint::TopMiddle).contains(point)) {
                        caughtFlag = true;
                        emit setItemCaught(this, MovePoint::TopMiddle);
                    }else if(getMousePointRect(MovePoint::BottomMiddle).contains(point)) {
                        caughtFlag = true;
                        emit setItemCaught(this, MovePoint::BottomMiddle);
                    }
                    else if(getMousePointRect(MovePoint::LeftMiddle).contains(point)) {
                        caughtFlag = true;
                        emit setItemCaught(this, MovePoint::LeftMiddle);
                    }
                    else if(getMousePointRect(MovePoint::RightMiddle).contains(point)) {
                        caughtFlag = true;
                        emit setItemCaught(this, MovePoint::RightMiddle);
                    }
                }
            }
            if((!caughtFlag)&&(internalRect().contains(point))) emit setItemCaught(this,MovePoint::All);
        }
    }
}
