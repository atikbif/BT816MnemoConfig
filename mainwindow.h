#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "displayscene.h"
#include "Elements/Properties/propertiesview.h"
#include <QSpinBox>
#include "plcconfig.h"
#include "editablesysvariables.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    qreal zoom = 1;
    DisplayScene *sc = nullptr;
    PropertiesView *prView = nullptr;
    QSpinBox *distanceItem = nullptr;
    QGraphicsPixmapItem *backgroundItem = nullptr;
    EditableSysVariables editVars;

    PLCConfig plc;
    QString backgroundImage;
    QString plcPrName;
    uint8_t canAddr = 1;
    QString evePath;

    QString lcdConfPrName = "PC21LCDConfig.bin";

    void setProperties(const std::vector<ElProperty> &properties);
    void updateRect(int x, int y, int w, int h, int lw);
    void clearProperties();
    void save();
    void open();
    void buildConfigFile();
    std::vector<RectItem*> getGraphicsItems();
    std::vector<RectItem*> getNumberItems();
    std::vector<RectItem*> getLampItems();
    std::vector<RectItem*> getTextItems();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButtonPlus_clicked();

    void on_pushButtonMinus_clicked();

    void on_pushButtonOriginalSize_clicked();

    void on_pushButtonFilledRect_clicked();

    void on_pushButtonFilledCircle_clicked();

    void on_pushButtonText_clicked();

    void on_pushButtonLamp_clicked();

    void on_pushButtonNumber_clicked();

private:
    Ui::MainWindow *ui;



    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
