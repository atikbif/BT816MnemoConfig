#include "mainwindow.h"
#include "qevent.h"
#include "ui_mainwindow.h"
#include "displayscene.h"
#include "Elements/Widgets/rectitem.h"
#include "Elements/Widgets/filledrectitem.h"
#include "Elements/Widgets/filledcircleitem.h"
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
#include "jsonplcconfigreader.h"
#include "dialogprojectconfig.h"
#include <QMessageBox>
#include "QSettings"
#include "QProcess"

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
    result["plc project"] = plcPrName;
    result["background image"] = backgroundImage;
    result["can address"] = canAddr;

    editVars.toJSON(result);

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

        editVars.clear();
        if(backgroundItem) {
            sc->removeItem(backgroundItem);
            delete(backgroundItem);
            sc->update();
            backgroundItem = nullptr;
        }
        plc = PLCConfig();

        sc->deselectAllItems();
        sc->removeInsertItem();
        sc->clear();
        auto *border = sc->addRect(0,0,800,480,QPen(Qt::gray,1));
        border->setZValue(-1000);
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        QJsonObject json = loadDoc.object();

        if(!(json.contains("key") && json["key"]=="DISPLAY MTS")) {
            ui->statusbar->showMessage("Неверный формат файла", 3000);
            return;
        }
        if(json.contains("can address")) {
            canAddr = json["can address"].toInt();
        }

        if(json.contains("plc project") && json["plc project"].isString()) {
            plcPrName = json["plc project"].toString();
        }else plcPrName = "";

        if(json.contains("background image") && json["background image"].isString()) {
            backgroundImage = json["background image"].toString();
        }else backgroundImage = "";

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
                    else if(itemName=="filled_circle") item = new FilledCircleItem(width,height);
                    else if(itemName=="lamp") item = new LampItem(width,height);
                    else if(itemName=="text") item = new TextItem(width,height);
                    else if(itemName=="number") item = new NumberItem(width,height);
                    //else if(itemName=="scal_image") item = new ScalableImageItem(width,height);


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
            editVars.fromJSON(json);
            ui->statusbar->showMessage("Файл успешно открыт", 3000);

            if(!plcPrName.isEmpty()) {
                if(QFile::exists(plcPrName)) {
                    JSONPLCConfigReader reader;
                    auto res = reader.readFromFile(plcPrName);
                    if(res) {
                        plc = res.value();
                        if(prView) {
                            prView->setPLCConfig(plc);
                        }
                    }
                }
            }

            if(!backgroundImage.isEmpty()) {
                if(backgroundItem) {
                    sc->removeItem(backgroundItem);
                    delete backgroundItem;
                    backgroundItem = nullptr;
                }

                if(backgroundItem==nullptr) {
                    if(QFile::exists(backgroundImage)) {
                        QPixmap pix(backgroundImage);
                        pix = pix.scaled(800,480);
                        backgroundItem = sc->addPixmap(pix);
                        backgroundItem->setZValue(-1000);
                    }

                }
            }

        }else ui->statusbar->showMessage("Неверный формат файла", 3000);
    }else ui->statusbar->showMessage("Ошибка открытия файла", 3000);

}

void MainWindow::buildConfigFile()
{
    if((backgroundItem!=nullptr) && (!backgroundImage.isEmpty())) {
        QFile converterFile = QFile(evePath + "/tools/eab_tools.exe");
        if(converterFile.exists()) {
            QString program = "\"" + QFileInfo(converterFile).absoluteFilePath() + "\"";

            QFile backgroundFile = QFile(backgroundImage);

            QString attr = QString(" img_cvt -i ") +  "\"" + QFileInfo(backgroundFile).absoluteFilePath() + "\"" + " -o " + "\"" + QFileInfo(backgroundFile).absolutePath() + "/conversion\"" +  " -f 24 -e thorough";
            QProcess builder;

            builder.setProcessChannelMode(QProcess::MergedChannels);
            builder.start(program+attr);
            if (!builder.waitForFinished()) {
                QMessageBox::warning(this,"ошибка конвертации",builder.errorString() + " команда " + program + attr);
                return;
            }

        }else {
            QMessageBox::warning(this,"ошибка конвертации","Не найден файл "+QFileInfo(converterFile).absoluteFilePath());
            return;
        }
    }
    QMessageBox::information(this,"сборка проекта","файл конфигурации дисплея успешно сгенерирован");
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->resize(800+200,480+200);

    sc = new DisplayScene();
    sc->setSceneRect(0,0,800,480);

    connect(sc,&DisplayScene::updateRect,this,&MainWindow::updateRect);
    connect(sc,&DisplayScene::clearProperties,this,&MainWindow::clearProperties);
    connect(sc,&DisplayScene::setProperties,this,&MainWindow::setProperties);

    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setScene(sc);

    auto *border = sc->addRect(0,0,800,480,QPen(Qt::gray,1));
    border->setZValue(-1000);



    ui->toolBar->addAction(QIcon(":/images/Open.png"),"открыть",[this](){open();});
    ui->toolBar->addAction(QIcon(":/images/Save.png"),"сохранить",[this](){save();});
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(QIcon(":/images/background.png"),"Фон",[this](){
        QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл с фоновым изображением", "", "*.png");
        if(fileName.isEmpty()) return;
        if(backgroundItem) {
            sc->removeItem(backgroundItem);
            delete backgroundItem;
            backgroundItem = nullptr;
        }

        if(backgroundItem==nullptr) {
            QPixmap pix(fileName);
            pix = pix.scaled(800,480);

            QDir dir("PRDATA");
            if (!dir.exists()) dir.mkpath(".");

            QFileInfo fInfo(fileName);
            fInfo.baseName();

            QString copyFileName = "PRDATA/" + fInfo.baseName() + ".png";
            QFile file(copyFileName);
            backgroundImage = copyFileName;
            file.open(QIODevice::WriteOnly);
            pix.save(&file, "PNG");
            file.close();
            backgroundItem = sc->addPixmap(pix);
            backgroundItem->setZValue(-1000);
        }

    });

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
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(QIcon(":/images/build.png"),"сгенерировать файл конфигурации дисплея",[this](){
        buildConfigFile();
    });
    prView = new PropertiesView();
    prView->setPLCConfig(plc);
    QLayout *layout = prView->getLayout();
    ui->groupBoxProperties->setLayout(layout);

    connect(prView,&PropertiesView::update_x,sc,&DisplayScene::update_x);
    connect(prView,&PropertiesView::update_y,sc,&DisplayScene::update_y);
    connect(prView,&PropertiesView::update_width,sc,&DisplayScene::update_width);
    connect(prView,&PropertiesView::update_height,sc,&DisplayScene::update_height);
    connect(prView,&PropertiesView::update_line_width,sc,&DisplayScene::update_line_width);
    connect(prView,&PropertiesView::updateProperty,sc,&DisplayScene::updateProperty);

    connect(ui->action_project_properties,&QAction::triggered,[this](){
        DialogProjectConfig *dialogPrConfig = new DialogProjectConfig();
        dialogPrConfig->setPLC(plc);
        dialogPrConfig->setCanAddr(canAddr);
        dialogPrConfig->setVars(editVars.getVars());
        dialogPrConfig->setEVEPath(evePath);
        dialogPrConfig->updateGUI();
        if(dialogPrConfig->exec()==QDialog::Accepted) {
            editVars.setVars(dialogPrConfig->getVars());
            canAddr = dialogPrConfig->getCanAddr();
            evePath = dialogPrConfig->getEVEPath();
            QSettings settings;
            settings.setValue("/Settings/evePath",evePath);
        }
        delete dialogPrConfig;
    });

    connect(ui->action_PC21,&QAction::triggered,[this](){
        QString fileName = QFileDialog::getOpenFileName(this, "Открыть проект", "", "Проект контроллера *.ldp");
        if(!fileName.isEmpty()) {
            JSONPLCConfigReader reader;
            auto res = reader.readFromFile(fileName);
            if(res) {
                plc = res.value();
                if(prView) {
                    prView->setPLCConfig(plc);

                    QDir dir("PRDATA");
                    if (!dir.exists()) dir.mkpath(".");

                    QFileInfo fInfo(fileName);
                    fInfo.baseName();
                    QFile::copy(fileName,"PRDATA/"+fInfo.baseName()+".ldp");
                    plcPrName = "PRDATA/"+fInfo.baseName()+".ldp";
                }
            }
        }
    });

    connect(ui->action_about,&QAction::triggered,[](){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Редактор мнемосхем");
        msgBox.setText("Версия " + QString(APP_VERSION));
        msgBox.exec();
    });

    connect(ui->action_clear_background,&QAction::triggered,[this](){
        if(backgroundItem) {
            sc->removeItem(backgroundItem);
            delete(backgroundItem);
            sc->update();
            backgroundItem = nullptr;
        }
    });

    connect(ui->actionOpenProject,&QAction::triggered,[this](){
        open();
    });

    connect(ui->actionSaveProject,&QAction::triggered,[this](){
        save();
    });

    connect(ui->actionSetBackground,&QAction::triggered,[this](){
        QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл с фоновым изображением", "", "*.png");
        if(fileName.isEmpty()) return;
        if(backgroundItem) {
            sc->removeItem(backgroundItem);
            delete backgroundItem;
            backgroundItem = nullptr;
        }

        if(backgroundItem==nullptr) {
            QPixmap pix(fileName);
            pix = pix.scaled(800,480);

            QDir dir("PRDATA");
            if (!dir.exists()) dir.mkpath(".");

            QFileInfo fInfo(fileName);
            fInfo.baseName();

            QString copyFileName = "PRDATA/" + fInfo.baseName() + ".png";
            QFile file(copyFileName);
            backgroundImage = copyFileName;
            file.open(QIODevice::WriteOnly);
            pix.save(&file, "PNG");
            file.close();
            backgroundItem = sc->addPixmap(pix);
            backgroundItem->setZValue(-1000);
        }
    });

    QSettings settings;
    evePath = settings.value("/Settings/evePath",QString()).toString();
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
    clearProperties();
    FilledRectItem *item = new FilledRectItem(50,50);
    item->setZValue(sc->getMaxZValue());
    setProperties(item->getProperties());
    connect(item,&RectItem::changeRect,sc,&DisplayScene::updateRect);
    sc->setInsertElement(item);
    ui->graphicsView->setFocus();
}

void MainWindow::on_pushButtonFilledCircle_clicked()
{
    clearProperties();
    FilledCircleItem *item = new FilledCircleItem(50,50);
    item->setZValue(sc->getMaxZValue());
    setProperties(item->getProperties());
    connect(item,&RectItem::changeRect,sc,&DisplayScene::updateRect);
    sc->setInsertElement(item);
    ui->graphicsView->setFocus();
}

void MainWindow::on_pushButtonText_clicked()
{
    clearProperties();
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
    clearProperties();
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
    clearProperties();
    NumberItem *item = new NumberItem(100,35);
    item->setZValue(sc->getMaxZValue());
    item->updateLineWidth(0);
    setProperties(item->getProperties());
    connect(item,&RectItem::changeRect,sc,&DisplayScene::updateRect);
    sc->setInsertElement(item);
    ui->graphicsView->setFocus();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
        case Qt::Key_Plus:
            if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
                on_pushButtonPlus_clicked();
            }
            break;
        case Qt::Key_Minus:
            if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
                on_pushButtonMinus_clicked();
            }
            break;
    }
}
