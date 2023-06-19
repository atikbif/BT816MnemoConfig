#ifndef RECTWIDGET_H
#define RECTWIDGET_H

#include <QGraphicsItem>
#include <QObject>
#include "../Properties/elproperty.h"
#include <vector>
#include <QJsonObject>

enum class MovePoint {TopLeft, TopRight, BottomLeft, BottomRight, LeftMiddle, RightMiddle, TopMiddle, BottomMiddle, All};

enum class SelectionMode {Single, Multi, Insert, None};

enum class ChangeMode {WidthAndHeight, Proportional, NoChange};

struct ColorValue{
    int r = 0;
    int g = 0;
    int b = 0;
};

struct RectGeometry {
    qreal x;
    qreal y;
    qreal width;
    qreal height;
};

class RectItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

protected:
    ChangeMode chMode = ChangeMode::WidthAndHeight;
    const int indent = 5;
    int lineWidth = 0;
    SelectionMode selectionMode = SelectionMode::None;
    qreal width, height;
    std::vector<ElProperty> properties;
    const int dist = 10;
    ColorValue borderColor;
    static ColorValue lastBorderColor;
    qreal startWidth, startHeight;

    QRectF getPointRect(MovePoint pointType) const;
    QRectF getMousePointRect(MovePoint pointType) const;
    QRectF internalRect() const;
    void drawBorder(QPainter *painter, bool zeroWidth = true);

    virtual qreal checkLeftDXMove(QPointF point, qreal dx);
    virtual qreal checkRightDXMove(QPointF point, qreal dx);
    virtual qreal checkTopDYMove(QPointF point, qreal dy);
    virtual qreal checkBottomDYMove(QPointF point, qreal dy);
    virtual RectGeometry changeGeometryFromPoint(RectGeometry rect, qreal dx, qreal dy, MovePoint pointType);

public:
    RectItem(qreal width, qreal height, QObject *parent=nullptr);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setSelection(SelectionMode value);
    SelectionMode getSelection() {return selectionMode;}

    std::vector<ElProperty> getProperties() const {return properties;}

    void updateGeometry(int x, int y, int w, int h);
    void updateWidth(int value);
    void updateHeight(int value);
    void updateLineWidth(int value);
    int getWidth() {return static_cast<int>(width);}
    int getHeight() {return static_cast<int>(height);}
    int getX() {return static_cast<int>((pos()).x());}
    int getY() {return static_cast<int>((pos()).y());}
    int getLineWidth() const;
    ChangeMode getChangeMode() {return chMode;}

    virtual void updateProperty(ElProperty prop);
    virtual void updateCaughtPos(QPointF point, qreal dx, qreal dy, MovePoint pointType);
    virtual RectItem* clone();
    virtual void write(QJsonObject &json);
    virtual void read(const QJsonObject &json);

    // QGraphicsItem interface
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void setItemCaught(RectItem *item, MovePoint moveType);
    void changeRect(int x, int y, int w, int h, int lw);

};

#endif // RECTWIDGET_H
