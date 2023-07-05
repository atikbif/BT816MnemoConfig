#include "dialogprojectconfig.h"
#include "ui_dialogprojectconfig.h"
#include "dialogaddeditablevar.h"
#include <QFileDialog>

DialogProjectConfig::DialogProjectConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProjectConfig)
{
    ui->setupUi(this);
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
}

uint8_t DialogProjectConfig::getCanAddr() const
{
    return ui->spinBox->value();
}

std::vector<SysVar> DialogProjectConfig::getVars() const
{
    return vars;
}

void DialogProjectConfig::setPLC(const PLCConfig &plc)
{
    this->plc = plc;
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
            int i = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(i);
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(sysVars.at(index).sysName));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(sysVars.at(index).userName));
            vars.push_back(sysVars.at(index));
            ui->tableWidget->selectRow(ui->tableWidget->rowCount()-1);
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

