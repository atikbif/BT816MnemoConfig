#ifndef DISPLAYSCENE_H
#define DISPLAYSCENE_H

#include <QObject>
#include "qgraphicsscene.h"
#include "Elements/Widgets/rectitem.h"
#include <QSharedPointer>

class DisplayScene : public QGraphicsScene
{
    Q_OBJECT
    RectItem *caughtRect = nullptr;
    MovePoint caughtPointType = MovePoint::All;
    QGraphicsItem *insertItem = nullptr;

    std::vector<RectItem*> selItems;
    std::vector<RectItem*> bufItems;

    qreal maxZValue = 0;

    qreal cur_x = 0;
    qreal cur_y = 0;
    bool moveEnable = false;

    void drawInsertElement(qreal x, qreal y);


public:
    DisplayScene(QObject *parent = nullptr);
    void setRectItemCaught(RectItem *item, MovePoint moveType);
    void setInsertElement(QGraphicsItem *item);

    void alignLeft();
    void alignRight();
    void alignTop();
    void alignBottom();
    void alignMaxWidth();
    void alignMaxHeight();
    void alignMinWidth();
    void alignMinHeight();
    void setSquare();
    void setHorizontalStep(int value);
    void setVerticalStep(int value);
    void turnOnAllLamps();
    void turnOffAllLamps();

    void update_x(int value);
    void update_y(int value);
    void update_width(int value);
    void update_height(int value);
    void update_line_width(int value);
    void onTop();
    void underBottom();

    qreal getMaxZValue() const;

    void updateProperty(const ElProperty &prop);
    void deselectAllItems();
    void removeInsertItem();
    // QGraphicsScene interface
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void setProperties(const std::vector<ElProperty> &properties);
    void updateRect(int x, int y, int w, int h, int lw);
    void clearProperties();
};

#endif // DISPLAYSCENE_H
