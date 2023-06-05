#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "displayscene.h"
#include "Elements/Widgets/rectitem.h"
#include "Elements/Widgets/ellipseitem.h"
#include "Elements/Widgets/filledrectitem.h"
#include "Elements/Widgets/scalableimageitem.h"
#include "Elements/Widgets/filledellipseitem.h"
#include "Elements/Widgets/textitem.h"
#include "Elements/Widgets/lampitem.h"
#include "Elements/Widgets/numberitem.h"
#include <QRectF>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QFileDialog>

void MainWindow::setProperties(const std::vector<ElProperty> &properties)
{
    if(prView) prView->setProperties(properties);
}

void MainWindow::updateRect(int x, int y, int w, int h, int lw)
{
    if(prView) prView->updateRect(x,y,w,h,lw);
}

void MainWindow::clearProperties()
{
    if(prView) prView->clearProperties();
}

void MainWindow::save()
{
    QJsonObject result;
    QJsonArray grItems;
    QList<QGraphicsItem*> items = sc->items(Qt::AscendingOrder);
    for(QGraphicsItem *item:items) {
        RectItem *rect = dynamic_cast<RectItem*>(item);
        if(rect) {
            QJsonObject grItem;
            rect->write(grItem);
            grItems.append(grItem);
        }
    }
    result["key"] = "DISPLAY MTS";
    result["version"] = "1";
    result["items"] = grItems;

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить проект", "",  "Проект пульта *.plt");

    QFile saveFile(fileName);
    if(saveFile.open(QIODevice::WriteOnly)) {
        saveFile.write(QJsonDocument(result).toJson());
        ui->statusbar->showMessage("Файл успешно сохранён", 3000);
    }else ui->statusbar->showMessage("Ошибка записи файла", 3000);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть проект", "", "Проект пульта *.plt");
    if(fileName.isEmpty()) return;
    QFile loadFile(fileName);
    if (loadFile.open(QIODevice::ReadOnly)) {
        sc->deselectAllItems();
        sc->removeInsertItem();
        sc->clear();
        auto *border = sc->addRect(0,0,800,600,QPen(Qt::gray,1));
        border->setZValue(-1000);
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        QJsonObject json = loadDoc.object();

        if(!(json.contains("key") && json["key"]=="DISPLAY MTS")) {
            ui->statusbar->showMessage("Неверный формат файла", 3000);
            return;
        }
        if (json.contains("items") && json["items"].isArray()) {
            QJsonArray itemsArray = json["items"].toArray();
            for(auto itemRef: itemsArray) {
                if(itemRef.isObject()) {
                    QJsonObject itemOb = itemRef.toObject();
                    QString itemName = itemOb["item"].toString();
                    int lineWidth = itemOb["line"].toInt();
                    int x = itemOb["x"].toInt();
                    int y = itemOb["y"].toInt();
                    int width = itemOb["width"].toInt();
                    int height = itemOb["height"].toInt();
                    int z = itemOb["z"].toInt();

                    RectItem *item = nullptr;
                    if(itemName=="filled_rect") item = new FilledRectItem(width,height);
                    else if(itemName=="filled_ellipse") item = new FilledEllipseItem(width,height);
                    else if(itemName=="lamp") item = new LampItem(width,height);
                    else if(itemName=="text") item = new TextItem(width,height);
                    else if(itemName=="number") item = new NumberItem(width,height);
                    else if(itemName=="scal_image") item = new ScalableImageItem(width,height);


                    if(item!=nullptr) {
                        item->setX(x);
                        item->setY(y);
                        item->setZValue(z);
                        item->updateLineWidth(lineWidth);

                        if(itemOb.contains("properties") && itemOb["properties"].isArray()) {
                            QJsonArray itemProperties = itemOb["properties"].toArray();
                            for(auto itemPrRef: itemProperties) {
                                QJsonObject prOb = itemPrRef.toObject();
                                QString prName = prOb["name"].toString();
                                QString prType = prOb["type"].toString();
                                if(prType=="string") {
                                    QString value = prOb["value"].toString();
                                    ElProperty pr(prName,ElProperty::Type::STRING_T);
                                    pr.setValue(value);
                                    item->updateProperty(pr);
                                }else if(prType=="int") {
                                    int value = prOb["value"].toInt();
                                    ElProperty pr(prName,ElProperty::Type::INT_T);
                                    pr.setValue(value);
                                    item->updateProperty(pr);
                                }else if(prType=="bool") {
                                    bool value = prOb["value"].toBool();
                                    ElProperty pr(prName,ElProperty::Type::BOOL_T);
                                    pr.setValue(value);
                                    item->updateProperty(pr);
                                }else if(prType=="double") {
                                    double value = prOb["value"].toDouble();
                                    ElProperty pr(prName,ElProperty::Type::DOUBLE_T);
                                    pr.setValue(value);
                                    item->updateProperty(pr);
                                }
                            }
                        }
                        sc->addItem(item);
                        connect(item,&RectItem::changeRect,sc,&DisplayScene::updateRect);
                        connect(item,&RectItem::setItemCaught,sc,&DisplayScene::setRectItemCaught);
                    }
                }
            }
            ui->statusbar->showMessage("Файл успешно открыт", 3000);
        }else ui->statusbar->showMessage("Неверный формат файла", 3000);
    }else ui->statusbar->showMessage("Ошибка открытия файла", 3000);

}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->resize(800+200,600+200);

    sc = new DisplayScene();
    sc->setSceneRect(0,0,800,600);

    connect(sc,&DisplayScene::updateRect,this,&MainWindow::updateRect);
    connect(sc,&DisplayScene::clearProperties,this,&MainWindow::clearProperties);
    connect(sc,&DisplayScene::setProperties,this,&MainWindow::setProperties);

    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setScene(sc);

    auto *border = sc->addRect(0,0,800,600,QPen(Qt::gray,1));
    border->setZValue(-1000);

    ui->toolBar->addAction(QIcon(":/images/Open.png"),"открыть",[this](){open();});
    ui->toolBar->addAction(QIcon(":/images/Save.png"),"сохранить",[this](){save();});
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(QIcon(":/images/align_left.png"),"выравнять по левому краю",[this](){sc->alignLeft();});
    ui->toolBar->addAction(QIcon(":/images/align_right.png"),"выравнять по правому краю",[this](){sc->alignRight();});
    ui->toolBar->addAction(QIcon(":/images/align_bottom.png"),"выравнять по нижнему краю",[this](){sc->alignBottom();});
    ui->toolBar->addAction(QIcon(":/images/align_top.png"),"выравнять по верхнему краю",[this](){sc->alignTop();});
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(QIcon(":/images/align_width.png"),"выравнять по максимальной ширине",[this](){sc->alignMaxWidth();});
    ui->toolBar->addAction(QIcon(":/images/align_width_min.png"),"выравнять по минимальной ширине",[this](){sc->alignMinWidth();});
    ui->toolBar->addAction(QIcon(":/images/align_hight.png"),"выравнять по максимальной высоте",[this](){sc->alignMaxHeight();});
    ui->toolBar->addAction(QIcon(":/images/align_height_min.png"),"выравнять по минимальной высоте",[this](){sc->alignMinHeight();});
    ui->toolBar->addAction(QIcon(":/images/square.png"),"квадрат",[this](){sc->setSquare();});
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(QIcon(":/images/top.png"),"на верхний слой",[this](){sc->onTop();});
    ui->toolBar->addAction(QIcon(":/images/bottom.png"),"на нижний слой",[this](){sc->underBottom();});
    ui->toolBar->addSeparator();
    distanceItem = new QSpinBox();
    distanceItem->setRange(30,400);
    distanceItem->setValue(70);
    ui->toolBar->addWidget(distanceItem);
    ui->toolBar->addAction(QIcon(":/images/hor_step.png"),"шаг по горизонтали",[this](){sc->setHorizontalStep(distanceItem->value());});
    ui->toolBar->addAction(QIcon(":/images/vert_step.png"),"шаг по вертикали",[this](){sc->setVerticalStep(distanceItem->value());});
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(QIcon(":/images/lamps/lamp2.png"),"включить все лампы",[this](){sc->turnOnAllLamps();});
    ui->toolBar->addAction(QIcon(":/images/lamps/lamp1.png"),"выключить все лампы",[this](){sc->turnOffAllLamps();});
    prView = new PropertiesView();
    QLayout *layout = prView->getLayout();
    ui->groupBoxProperties->setLayout(layout);

    connect(prView,&PropertiesView::update_x,sc,&DisplayScene::update_x);
    connect(prView,&PropertiesView::update_y,sc,&DisplayScene::update_y);
    connect(prView,&PropertiesView::update_width,sc,&DisplayScene::update_width);
    connect(prView,&PropertiesView::update_height,sc,&DisplayScene::update_height);
    connect(prView,&PropertiesView::update_line_width,sc,&DisplayScene::update_line_width);
    connect(prView,&PropertiesView::updateProperty,sc,&DisplayScene::updateProperty);

//    QPixmap pix(":/images/background.png");
//    pix = pix.scaled(800,600);
//    sc->addPixmap(pix);

    //item2->setZValue(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonPlus_clicked()
{
    ui->graphicsView->scale(1.1,1.1);
    zoom*=1.1;
}

void MainWindow::on_pushButtonMinus_clicked()
{
    ui->graphicsView->scale(1/1.1,1/1.1);
    zoom/=1.1;
}

void MainWindow::on_pushButtonOriginalSize_clicked()
{
    ui->graphicsView->scale(1.0/zoom,1.0/zoom);
    zoom = 1;
}

void MainWindow::on_pushButtonFilledRect_clicked()
{
    FilledRectItem *item = new FilledRectItem(50,50);
    item->setZValue(sc->getMaxZValue());
    setProperties(item->getProperties());
    connect(item,&RectItem::changeRect,sc,&DisplayScene::updateRect);
    sc->setInsertElement(item);
    ui->graphicsView->setFocus();
}

void MainWindow::on_pushButtonImage_clicked()
{
    ScalableImageItem *item = new ScalableImageItem(64,64);
    item->setZValue(sc->getMaxZValue());
    setProperties(item->getProperties());
    connect(item,&RectItem::changeRect,sc,&DisplayScene::updateRect);
    sc->setInsertElement(item);
    ui->graphicsView->setFocus();
}

void MainWindow::on_pushButtonFilledEllipse_clicked()
{
    FilledEllipseItem *item = new FilledEllipseItem(50,50);
    item->setZValue(sc->getMaxZValue());
    setProperties(item->getProperties());
    connect(item,&RectItem::changeRect,sc,&DisplayScene::updateRect);
    sc->setInsertElement(item);
    ui->graphicsView->setFocus();
}

void MainWindow::on_pushButtonText_clicked()
{
    TextItem *item = new TextItem(150,35);
    item->setZValue(sc->getMaxZValue());
    item->updateLineWidth(0);
    setProperties(item->getProperties());
    connect(item,&RectItem::changeRect,sc,&DisplayScene::updateRect);
    sc->setInsertElement(item);
    ui->graphicsView->setFocus();
}

void MainWindow::on_pushButtonLamp_clicked()
{
    LampItem *item = new LampItem(50,50);
    item->setZValue(sc->getMaxZValue());
    item->updateLineWidth(0);
    setProperties(item->getProperties());
    connect(item,&RectItem::changeRect,sc,&DisplayScene::updateRect);
    sc->setInsertElement(item);
    ui->graphicsView->setFocus();
}

void MainWindow::on_pushButtonNumber_clicked()
{
    NumberItem *item = new NumberItem(100,35);
    item->setZValue(sc->getMaxZValue());
    item->updateLineWidth(0);
    setProperties(item->getProperties());
    connect(item,&RectItem::changeRect,sc,&DisplayScene::updateRect);
    sc->setInsertElement(item);
    ui->graphicsView->setFocus();
}
