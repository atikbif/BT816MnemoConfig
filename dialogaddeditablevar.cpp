#include "dialogaddeditablevar.h"
#include "ui_dialogaddeditablevar.h"

SysVarType DialogAddEditableVar::lastVarType = SysVarType::CLUSTER_BIT;

DialogAddEditableVar::DialogAddEditableVar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddEditableVar)
{
    ui->setupUi(this);

    ui->comboBoxVarType->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::CLUSTER_BIT));
    ui->comboBoxVarType->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::NET_BIT));
    ui->comboBoxVarType->addItem(SysVar::getAnalogueVarTypeString(AnalogueVarType::CLUSTER_REG));
    ui->comboBoxVarType->addItem(SysVar::getAnalogueVarTypeString(AnalogueVarType::NET_REG));

    vType = lastVarType;

    setVarType(vType);
}

DialogAddEditableVar::~DialogAddEditableVar()
{
    delete ui;
}

void DialogAddEditableVar::setVarType(SysVarType value)
{
    vType = value;
    lastVarType = vType;

    if(vType==SysVarType::CLUSTER_BIT) {
        ui->comboBoxVarType->setCurrentText(SysVar::getDiscreteVarTypeString(DiscreteVarType::CLUSTER_BIT));
    }else if(vType==SysVarType::NET_BIT) {
        ui->comboBoxVarType->setCurrentText(SysVar::getDiscreteVarTypeString(DiscreteVarType::NET_BIT));
    }else if(vType==SysVarType::CLUSTER_REG) {
        ui->comboBoxVarType->setCurrentText(SysVar::getAnalogueVarTypeString(AnalogueVarType::CLUSTER_REG));
    }else if(vType==SysVarType::NET_REG) {
        ui->comboBoxVarType->setCurrentText(SysVar::getAnalogueVarTypeString(AnalogueVarType::NET_REG));
    }
}

SysVarType DialogAddEditableVar::getVarType() const
{
    return vType;
}

int DialogAddEditableVar::getVarIndex() const
{
    return varIndex;
}

void DialogAddEditableVar::setPLC(const PLCConfig &plc)
{
    this->plc = plc;
    on_comboBoxVarType_currentIndexChanged(ui->comboBoxVarType->currentIndex());
}

void DialogAddEditableVar::on_comboBoxVarType_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    QString curType = ui->comboBoxVarType->currentText();
    if(curType==SysVar::getDiscreteVarTypeString(DiscreteVarType::CLUSTER_BIT)) {
        setVarType(SysVarType::CLUSTER_BIT);
        std::vector<Var> vars = plc.getDiscreteVarByType(DiscreteVarType::CLUSTER_BIT);
        ui->comboBoxVarName->clear();
        for(const auto &v:vars) {
            QString varName = v.sysName;
            if(!v.userName.isEmpty()) {
                varName += " (" + v.userName + ")";
            }
            ui->comboBoxVarName->addItem(varName);
        }
    }else if(curType==SysVar::getDiscreteVarTypeString(DiscreteVarType::NET_BIT)) {
        setVarType(SysVarType::NET_BIT);
        std::vector<Var> vars = plc.getDiscreteVarByType(DiscreteVarType::NET_BIT);
        ui->comboBoxVarName->clear();
        for(const auto &v:vars) {
            QString varName = v.sysName;
            if(!v.userName.isEmpty()) {
                varName += " (" + v.userName + ")";
            }
            ui->comboBoxVarName->addItem(varName);
        }
    }else if(curType==SysVar::getAnalogueVarTypeString(AnalogueVarType::CLUSTER_REG)) {
        setVarType(SysVarType::CLUSTER_REG);
        std::vector<Var> vars = plc.getAnalogueVarByType(AnalogueVarType::CLUSTER_REG);
        ui->comboBoxVarName->clear();
        for(const auto &v:vars) {
            QString varName = v.sysName;
            if(!v.userName.isEmpty()) {
                varName += " (" + v.userName + ")";
            }
            ui->comboBoxVarName->addItem(varName);
        }
    }else if(curType==SysVar::getAnalogueVarTypeString(AnalogueVarType::NET_REG)) {
        setVarType(SysVarType::NET_REG);
        std::vector<Var> vars = plc.getAnalogueVarByType(AnalogueVarType::NET_REG);
        ui->comboBoxVarName->clear();
        for(const auto &v:vars) {
            QString varName = v.sysName;
            if(!v.userName.isEmpty()) {
                varName += " (" + v.userName + ")";
            }
            ui->comboBoxVarName->addItem(varName);
        }
    }
}


void DialogAddEditableVar::on_comboBoxVarName_currentIndexChanged(int index)
{
    varIndex = index;
}

