#include "jsonplcconfigreader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <optional>


JSONPLCConfigReader::JSONPLCConfigReader()
{

}

std::optional<PLCConfig> JSONPLCConfigReader::readFromFile(const QString &fName)
{
    QFile file(fName);
    if(file.open(QIODevice::ReadOnly)) {
        QByteArray prData = file.readAll();
        QJsonDocument prDoc = QJsonDocument::fromJson(prData);
        QJsonObject prOb = prDoc.object();
        if(prOb.contains("PLC Config")&&prOb["PLC Config"].isObject()) {
            QJsonObject plcOb = prOb["PLC Config"].toObject();
            QString plcName = plcOb["Name"].toString("");
            PLCConfig conf(plcName);

            conf.appCN = plcOb["Application CN"].toInt(0);
            conf.appCheckSum = plcOb["Application CRC"].toInt(0);
            conf.appName = plcOb["Application Name"].toString("");
            conf.appTime = plcOb["Application Time"].toString("01.01.2001 00:00:00");
            conf.appVersion = plcOb["Application Version"].toString("1.0");

            // read analogue sensors configuration

            std::vector<Sensor> sensors;

            if(plcOb.contains("Sensors") && plcOb["Sensors"].isArray()) {

                QJsonArray sensArray = plcOb["Sensors"].toArray();
                for(auto element: sensArray) {
                    QJsonObject sensOb = element.toObject();
                    Sensor sens;
                    sens.sensType = static_cast<SensType>(sensOb["Code"].toInt(0));
                    sens.highLimit = sensOb["High Limit"].toString("0");
                    sens.lowLimit = sensOb["Low Limit"].toString("0");
                    sens.name = sensOb["Name"].toString("");
                    sens.measureUnit = sensOb["Unit"].toString("");
                    sensors.push_back(sens);
                }
            }

            std::vector<int> sensConnection;

            if(plcOb.contains("Sensor Connection") && plcOb["Sensor Connection"].isArray()) {
                QJsonArray sensConnectionArray = plcOb["Sensor Connection"].toArray();
                for(const auto &el:sensConnectionArray) {
                    sensConnection.push_back(el.toInt(0));
                }
            }

            if(prOb.contains("Variable Storage") && prOb["Variable Storage"].isObject()) {
                QJsonObject varStorOb = prOb["Variable Storage"].toObject();
                if(varStorOb.contains("System Variables") && varStorOb["System Variables"].isObject()) {
                    QJsonObject sysVarStorOb = varStorOb["System Variables"].toObject();
                    if(sysVarStorOb.contains("Parent Groups") && sysVarStorOb["Parent Groups"].isArray()) {
                        QJsonArray sysVarParGroupsArray = sysVarStorOb["Parent Groups"].toArray();

                        // read inputs configuration

                        if(plcOb.contains("Input Types") && plcOb["Input Types"].isArray()) {
                            QJsonArray inpTypesArray = plcOb["Input Types"].toArray();
                            int inpNum = 0;
                            for(const auto &inpType:inpTypesArray) {

                                // input patterns

                                DiscreteInp diInp;
                                diInp.num = inpNum+1;
                                diInp.userName = "";
                                diInp.sysName = "DI" + QString::number(inpNum+1);
                                diInp.usedFlag = false;

                                AnalogueInp anInp;
                                anInp.num = inpNum+1;
                                anInp.sysName = "AI" + QString::number(inpNum+1);
                                anInp.userName = "";
                                anInp.inpType = InpType::VType;
                                Sensor s;
                                s.sensType = SensType::NC;
                                s.highLimit = "0";
                                s.lowLimit = "0";
                                s.measureUnit = "";
                                s.name = "-";
                                anInp.sensor = s;
                                anInp.usedFlag = false;

                                bool discrInpFlag  = false;
                                if(QString::compare(inpType.toString(),"I")) {
                                    if(sensConnection.size()>inpNum) {
                                        if(sensConnection.at(inpNum)==0) discrInpFlag = true;
                                    }
                                }
                                if(discrInpFlag) {
                                    for(auto el:sysVarParGroupsArray) {
                                        QJsonObject elOb = el.toObject();
                                        if(elOb.contains("Name") && elOb["Name"].isString()) {
                                            if(QString::compare(elOb["Name"].toString(),"Дискретные входы")) {
                                                if(elOb.contains("Groups") && elOb["Groups"].isArray()) {
                                                    QJsonArray diVarsArray = elOb["Groups"].toArray();
                                                    for(auto gr:diVarsArray) {
                                                        QJsonObject grOb = gr.toObject();
                                                        if(grOb.contains("Name") && grOb["Name"].isString()) {
                                                            QString grName = grOb["Name"].toString("");
                                                            if(QString::compare(grName,"состояние")) {
                                                                if(grOb.contains("Variables") && grOb["Variables"].isArray()) {
                                                                    QJsonArray varArray = grOb["Variables"].toArray();
                                                                    int varNum = 0;
                                                                    for(auto var:varArray) {
                                                                        QJsonObject varOb = var.toObject();
                                                                        if(varNum==inpNum) {
                                                                            diInp.sysName = varOb["Name"].toString("");
                                                                            diInp.userName = varOb["Comment"].toString("");
                                                                            diInp.usedFlag = true;
                                                                        }
                                                                        varNum++;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }else {
                                    for(auto el:sysVarParGroupsArray) {
                                        QJsonObject elOb = el.toObject();
                                        if(elOb.contains("Name") && elOb["Name"].isString()) {
                                            if(QString::compare(elOb["Name"].toString(),"Аналоговые входы")) {
                                                if(elOb.contains("Groups") && elOb["Groups"].isArray()) {
                                                    QJsonArray aiVarsArray = elOb["Groups"].toArray();
                                                    for(auto gr:aiVarsArray) {
                                                        QJsonObject grOb = gr.toObject();
                                                        if(grOb.contains("Name") && grOb["Name"].isString()) {
                                                            QString grName = grOb["Name"].toString("");
                                                            if(QString::compare(grName,"состояние")) {
                                                                if(grOb.contains("Variables") && grOb["Variables"].isArray()) {
                                                                    QJsonArray varArray = grOb["Variables"].toArray();
                                                                    int varNum = 0;
                                                                    for(auto var:varArray) {
                                                                        QJsonObject varOb = var.toObject();
                                                                        if(varNum==inpNum) {
                                                                            anInp.sysName = varOb["Name"].toString("");
                                                                            anInp.userName = varOb["Comment"].toString("");

                                                                            if(inpNum<sensConnection.size()) {
                                                                                int sensNum = sensConnection.at(inpNum);
                                                                                if(sensNum && sensNum<=sensors.size()) {
                                                                                    Sensor s = sensors.at(sensNum-1);
                                                                                    anInp.sensor = s;
                                                                                    anInp.usedFlag = true;
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                if(inpNum<conf.dInput.size()) {
                                    conf.dInput[inpNum] = diInp;
                                }
                                if(inpNum<conf.anInput.size()) {
                                    conf.anInput[inpNum] = anInp;
                                }
                                inpNum++;
                            }
                        }

                        // read discrete outputs
                        int outNum = 0;
                        for(auto el:sysVarParGroupsArray) {
                            QJsonObject elOb = el.toObject();
                            if(elOb.contains("Name") && elOb["Name"].isString()) {
                                if(QString::compare(elOb["Name"].toString(),"Дискретные выходы")) {
                                    DiscreteOutput dOut;
                                    dOut.num = outNum+1;
                                    dOut.userName = "";
                                    dOut.sysName = "DO" + QString::number(outNum+1);
                                    if(elOb.contains("Groups") && elOb["Groups"].isArray()) {
                                        QJsonArray doVarsArray = elOb["Groups"].toArray();
                                        for(auto gr:doVarsArray) {
                                            QJsonObject grOb = gr.toObject();
                                            if(grOb.contains("Name") && grOb["Name"].isString()) {
                                                QString grName = grOb["Name"].toString("");
                                                if(QString::compare(grName,"состояние")) {
                                                    if(grOb.contains("Variables") && grOb["Variables"].isArray()) {
                                                        QJsonArray varArray = grOb["Variables"].toArray();
                                                        for(auto var:varArray) {
                                                            QJsonObject varOb = var.toObject();
                                                            dOut.sysName = varOb["Name"].toString("");
                                                            dOut.userName = varOb["Comment"].toString("");
                                                            if(outNum<conf.dOut.size()) {
                                                                conf.dOut[outNum] = dOut;
                                                            }
                                                            outNum++;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            file.close();
            return conf;
        }
        file.close();
    }
    return std::nullopt;
}
