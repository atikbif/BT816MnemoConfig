#include "dialogprojectconfig.h"
#include "ui_dialogprojectconfig.h"
#include "dialogaddeditablevar.h"

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
}

void DialogProjectConfig::setVars(const std::vector<SysVar> &v)
{
    vars = v;
    for(auto &v: vars) {
        SysVarType vType = v.varType;
        for(const auto &plcV:plc.getSysVarByType(vType)) {
            if(plcV.sysName==v.sysName) {
                v.userName = plcV.userName;
                break;
            }
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
}

uint8_t DialogProjectConfig::getCanAddr() const
{
    return can_addr;
}

std::vector<SysVar> DialogProjectConfig::getVars() const
{
    return vars;
}

void DialogProjectConfig::setPLC(PLCConfig plc)
{
    this->plc = plc;
}

void DialogProjectConfig::on_pushButtonAdd_clicked()
{
    DialogAddEditableVar *dialogAddVar = new DialogAddEditableVar();
    dialogAddVar->setPLC(plc);
    if(dialogAddVar->exec()==QDialog::Accepted) {
        SysVarType vType = dialogAddVar->getVarType();
        int index = dialogAddVar->getVarIndex();
        std::vector<SysVar> vars = plc.getSysVarByType(vType);
        if(vars.size()>=index) {
            int i = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(i);
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(vars.at(index).sysName));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(vars.at(index).userName));
            this->vars.push_back(vars.at(index));
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

