#ifndef DIALOGADDEDITABLEVAR_H
#define DIALOGADDEDITABLEVAR_H

#include <QDialog>
#include "plcconfig.h"

namespace Ui {
class DialogAddEditableVar;
}

class DialogAddEditableVar : public QDialog
{
    Q_OBJECT

    static SysVarType lastVarType;
    SysVarType vType;
    int varIndex = 0;
    PLCConfig plc;

public:
    explicit DialogAddEditableVar(QWidget *parent = nullptr);
    ~DialogAddEditableVar();
    void setVarType(SysVarType value);
    void setVarIndex(int value);
    SysVarType getVarType() const;
    int getVarIndex() const;
    void setPLC(const PLCConfig &plc);

private slots:
    void on_comboBoxVarType_currentIndexChanged(int index);

    void on_comboBoxVarName_currentIndexChanged(int index);

private:
    Ui::DialogAddEditableVar *ui;
};

#endif // DIALOGADDEDITABLEVAR_H
