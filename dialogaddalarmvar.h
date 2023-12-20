#ifndef DIALOGADDALARMVAR_H
#define DIALOGADDALARMVAR_H

#include <QDialog>
#include "plcconfig.h"

namespace Ui {
class DialogAddAlarmVar;
}

class DialogAddAlarmVar : public QDialog
{
    Q_OBJECT

    static SysVarType lastVarType;
    SysVarType vType;
    int varIndex = 0;
    MessageType messageType = MessageType::InfoData;

    PLCConfig plc;

public:
    explicit DialogAddAlarmVar(QWidget *parent = nullptr);
    ~DialogAddAlarmVar();
    void setVarType(SysVarType value);
    void setVarIndex(int value);
    void setMessageType(MessageType value);
    void setSysVarType(SysVarType value);
    SysVarType getVarType() const;
    int getVarIndex() const;
    MessageType getMessageType() const;
    void setPLC(const PLCConfig &plc);

private slots:
    void on_comboBoxVarType_currentIndexChanged(int index);

    void on_comboBoxVarName_currentIndexChanged(int index);

    void on_comboBoxAlarmType_currentIndexChanged(int index);

private:
    Ui::DialogAddAlarmVar *ui;
};

#endif // DIALOGADDALARMVAR_H
