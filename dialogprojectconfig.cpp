#include "dialogprojectconfig.h"
#include "ui_dialogprojectconfig.h"
#include "dialogaddeditablevar.h"
#include "dialogaddalarmvar.h"
#include <QFileDialog>
#include <QMessageBox>

DialogProjectConfig::DialogProjectConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProjectConfig)
{
    ui->setupUi(this);
    connect(ui->tableWidget,&QTableWidget::cellDoubleClicked,[this](int row,int column){
        Q_UNUSED(column)
        if(row<vars.size()) {
            SysVar var = vars.at(row);

            std::vector<SysVar> sysVars = plc.getSysVarByType(var.varType);
            auto it = std::find_if(sysVars.begin(),sysVars.end(),[var](const SysVar &v){return v.sysName==var.sysName;});
            if(it!=sysVars.end()) {
                int index = std::distance(sysVars.begin(),it);
                DialogAddEditableVar *dialogAddVar = new DialogAddEditableVar();
                dialogAddVar->setPLC(plc);
                dialogAddVar->setVarType(var.varType);
                dialogAddVar->setVarIndex(index);
                if(dialogAddVar->exec()==QDialog::Accepted) {
                    SysVarType vType = dialogAddVar->getVarType();
                    index = dialogAddVar->getVarIndex();
                    sysVars = plc.getSysVarByType(vType);
                    if(sysVars.size()>=index) {
                        SysVar newVar = sysVars.at(index);
                        auto it = std::find_if(vars.begin(),vars.end(),[newVar](const SysVar &v){return newVar.sysName==v.sysName;});
                        if(it==vars.end() || (newVar.sysName==var.sysName)) {
                            vars[row] = newVar;
                            updateGUI();
                            ui->tableWidget->setCurrentCell(row,0);
                        }else {
                            QMessageBox::warning(this,"проверка дубликатов","Переменная с таким именем уже была ранее добавлена");
                        }
                    }
                }
            }
        }

    });

    connect(ui->tableWidgetAlarm,&QTableWidget::cellDoubleClicked,[this](int row,int column){
        Q_UNUSED(column)
        if(row<alarmVars.size()) {
             SysVar var = alarmVars.at(row).var;
             MessageType mType = alarmVars.at(row).messageType;

             std::vector<SysVar> sysVars = plc.getSysVarByType(var.varType);
             auto it = std::find_if(sysVars.begin(),sysVars.end(),[var](const SysVar &v){return v.sysName==var.sysName;});
             if(it!=sysVars.end()) {
                 int index = std::distance(sysVars.begin(),it);

                 DialogAddAlarmVar *dialogAddAlarmVar = new DialogAddAlarmVar();

                 dialogAddAlarmVar->setPLC(plc);
                 dialogAddAlarmVar->setVarType(var.varType);
                 dialogAddAlarmVar->setVarIndex(index);
                 dialogAddAlarmVar->setMessageType(mType);
                 if(dialogAddAlarmVar->exec()==QDialog::Accepted) {
                     mType = dialogAddAlarmVar->getMessageType();
                     SysVarType vType = dialogAddAlarmVar->getVarType();
                     index = dialogAddAlarmVar->getVarIndex();
                     sysVars = plc.getSysVarByType(vType);
                    if(sysVars.size()>=index) {
                         SysVar newVar = sysVars.at(index);
                         auto it = std::find_if(alarmVars.begin(),alarmVars.end(),[newVar](const AlarmInfoVar &v){return newVar.sysName==v.var.sysName;});
                         if(it==alarmVars.end() || (newVar.sysName==var.sysName)) {
                            AlarmInfoVar alInfoVar;
                            alInfoVar.messageType = mType;
                            alInfoVar.var = newVar;
                            alarmVars[row] = alInfoVar;
                            updateGUI();
                            ui->tableWidgetAlarm->setCurrentCell(row,0);
                         }else {
                             QMessageBox::warning(this,"проверка дубликатов","Переменная с таким именем уже была ранее добавлена");
                         }
                     }
                 }
             }
        }
    });
}

DialogProjectConfig::~DialogProjectConfig()
{
    delete ui;
}

void DialogProjectConfig::setCanAddr(uint8_t value)
{
    can_addr = value;
    ui->spinBox->setValue(can_addr);
}

void DialogProjectConfig::setVars(const std::vector<SysVar> &v)
{
    vars = v;

    for(auto &var: vars) {
        SysVarType vType = var.varType;
        std::vector<SysVar> varsByType = plc.getSysVarByType(vType);
        auto it = std::find_if(varsByType.begin(),varsByType.end(),[&](const SysVar &sysVar){
            return (sysVar.sysName==var.sysName);
        });
        if(it!=varsByType.end()) {
            var.userName = it->userName;
        }
    }
}

void DialogProjectConfig::updateGUI()
{
    ui->spinBox->setValue(can_addr);
    while(ui->tableWidget->rowCount()) {
        ui->tableWidget->removeRow(0);
    }
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    for(const auto &v:vars) {
        int i = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(v.sysName));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(v.userName));
    }
    //ui->tableWidget->resizeColumnsToContents();
    if(ui->tableWidget->rowCount()) ui->tableWidget->selectRow(0);
    ui->lineEditEvePath->setText(evePath);

    ui->tableWidgetAlarm->setSelectionBehavior(QAbstractItemView::SelectRows);

    while(ui->tableWidgetAlarm->rowCount()) {
        ui->tableWidgetAlarm->removeRow(0);
    }

    for(const auto &v:alarmVars) {
        int i = ui->tableWidgetAlarm->rowCount();
        ui->tableWidgetAlarm->insertRow(i);
        ui->tableWidgetAlarm->setItem(i,0,new QTableWidgetItem(v.var.sysName));
        ui->tableWidgetAlarm->setItem(i,1,new QTableWidgetItem(AlarmInfoVar::getMessageTypeString(v.messageType)));
        ui->tableWidgetAlarm->setItem(i,2,new QTableWidgetItem(v.var.userName));
        if(v.messageType==MessageType::InfoData){
            ui->tableWidgetAlarm->item(i, 0) ->setData(Qt::BackgroundRole, QColor (0,255,0));
            ui->tableWidgetAlarm->item(i, 1) ->setData(Qt::BackgroundRole, QColor (0,255,0));
            ui->tableWidgetAlarm->item(i, 2) ->setData(Qt::BackgroundRole, QColor (0,255,0));
        }else if(v.messageType==MessageType::WarningData){
            ui->tableWidgetAlarm->item(i, 0) ->setData(Qt::BackgroundRole, QColor (255,255,0));
            ui->tableWidgetAlarm->item(i, 1) ->setData(Qt::BackgroundRole, QColor (255,255,0));
            ui->tableWidgetAlarm->item(i, 2) ->setData(Qt::BackgroundRole, QColor (255,255,0));
        }else if(v.messageType==MessageType::AlarmData){
            ui->tableWidgetAlarm->item(i, 0) ->setData(Qt::BackgroundRole, QColor (255,0,0));
            ui->tableWidgetAlarm->item(i, 1) ->setData(Qt::BackgroundRole, QColor (255,0,0));
            ui->tableWidgetAlarm->item(i, 2) ->setData(Qt::BackgroundRole, QColor (255,0,0));
        }
    }
    if(ui->tableWidgetAlarm->rowCount()) ui->tableWidget->selectRow(0);
}

uint8_t DialogProjectConfig::getCanAddr() const
{
    return ui->spinBox->value();
}

std::vector<SysVar> DialogProjectConfig::getVars() const
{
    return vars;
}

std::vector<AlarmInfoVar> DialogProjectConfig::getInfoVar() const
{
    return alarmVars;
}

void DialogProjectConfig::setPLC(const PLCConfig &plc)
{
    this->plc = plc;
    alarmVars = plc.getAlarmInfoVar();
}

void DialogProjectConfig::setEVEPath(const QString &value)
{
    evePath = value;
    ui->lineEditEvePath->setText(evePath);
}

void DialogProjectConfig::on_pushButtonAdd_clicked()
{
    DialogAddEditableVar *dialogAddVar = new DialogAddEditableVar();
    dialogAddVar->setPLC(plc);
    if(dialogAddVar->exec()==QDialog::Accepted) {
        SysVarType vType = dialogAddVar->getVarType();
        int index = dialogAddVar->getVarIndex();
        std::vector<SysVar> sysVars = plc.getSysVarByType(vType);
        if(sysVars.size()>=index) {
            SysVar var = sysVars.at(index);
            auto it = std::find_if(vars.begin(),vars.end(),[var](const SysVar &v){return var.sysName==v.sysName;});
            if(it==vars.end()) {
                int i = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(i);
                ui->tableWidget->setItem(i,0,new QTableWidgetItem(var.sysName));
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(var.userName));
                vars.push_back(var);
                ui->tableWidget->selectRow(ui->tableWidget->rowCount()-1);
            }else {
                QMessageBox::warning(this,"проверка дубликатов","Переменная с таким именем уже была ранее добавлена");
            }
        }
    }
    delete dialogAddVar;
}


void DialogProjectConfig::on_pushButtonDel_clicked()
{
    if(ui->tableWidget->rowCount()) {
        QItemSelectionModel *select = ui->tableWidget->selectionModel();
        QModelIndexList selection = select->selectedRows();
        if(!selection.isEmpty()) {
            int index = selection.at(0).row();
            vars.erase(vars.begin() + index);
            ui->tableWidget->removeRow(index);
            if(ui->tableWidget->rowCount()) {
                while(index>=ui->tableWidget->rowCount()) index--;
                if(index>=0) ui->tableWidget->selectRow(index);
            }
        }
    }
}


void DialogProjectConfig::on_pushButtonUp_clicked()
{
    if(ui->tableWidget->rowCount()) {
        QItemSelectionModel *select = ui->tableWidget->selectionModel();
        QModelIndexList selection = select->selectedRows();
        if(!selection.isEmpty()) {
            int index = selection.at(0).row();
            if(index>0) {
                auto selVar = vars.at(index);
                auto prevVar = vars.at(index-1);
                vars[index-1] = selVar;
                vars[index] = prevVar;
                updateGUI();
                index--;
                ui->tableWidget->selectRow(index);
            }
        }
    }
}


void DialogProjectConfig::on_pushButtonDown_clicked()
{
    if(ui->tableWidget->rowCount()) {
        QItemSelectionModel *select = ui->tableWidget->selectionModel();
        QModelIndexList selection = select->selectedRows();
        if(!selection.isEmpty()) {
            int index = selection.at(0).row();
            if(index<(ui->tableWidget->rowCount()-1)) {
                auto selVar = vars.at(index);
                auto nextVar = vars.at(index+1);
                vars[index+1] = selVar;
                vars[index] = nextVar;
                updateGUI();
                index++;
                ui->tableWidget->selectRow(index);
            }
        }
    }
}


void DialogProjectConfig::on_pushButtonEvePath_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, "Путь к EVE Asset Builder", evePath);
    if(!dirName.isEmpty()) {
        evePath = dirName;
        ui->lineEditEvePath->setText(evePath);
    }
}


void DialogProjectConfig::on_pushButtonAddAlarm_clicked()
{
    DialogAddAlarmVar *dialogAddVar = new DialogAddAlarmVar();
    dialogAddVar->setPLC(plc);
    if(dialogAddVar->exec()==QDialog::Accepted) {
        SysVarType vType = dialogAddVar->getVarType();
        int index = dialogAddVar->getVarIndex();
        MessageType mType = dialogAddVar->getMessageType();
        std::vector<SysVar> sysVars = plc.getSysVarByType(vType);
        if(sysVars.size()>=index) {
            SysVar var = sysVars.at(index);
            auto it = std::find_if(alarmVars.begin(),alarmVars.end(),[var](const AlarmInfoVar &v){return var.sysName==v.var.sysName;});
            if(it==alarmVars.end()) {
                int i = ui->tableWidgetAlarm->rowCount();
                ui->tableWidgetAlarm->insertRow(i);
                ui->tableWidgetAlarm->setItem(i,0,new QTableWidgetItem(var.sysName));
                ui->tableWidgetAlarm->setItem(i,1,new QTableWidgetItem(AlarmInfoVar::getMessageTypeString(mType)));
                ui->tableWidgetAlarm->setItem(i,2,new QTableWidgetItem(var.userName));
                if(mType==MessageType::InfoData){
                    ui->tableWidgetAlarm->item(i, 0) ->setData(Qt::BackgroundRole, QColor (0,255,0));
                    ui->tableWidgetAlarm->item(i, 1) ->setData(Qt::BackgroundRole, QColor (0,255,0));
                    ui->tableWidgetAlarm->item(i, 2) ->setData(Qt::BackgroundRole, QColor (0,255,0));
                }else if(mType==MessageType::WarningData){
                    ui->tableWidgetAlarm->item(i, 0) ->setData(Qt::BackgroundRole, QColor (255,255,0));
                    ui->tableWidgetAlarm->item(i, 1) ->setData(Qt::BackgroundRole, QColor (255,255,0));
                    ui->tableWidgetAlarm->item(i, 2) ->setData(Qt::BackgroundRole, QColor (255,255,0));
                }else if(mType==MessageType::AlarmData){
                    ui->tableWidgetAlarm->item(i, 0) ->setData(Qt::BackgroundRole, QColor (255,0,0));
                    ui->tableWidgetAlarm->item(i, 1) ->setData(Qt::BackgroundRole, QColor (255,0,0));
                    ui->tableWidgetAlarm->item(i, 2) ->setData(Qt::BackgroundRole, QColor (255,0,0));
                }
                AlarmInfoVar v;
                v.messageType = mType;
                v.var = var;
                alarmVars.push_back(v);
                ui->tableWidgetAlarm->selectRow(ui->tableWidget->rowCount()-1);
            }else {
                QMessageBox::warning(this,"проверка дубликатов","Переменная с таким именем уже была ранее добавлена");
            }
        }
    }
    delete dialogAddVar;
}


void DialogProjectConfig::on_pushButtonDelAlarm_clicked()
{
    if(ui->tableWidgetAlarm->rowCount()) {
        QItemSelectionModel *select = ui->tableWidgetAlarm->selectionModel();
        QModelIndexList selection = select->selectedRows();
        if(!selection.isEmpty()) {
            int index = selection.at(0).row();
            alarmVars.erase(alarmVars.begin() + index);
            ui->tableWidgetAlarm->removeRow(index);
            if(ui->tableWidgetAlarm->rowCount()) {
                while(index>=ui->tableWidgetAlarm->rowCount()) index--;
                if(index>=0) ui->tableWidgetAlarm->selectRow(index);
            }
        }
    }
}

