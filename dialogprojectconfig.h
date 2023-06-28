#ifndef DIALOGPROJECTCONFIG_H
#define DIALOGPROJECTCONFIG_H

#include <QDialog>
#include "plcconfig.h"

namespace Ui {
class DialogProjectConfig;
}

class DialogProjectConfig : public QDialog
{
    Q_OBJECT
    uint8_t can_addr;
    std::vector<SysVar> vars;
    PLCConfig plc;
    QString evePath;
public:
    explicit DialogProjectConfig(QWidget *parent = nullptr);
    ~DialogProjectConfig();

    void setCanAddr(uint8_t value);
    void setVars(const std::vector<SysVar> &v);
    void updateGUI();
    uint8_t getCanAddr() const;
    std::vector<SysVar> getVars() const;
    void setPLC(PLCConfig plc);
    void setEVEPath(const QString &value);
    QString getEVEPath() {return evePath;}

private slots:
    void on_pushButtonAdd_clicked();

    void on_pushButtonDel_clicked();

    void on_pushButtonUp_clicked();

    void on_pushButtonDown_clicked();

    void on_pushButtonEvePath_clicked();

private:
    Ui::DialogProjectConfig *ui;
};

#endif // DIALOGPROJECTCONFIG_H
