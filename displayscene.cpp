#include "displayscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include "Elements/Widgets/lampitem.h"
#include <QApplication>
#include <QTimer>

void DisplayScene::setRectItemCaught(RectItem *item, MovePoint moveType)
{
    caughtRect = item;
    caughtPointType = moveType;
}

void DisplayScene::setInsertElement(QGraphicsItem *item)
{
    if(insertItem) {
        removeItem(insertItem);
        delete insertItem;
    }
    insertItem = item;
    insertItem->setVisible(false);
    addItem(insertItem);
    deselectAllItems();
}

void DisplayScene::alignLeft()
{
    if(selItems.size()) {
        int x = selItems.at(0)->getX();
        for(const auto& item:selItems) if(item->getX()<x) x = item->getX();
        for(auto &item:selItems) item->setPos(x,item->getY());
    }
}

void DisplayScene::alignRight()
{
    if(selItems.size()) {
        int x = selItems.at(0)->getX() + selItems.at(0)->getWidth();
        for(const auto& item:selItems) if(item->getX()+item->getWidth() > x) x = item->getX() + item->getWidth();
        for(auto &item:selItems) item->setPos(x-item->getWidth(),item->getY());
    }
}

void DisplayScene::alignTop()
{
    if(selItems.size()) {
        int y = selItems.at(0)->getY();
        for(const auto& item:selItems) if(item->getY()<y) y = item->getY();
        for(auto &item:selItems) item->setPos(item->getX(),y);
    }
}

void DisplayScene::alignBottom()
{
    if(selItems.size()) {
        int y = selItems.at(0)->getY() +selItems.at(0)->getHeight();
        for(const auto& item:selItems) if(item->getY() + item->getHeight() > y) y = item->getY() + item->getHeight();
        for(auto &item:selItems) item->setPos(item->getX(),y-item->getHeight());
    }
}

void DisplayScene::alignMinWidth()
{
    if(selItems.size()) {
        int w = selItems.at(0)->getWidth();
        for(const auto& item:selItems) {
            if(item->getWidth()<w) w = item->getWidth();
        }
        for(auto &item:selItems) item->updateGeometry(item->getX(),item->getY(),w,item->getHeight());
    }
}

void DisplayScene::alignMinHeight()
{
    if(selItems.size()) {
        int h = selItems.at(0)->getHeight();
        for(const auto& item:selItems) {
            if(item->getHeight()<h) h = item->getHeight();
        }
        for(auto &item:selItems) item->updateGeometry(item->getX(),item->getY(),item->getWidth(),h);
    }
}

void DisplayScene::setSquare()
{
    for(auto& item:selItems) {
        int distance = item->getHeight();
        int w = item->getWidth();
        if(w<distance) distance = w;
        item->updateGeometry(item->getX(),item->getY(),distance,distance);
    }
}

void DisplayScene::setHorizontalStep(int value)
{
    std::vector<RectItem*> items = selItems;
    if(items.size()>=2) {
        std::sort(items.begin(),items.end(),[](RectItem *first, RectItem *second){return first->getX()<second->getX();});
        int start_pos = items.at(0)->getX();
        int addiction = 0;
        for(auto item: items) {
            item->setX(start_pos+addiction);
            addiction+=value;
        }
    }

}

void DisplayScene::setVerticalStep(int value)
{
    std::vector<RectItem*> items = selItems;
    if(selItems.size()>=2) {
        std::sort(items.begin(),items.end(),[](RectItem *first, RectItem *second){return first->getY()<second->getY();});
        int start_pos = items.at(0)->getY();
        int addiction = 0;
        for(auto item: items) {
            item->setY(start_pos+addiction);
            addiction+=value;
        }
    }
}

void DisplayScene::turnOnAllLamps()
{
    for(auto item: items()) {
        LampItem *lamp = dynamic_cast<LampItem*>(item);
        if(lamp) {
            ElProperty pr("lamp_state",ElProperty::Type::BOOL_T);
            pr.setValue(true);
            lamp->updateProperty(pr);
        }
    }
    if(selItems.size()==1) {
        RectItem *item = selItems.at(0);
        emit setProperties(item->getProperties());
        emit updateRect(item->getX(),item->getY(),item->getWidth(),item->getHeight(),item->getLineWidth());
    }else emit clearProperties();
}

void DisplayScene::turnOffAllLamps()
{
    for(auto item: items()) {
        LampItem *lamp = dynamic_cast<LampItem*>(item);
        if(lamp) {
            ElProperty pr("lamp_state",ElProperty::Type::BOOL_T);
            pr.setValue(false);
            lamp->updateProperty(pr);
        }
    }
    if(selItems.size()==1) {
        RectItem *item = selItems.at(0);
        emit setProperties(item->getProperties());
        emit updateRect(item->getX(),item->getY(),item->getWidth(),item->getHeight(),item->getLineWidth());
    }else emit clearProperties();
}

void DisplayScene::alignMaxWidth()
{
    if(selItems.size()) {
        int w = selItems.at(0)->getWidth();
        for(const auto& item:selItems) {
            if(item->getWidth()>w) w = item->getWidth();
        }
        for(auto &item:selItems) item->updateGeometry(item->getX(),item->getY(),w,item->getHeight());
    }
}

void DisplayScene::alignMaxHeight()
{
    if(selItems.size()) {
        int h = selItems.at(0)->getHeight();
        for(const auto& item:selItems) {
            if(item->getHeight()>h) h = item->getHeight();
        }
        for(auto &item:selItems) item->updateGeometry(item->getX(),item->getY(),item->getWidth(),h);
    }
}

void DisplayScene::drawInsertElement(qreal x, qreal y)
{
    if(insertItem) {
        insertItem->setVisible(true);
        insertItem->setPos(x,y);
        RectItem *rect = dynamic_cast<RectItem*>(insertItem);
        if(rect) {
            emit updateRect(rect->getX(),rect->getY(),rect->getWidth(),rect->getHeight(), rect->getLineWidth());
        }
    }
}

void DisplayScene::deselectAllItems()
{
    auto grItems = items();
    for(auto *grItem: grItems) {
        auto rectItem = dynamic_cast<RectItem*>(grItem);
        if(rectItem) {
            rectItem->setSelection(SelectionMode::None);
        }
    }
    selItems.clear();

}

void DisplayScene::removeInsertItem()
{
    if(insertItem) {
        removeItem(insertItem);
        delete insertItem;
        insertItem = nullptr;
    }
}

void DisplayScene::update_x(int value)
{
    if(selItems.size()==1) {
        selItems.at(0)->setX(value);
    }
}

void DisplayScene::update_y(int value)
{
    if(selItems.size()==1) {
        selItems.at(0)->setY(value);
    }
}

void DisplayScene::update_width(int value)
{
    if(selItems.size()==1) {
        RectItem *item = dynamic_cast<RectItem*>(selItems.at(0));
        if(value>0 && item && item->getChangeMode()!=ChangeMode::NoChange) {
            item->updateWidth(value);
            if(item->getChangeMode()==ChangeMode::Proportional) {
                item->updateHeight(value);
            }
        }
    }
}

void DisplayScene::update_height(int value)
{
    if(selItems.size()==1) {
        RectItem *item = dynamic_cast<RectItem*>(selItems.at(0));
        if(value>0 && item && item->getChangeMode()!=ChangeMode::NoChange) {
            item->updateHeight(value);
            if(item->getChangeMode()==ChangeMode::Proportional) {
                item->updateWidth(value);
            }
        }
    }
}

void DisplayScene::update_line_width(int value)
{
    if(selItems.size()==1) {
        RectItem *item = dynamic_cast<RectItem*>(selItems.at(0));
        if(value>=0 && item) item->updateLineWidth(value);
    }
}

void DisplayScene::onTop()
{
    if(selItems.size()==1) {
        RectItem *item = dynamic_cast<RectItem*>(selItems.at(0));
        if(item) {
            auto polygon = item->mapToScene(item->boundingRect());
            QList<QGraphicsItem*> grItems = items(polygon,Qt::IntersectsItemBoundingRect);
            grItems.removeOne(item);
            std::vector<RectItem*> rItems;
            for(QGraphicsItem *grItem:grItems) {
                RectItem* rect = dynamic_cast<RectItem*>(grItem);
                if(rect) {rItems.push_back(rect);}
            }

            if(rItems.size()) {
                qreal z = rItems[0]->zValue();
                for(RectItem *rect: rItems) {
                    if(rect->zValue()>z) z = rect->zValue();
                }
                item->setZValue(z+1);
                maxZValue = z+1;
            }
            update(item->boundingRect());
        }
    }
}

void DisplayScene::underBottom()
{
    if(selItems.size()==1) {
        RectItem *item = dynamic_cast<RectItem*>(selItems.at(0));
        if(item) {
            auto polygon = item->mapToScene(item->boundingRect());
            QList<QGraphicsItem*> grItems = items(polygon,Qt::IntersectsItemBoundingRect);
            grItems.removeOne(item);
            std::vector<RectItem*> rItems;
            for(QGraphicsItem *grItem:grItems) {
                RectItem* rect = dynamic_cast<RectItem*>(grItem);
                if(rect) {rItems.push_back(rect);}
            }

            if(rItems.size()) {
                qreal z = rItems[0]->zValue();
                for(RectItem *rect: rItems) {
                    if(rect->zValue()<z) z = rect->zValue();
                }
                item->setZValue(z-1);
            }
            update(item->boundingRect());
        }
    }
}

qreal DisplayScene::getMaxZValue() const
{
    return maxZValue;
}

void DisplayScene::updateProperty(const ElProperty &prop)
{
    if(selItems.size()==1) {
        RectItem *item = dynamic_cast<RectItem*>(selItems.at(0));
        item->updateProperty(prop);
    }
}

DisplayScene::DisplayScene(QObject *parent): QGraphicsScene(parent)
{

}

void DisplayScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    static qreal prev_x = 0;
    static qreal prev_y = 0;
    qreal x = event->scenePos().x();
    qreal y = event->scenePos().y();
    if(moveEnable && caughtRect!=nullptr && !(event->modifiers() & Qt::ControlModifier)) {
        if(caughtPointType==MovePoint::All) {
            for(auto item:selItems) {
                item->updateCaughtPos(event->scenePos(), x-prev_x,y-prev_y,caughtPointType);
            }
        }else {
            caughtRect->updateCaughtPos(event->scenePos(), x-prev_x,y-prev_y,caughtPointType);
        }
    }
    prev_x = cur_x = x;
    prev_y = cur_y = y;
    drawInsertElement(event->scenePos().x(),event->scenePos().y());
    QGraphicsScene::mouseMoveEvent(event);
}

void DisplayScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    caughtRect=nullptr;
    moveEnable = false;
    QGraphicsScene::mouseReleaseEvent(event);
}

void DisplayScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() & Qt::LeftButton) {

        emit clearProperties();

        if(insertItem) {
            RectItem *rect = dynamic_cast<RectItem*>(insertItem);
            if(rect) {
                rect->setSelection(SelectionMode::Single);
                if(auto it=std::find(selItems.begin(),selItems.end(),rect);it==selItems.end()) {
                    selItems.push_back(rect);
                }
                connect(rect,&RectItem::setItemCaught,this,&DisplayScene::setRectItemCaught);
                emit setProperties(rect->getProperties());
                emit updateRect(rect->getX(),rect->getY(),rect->getWidth(),rect->getHeight(), rect->getLineWidth());
                insertItem = nullptr;
            }else {
                removeItem(insertItem);
                delete insertItem;
                insertItem = nullptr;
            }
        }else {
            auto item = itemAt(event->scenePos(),QTransform());
            if(item) {
                auto rectItem = dynamic_cast<RectItem*>(item);
                if(rectItem) {
                    QTimer::singleShot(50, [this](){moveEnable=true;});
                    if(!(event->modifiers() & Qt::ControlModifier)) {
                        if(auto it=std::find(selItems.begin(),selItems.end(),rectItem);it==selItems.end()) {
                            deselectAllItems();
                            selItems.push_back(rectItem);
                        }
                        rectItem->setSelection(SelectionMode::Single);
                    }
                    else {
                        if(auto it=std::find(selItems.begin(),selItems.end(),rectItem);it==selItems.end()) {
                            rectItem->setSelection(SelectionMode::Single);
                            selItems.push_back(rectItem);
                        }else {
                            rectItem->setSelection(SelectionMode::None);
                            selItems.erase(it);
                        }
                    }
                }else deselectAllItems();
            }else deselectAllItems();
        }

        if(selItems.size()==1) {
            RectItem *item = selItems.at(0);
            emit setProperties(item->getProperties());
            emit updateRect(item->getX(),item->getY(),item->getWidth(),item->getHeight(),item->getLineWidth());
        }else emit clearProperties();
    }
    QGraphicsScene::mousePressEvent(event);
}

void DisplayScene::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Delete:
        {

            while(selItems.begin()!=selItems.end()) {
                RectItem* item = selItems.at(0);
                selItems.erase(selItems.cbegin());
                removeItem(item);
                delete item;
            }
            emit clearProperties();
        }
        break;
    case Qt::Key_Escape:
        if(insertItem) {
            removeItem(insertItem);
            delete insertItem;
            insertItem = nullptr;
            emit clearProperties();
        }
        break;
    case Qt::Key_C:
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            for(auto rect:bufItems) delete rect;
            bufItems.clear();
            for(RectItem* rect:selItems) {
                RectItem *clRect = rect->clone();
                bufItems.push_back(clRect);
                qDebug() << "add";
            }
            deselectAllItems();
        }
        break;
    case Qt::Key_X:
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            for(auto rect:bufItems) delete rect;
            bufItems.clear();
            for(RectItem* rect:selItems) {
                RectItem *clRect = rect->clone();
                bufItems.push_back(clRect);
                qDebug() << "add";
            }
            while(selItems.begin()!=selItems.end()) {
                RectItem* item = selItems.at(0);
                selItems.erase(selItems.cbegin());
                removeItem(item);
                delete item;
            }
            emit clearProperties();
            deselectAllItems();
        }
        break;
    case Qt::Key_V:
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            if(bufItems.size()) {
                qDebug() << "Paste";
                deselectAllItems();
                std::sort(bufItems.begin(),bufItems.end(),[](RectItem *first, RectItem *second){return first->getY()<second->getY();});
                std::stable_sort(bufItems.begin(),bufItems.end(),[](RectItem *first, RectItem *second){return first->getX()<second->getX();});
                qDebug() << "sort finished";
                qreal dx = cur_x - bufItems.at(0)->getX();
                qreal dy = cur_y - bufItems.at(0)->getY();
                for(RectItem *rect:bufItems) {
                    RectItem* clRect = rect->clone();
                    addItem(clRect);
                    clRect->setSelection(SelectionMode::Single);
                    selItems.push_back(clRect);
                    clRect->setX(clRect->getX()+dx);
                    clRect->setY(clRect->getY()+dy);
                    connect(clRect,&RectItem::setItemCaught,this,&DisplayScene::setRectItemCaught);
                    connect(clRect,&RectItem::changeRect,this,&DisplayScene::updateRect);
                }
            }
            if(selItems.size()>1) emit clearProperties();
            else if(selItems.size()==1) emit setProperties(selItems.at(0)->getProperties());
        }
        break;
    }
    emit QGraphicsScene::keyPressEvent(event);
}
