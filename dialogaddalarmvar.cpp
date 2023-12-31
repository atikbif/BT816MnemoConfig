#include "dialogaddalarmvar.h"
#include "ui_dialogaddalarmvar.h"
#include <QGuiApplication>
#include <QScreen>
#include <QAbstractItemView>

SysVarType DialogAddAlarmVar::lastVarType = SysVarType::CLUSTER_BIT;

DialogAddAlarmVar::DialogAddAlarmVar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddAlarmVar)
{
    ui->setupUi(this);

    ui->comboBoxVarType->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::CLUSTER_BIT));
    ui->comboBoxVarType->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::NET_BIT));

    ui->comboBoxAlarmType->addItem(AlarmInfoVar::getMessageTypeString(MessageType::InfoData));
    ui->comboBoxAlarmType->addItem(AlarmInfoVar::getMessageTypeString(MessageType::WarningData));
    ui->comboBoxAlarmType->addItem(AlarmInfoVar::getMessageTypeString(MessageType::AlarmData));

    auto screens = QGuiApplication::screens();
    uint16_t maxHeight = 800;
    if(screens.size()) {
        maxHeight = screens.at(0)->availableGeometry().height()*0.5;
    }
    ui->comboBoxVarName->view()->window()->setMaximumHeight(maxHeight);

    vType = lastVarType;
    setVarType(vType);
}

DialogAddAlarmVar::~DialogAddAlarmVar()
{
    delete ui;
}

void DialogAddAlarmVar::setVarType(SysVarType value)
{
    if((vType!=SysVarType::CLUSTER_BIT) && (vType!=SysVarType::NET_BIT)) {
        value = SysVarType::CLUSTER_BIT;
    }
    vType = value;
    lastVarType = vType;

    if(vType==SysVarType::CLUSTER_BIT) {
        ui->comboBoxVarType->setCurrentText(SysVar::getDiscreteVarTypeString(DiscreteVarType::CLUSTER_BIT));
    }else if(vType==SysVarType::NET_BIT) {
        ui->comboBoxVarType->setCurrentText(SysVar::getDiscreteVarTypeString(DiscreteVarType::NET_BIT));
    }
}

void DialogAddAlarmVar::setVarIndex(int value)
{
    if(value<ui->comboBoxVarName->count()) {
        ui->comboBoxVarName->setCurrentIndex(value);
    }
}

void DialogAddAlarmVar::setMessageType(MessageType value)
{
    messageType = value;

    if(messageType==MessageType::InfoData) {
        ui->comboBoxAlarmType->setCurrentText(AlarmInfoVar::getMessageTypeString(MessageType::InfoData));
    }else if(messageType==MessageType::WarningData) {
        ui->comboBoxAlarmType->setCurrentText(AlarmInfoVar::getMessageTypeString(MessageType::WarningData));
    }else if(messageType==MessageType::AlarmData) {
        ui->comboBoxAlarmType->setCurrentText(AlarmInfoVar::getMessageTypeString(MessageType::AlarmData));
    }
}

void DialogAddAlarmVar::setSysVarType(SysVarType value)
{
    vType = value;
    lastVarType = vType;

    if(vType==SysVarType::CLUSTER_BIT) {
        ui->comboBoxVarType->setCurrentText(SysVar::getDiscreteVarTypeString(DiscreteVarType::CLUSTER_BIT));
    }else if(vType==SysVarType::NET_BIT) {
        ui->comboBoxVarType->setCurrentText(SysVar::getDiscreteVarTypeString(DiscreteVarType::NET_BIT));
    }
}

SysVarType DialogAddAlarmVar::getVarType() const
{
    return vType;
}

int DialogAddAlarmVar::getVarIndex() const
{
    return varIndex;
}

MessageType DialogAddAlarmVar::getMessageType() const
{
    return messageType;
}

void DialogAddAlarmVar::setPLC(const PLCConfig &plc)
{
    this->plc = plc;
    on_comboBoxVarType_currentIndexChanged(ui->comboBoxVarType->currentIndex());
}

void DialogAddAlarmVar::on_comboBoxVarType_currentIndexChanged(int index)
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
    }
}


void DialogAddAlarmVar::on_comboBoxVarName_currentIndexChanged(int index)
{
    varIndex = index;
}


void DialogAddAlarmVar::on_comboBoxAlarmType_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    if(ui->comboBoxAlarmType->currentText()==AlarmInfoVar::getMessageTypeString(MessageType::InfoData)) {
        messageType = MessageType::InfoData;
    }else if(ui->comboBoxAlarmType->currentText()==AlarmInfoVar::getMessageTypeString(MessageType::WarningData)) {
        messageType = MessageType::WarningData;
    }else if(ui->comboBoxAlarmType->currentText()==AlarmInfoVar::getMessageTypeString(MessageType::AlarmData)) {
        messageType = MessageType::AlarmData;
    }
}

