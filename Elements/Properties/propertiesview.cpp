#include "propertiesview.h"
#include <QWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QColorDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include "../Widgets/lampimages.h"
#include "../Widgets/scalimages.h"
#include "Elements/Widgets/rectitem.h"
#include "plcconfig.h"
#include "typical_fonts.h"

void PropertiesView::clearLayout(QLayout *l)
{
    QLayoutItem *item;
    while(1) {
        if(!l->isEmpty()) {
            item = l->takeAt(0);
            if(item) {
                if (item->layout()) {
                    clearLayout(item->layout());
                }
                if (item->widget()) {
                   delete item->widget();
                }
                l->removeItem(item);
                delete item;
            }else break;
        }else break;
    }
}

QString PropertiesView::getStringFromProperty(ElProperty pr)
{
    if(pr.getType()==ElProperty::Type::STRING_T) {
        auto prVal = pr.getValue();
        if(auto val = std::get_if<QString>(&prVal)) {
            return *val;
        }
    }
    return "";
}

int PropertiesView::getIntFromProperty(ElProperty pr)
{
    if(pr.getType()==ElProperty::Type::INT_T) {
        auto prVal = pr.getValue();
        if(auto val = std::get_if<int>(&prVal)) {
            return *val;
        }
    }
    return 0;
}

bool PropertiesView::getBoolFromProperty(ElProperty pr)
{
    if(pr.getType()==ElProperty::Type::BOOL_T) {
        auto prVal = pr.getValue();
        if(auto val = std::get_if<bool>(&prVal)) {
            return *val;
        }
    }
    return false;
}

PropertiesView::PropertiesView(QObject *parent) : QObject(parent)
{
    layout = new QVBoxLayout();
}

void PropertiesView::setProperties(const std::vector<ElProperty> &properties)
{
    Q_UNUSED(properties)

    QString textColor = "QLabel { color : white}";
    QString spinHeight = "QSpinBox { height : 24}";
    int textSize = 12;

    clearLayout(layout);

    layout->addSpacing(50);
    QFormLayout *fLayout = new QFormLayout();
    ViewSpinbox *spBox = new ViewSpinbox();
    spBox->setStyleSheet(spinHeight);
    spBox->setRange(-10000,10000);
    connect(spBox,&ViewSpinbox::valChanged,this,&PropertiesView::update_x);
    QLabel *wName = new QLabel("x");
    wName->setStyleSheet(textColor);
    wName->setFont(QFont("Times", textSize, QFont::Bold));
    wName->setAlignment(Qt::AlignVCenter);
    fLayout->addRow(wName,spBox);
    widgets["x"] = spBox;

    spBox = new ViewSpinbox();
    spBox->setStyleSheet(spinHeight);
    spBox->setRange(-10000,10000);
    connect(spBox,&ViewSpinbox::valChanged,this,&PropertiesView::update_y);
    wName = new QLabel("y");
    wName->setStyleSheet(textColor);
    wName->setFont(QFont("Times", textSize, QFont::Bold));
    wName->setAlignment(Qt::AlignVCenter);
    fLayout->addRow(wName,spBox);
    widgets["y"] = spBox;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="change";});
    if(it!=properties.end()) {
        int val = getIntFromProperty(*it);
        ChangeMode chMode = static_cast<ChangeMode>(val);

        if(chMode!=ChangeMode::NoChange) {
            spBox = new ViewSpinbox();
            spBox->setStyleSheet(spinHeight);
            spBox->setRange(25,800);
            connect(spBox,&ViewSpinbox::valChanged,this,&PropertiesView::update_width);
            if(chMode==ChangeMode::Proportional) wName = new QLabel("размер");
            else wName = new QLabel("ширина");
            wName->setStyleSheet(textColor);
            wName->setFont(QFont("Times", textSize, QFont::Bold));
            wName->setAlignment(Qt::AlignVCenter);
            fLayout->addRow(wName,spBox);
            widgets["width"] = spBox;
        }

        if(chMode==ChangeMode::WidthAndHeight) {
            spBox = new ViewSpinbox();
            spBox->setStyleSheet(spinHeight);
            spBox->setRange(25,480);
            connect(spBox,&ViewSpinbox::valChanged,this,&PropertiesView::update_height);
            wName = new QLabel("высота");
            wName->setStyleSheet(textColor);
            wName->setFont(QFont("Times", textSize, QFont::Bold));
            wName->setAlignment(Qt::AlignVCenter);
            fLayout->addRow(wName,spBox);
            widgets["height"] = spBox;
        }
    }

    spBox = new ViewSpinbox();
    spBox->setStyleSheet(spinHeight);
    spBox->setRange(0,50);
    connect(spBox,&ViewSpinbox::valChanged,this,&PropertiesView::update_line_width);
    wName = new QLabel("толщина");
    wName->setStyleSheet(textColor);
    wName->setFont(QFont("Times", textSize, QFont::Bold));
    wName->setAlignment(Qt::AlignVCenter);
    fLayout->addRow(wName,spBox);
    widgets["line_width"] = spBox;

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="line_color";});
    if(it!=properties.end()) { // цвет линии

        fLayout->addRow(new QLabel(), new QLabel());
        QLabel *wName = new QLabel("цвет");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        QString col = getStringFromProperty(*it);
        bool res = false;
        long rgb = col.toLong(&res,16);
        int r_color = (rgb>>16) & 0xFF;
        int g_color = (rgb>>8) & 0xFF;
        int b_color = rgb & 0xFF;
        QPushButton *colButton = new QPushButton("...");
        colButton->setFlat(true);
        QPalette pal = colButton->palette();
        pal.setColor(QPalette::Button, QColor(r_color,g_color,b_color));
        colButton->setAutoFillBackground(true);
        colButton->setPalette(pal);
        connect(colButton,&QPushButton::clicked,[this,r_color,g_color,b_color,colButton](){
            QColor color = QColorDialog::getColor(QColor(r_color,g_color,b_color));
            if(color.isValid()) {
                QPalette pal = colButton->palette();
                pal.setColor(QPalette::Button, QColor(color));
                colButton->setAutoFillBackground(true);
                colButton->setPalette(pal);
                ElProperty pr("line_color",ElProperty::Type::STRING_T);
                QString colStr = color.name(QColor::HexRgb);
                colStr = colStr.mid(1,6);
                pr.setValue(colStr);
                emit updateProperty(pr);
            }
        });
        fLayout->addRow(wName,colButton);
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="on_color";});
    if(it!=properties.end()) {
        QLabel *wName = new QLabel("цвет вкл");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        QString col = getStringFromProperty(*it);
        bool res = false;
        long rgb = col.toLong(&res,16);
        int r_color = (rgb>>16) & 0xFF;
        int g_color = (rgb>>8) & 0xFF;
        int b_color = rgb & 0xFF;
        QPushButton *colButton = new QPushButton("...");
        colButton->setFlat(true);
        QPalette pal = colButton->palette();
        pal.setColor(QPalette::Button, QColor(r_color,g_color,b_color));
        colButton->setAutoFillBackground(true);
        colButton->setPalette(pal);
        connect(colButton,&QPushButton::clicked,[this,r_color,g_color,b_color,colButton](){
            QColor color = QColorDialog::getColor(QColor(r_color,g_color,b_color));
            if(color.isValid()) {
                QPalette pal = colButton->palette();
                pal.setColor(QPalette::Button, QColor(color));
                colButton->setAutoFillBackground(true);
                colButton->setPalette(pal);
                ElProperty pr("on_color",ElProperty::Type::STRING_T);
                QString colStr = color.name(QColor::HexRgb);
                colStr = colStr.mid(1,6);
                pr.setValue(colStr);
                emit updateProperty(pr);
            }
        });
        fLayout->addRow(wName,colButton);
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="off_color";});
    if(it!=properties.end()) {
        QLabel *wName = new QLabel("цвет выкл");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        QString col = getStringFromProperty(*it);
        bool res = false;
        long rgb = col.toLong(&res,16);
        int r_color = (rgb>>16) & 0xFF;
        int g_color = (rgb>>8) & 0xFF;
        int b_color = rgb & 0xFF;
        QPushButton *colButton = new QPushButton("...");
        colButton->setFlat(true);
        QPalette pal = colButton->palette();
        pal.setColor(QPalette::Button, QColor(r_color,g_color,b_color));
        colButton->setAutoFillBackground(true);
        colButton->setPalette(pal);
        connect(colButton,&QPushButton::clicked,[this,r_color,g_color,b_color,colButton](){
            QColor color = QColorDialog::getColor(QColor(r_color,g_color,b_color));
            if(color.isValid()) {
                QPalette pal = colButton->palette();
                pal.setColor(QPalette::Button, QColor(color));
                colButton->setAutoFillBackground(true);
                colButton->setPalette(pal);
                ElProperty pr("off_color",ElProperty::Type::STRING_T);
                QString colStr = color.name(QColor::HexRgb);
                colStr = colStr.mid(1,6);
                pr.setValue(colStr);
                emit updateProperty(pr);
            }
        });
        fLayout->addRow(wName,colButton);
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_bool_type";});
    if(it!=properties.end()) {

        QComboBox *varTypeBox = new QComboBox();
        varTypeBox->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::DI));
        varTypeBox->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::DO));
        varTypeBox->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::CLUSTER_BIT));
        varTypeBox->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::NET_BIT));

        int index = getIntFromProperty(*it);
        if(index>=0 && index<varTypeBox->count()) varTypeBox->setCurrentIndex(index);

        connect(varTypeBox,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
            ElProperty pr("link_bool_type",ElProperty::Type::INT_T);
            if(index>=0) {
                pr.setValue(index);
                emit updateProperty(pr);
            }
        });

        widgets["bool_type"] = varTypeBox;

        QLabel *wName = new QLabel("тип переменной");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);
        fLayout->addRow(wName,varTypeBox);
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_bool_index";});
    if(it!=properties.end()) {

        auto widgIt = widgets.find("bool_type");
        if(widgIt!=widgets.end()) {
            QComboBox *comboBoxVarType = dynamic_cast<QComboBox*>(widgIt->second);
            if(comboBoxVarType) {
                DiscreteVarType varType = SysVar::getDiscreteVarTypeFromString(comboBoxVarType->currentText());
                std::vector<Var> vars = plc.getDiscreteVarByType(varType);
                QComboBox *varNameBox = new QComboBox();
                for(const auto &v:vars) {
                    QString varName = v.sysName;
                    if(!v.userName.isEmpty()) {
                        varName += " (" + v.userName + ")";
                    }
                    varNameBox->addItem(varName);
                }

                int index = getIntFromProperty(*it);
                if(index>=0 && index<varNameBox->count()) varNameBox->setCurrentIndex(index);

                connect(varNameBox,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
                    ElProperty pr("link_bool_index",ElProperty::Type::INT_T);
                    if(index>=0) {
                        pr.setValue(index);
                        emit updateProperty(pr);
                    }
                });
                connect(comboBoxVarType,QOverload<int>::of(&QComboBox::currentIndexChanged),[this, properties](int index){
                    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_bool_type";});
                    if(it!=properties.end()) {
                        auto widgIt = widgets.find("bool_type");
                        if(widgIt!=widgets.end()) {
                            QComboBox *comboBoxVarType = dynamic_cast<QComboBox*>(widgIt->second);
                            if(comboBoxVarType) {
                                DiscreteVarType varType = SysVar::getDiscreteVarTypeFromString(comboBoxVarType->currentText());
                                it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_bool_index";});
                                if(it!=properties.end()) {
                                    auto widgNameIt = widgets.find("bool_index");
                                    if(widgNameIt!=widgets.end()) {
                                        QComboBox *comboBoxVarName = dynamic_cast<QComboBox*>(widgNameIt->second);
                                        if(comboBoxVarName) {
                                            std::vector<Var> vars = plc.getDiscreteVarByType(varType);
                                            comboBoxVarName->clear();
                                            for(const auto &v:vars) {
                                                QString varName = v.sysName;
                                                if(!v.userName.isEmpty()) {
                                                    varName += " (" + v.userName + ")";
                                                }
                                                comboBoxVarName->addItem(varName);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                });

                widgets["bool_index"] = varNameBox;

                QLabel *wName = new QLabel("переменная");
                wName->setStyleSheet(textColor);
                wName->setFont(QFont("Times", textSize, QFont::Bold));
                wName->setAlignment(Qt::AlignVCenter);
                fLayout->addRow(wName,varNameBox);
            }
        }
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="fill";});
    if(it!=properties.end()) { // заливка
        QLabel *wName = new QLabel("заливка");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        bool fill = getBoolFromProperty(*it);
        QCheckBox *check = new QCheckBox();
        check->setChecked(fill);
        connect(check,&QCheckBox::stateChanged,[this](int state){
            ElProperty pr("fill",ElProperty::Type::BOOL_T);
            bool fill = false;
            if(state==Qt::Checked) fill=true;
            pr.setValue(fill);
            emit updateProperty(pr);
        });

        fLayout->addRow(wName,check);
        widgets["rect_radius"] = spBox;
    }


    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="text_value";});
    if(it!=properties.end()) { // текст
        fLayout->addRow(new QLabel(), new QLabel());
        QLabel *wName = new QLabel("текст");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        QLineEdit *textWidget = new QLineEdit(getStringFromProperty(*it));
        connect(textWidget,&QLineEdit::returnPressed,[this,textWidget](){
            ElProperty pr("text_value",ElProperty::Type::STRING_T);
            pr.setValue(textWidget->text());
            emit updateProperty(pr);
        });
        fLayout->addRow(wName,textWidget);
        widgets["text_value"] = textWidget;
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="eng_font_index";});
    if(it!=properties.end()) { // стандартный шртфт латиница
        QLabel *wName = new QLabel("шрифт");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        int f = getIntFromProperty(*it);

        QComboBox *fontHeightBox = new QComboBox();
        for(int i=0;i<static_cast<int>(EngFonts::LastEl);i++) {
            int height = getEngFontHeight(static_cast<EngFonts>(i));
            fontHeightBox->addItem(QString::number(height));
        }

        if(f>=0 && f<fontHeightBox->count()) {
            fontHeightBox->setCurrentIndex(f);
        }

        connect(fontHeightBox,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
            ElProperty pr("eng_font_index",ElProperty::Type::INT_T);
            if(index>=0) {
                pr.setValue(index);
                emit updateProperty(pr);
            }
        });

        fLayout->addRow(wName,fontHeightBox);
        widgets["font_size"] = spBox;
    }



    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="font_size";});
    if(it!=properties.end()) { // размер шрифта
        QLabel *wName = new QLabel("шрифт");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        int fs = getIntFromProperty(*it);
        spBox = new ViewSpinbox();
        spBox->setStyleSheet(spinHeight);
        spBox->setRange(8,50);
        spBox->setValue(fs);
        connect(spBox,&ViewSpinbox::valChanged,[this,spBox](){
            ElProperty pr("font_size",ElProperty::Type::INT_T);
            int fs = spBox->value();
            pr.setValue(fs);
            emit updateProperty(pr);
        });

        fLayout->addRow(wName,spBox);
        widgets["font_size"] = spBox;
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="text_align";});
    if(it!=properties.end()) { // выравнивание
        QLabel *wName = new QLabel("выравн.");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        int fs = getIntFromProperty(*it);
        QComboBox *box = new QComboBox();
        box->setFont(QFont("Times",textSize-1));
        box->addItems(QStringList() << "слева" << "справа" << "по центру");
        if(fs>=0 && fs<=2) box->setCurrentIndex(fs);
        connect(box,QOverload<int>::of(&QComboBox::currentIndexChanged),[this,box](){
            ElProperty pr("text_align",ElProperty::Type::INT_T);
            pr.setValue(box->currentIndex());
            emit updateProperty(pr);
        });
        fLayout->addRow(wName,box);
        widgets["font_size"] = box;
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_state";});
    if(it!=properties.end()) { // состояние лампы
        fLayout->addRow(new QLabel(), new QLabel());
        QLabel *wName = new QLabel("вкл");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        bool ls = getBoolFromProperty(*it);

        QHBoxLayout *l = new QHBoxLayout();
        QCheckBox *check = new QCheckBox();
        check->setChecked(ls);
        connect(check,&QCheckBox::stateChanged,[this](int state){
            ElProperty pr("lamp_state",ElProperty::Type::BOOL_T);
            bool lampState = false;
            if(state==Qt::Checked) lampState=true;
            pr.setValue(lampState);
            emit updateProperty(pr);
        });
        l->addWidget(wName);
        l->addWidget(check);
        l->addWidget(new QLabel());

        //fLayout->addRow(wName,check);
        fLayout->addRow(l);
        widgets["lamp_state"] = check;
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_on_index";});
    if(it!=properties.end()) { // иконка для вкл состояния
        QLabel *wName = new QLabel("вкл");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        int index = getIntFromProperty(*it);
        if(index<0 || index>=LampImages::getQuantity()) index = 0;

        QHBoxLayout *l = new QHBoxLayout();
        l->addWidget(wName);

        QPushButton *b = new QPushButton();
        b->setIcon(QIcon(LampImages::getImage(index)));
        b->setIconSize(QSize(24,24));

        spBox = new ViewSpinbox();
        spBox->setStyleSheet(spinHeight);
        spBox->setRange(0,LampImages::getQuantity()-1);
        spBox->setValue(index);
        connect(spBox,&ViewSpinbox::valChanged,[this,spBox,b](){
            ElProperty pr("lamp_on_index",ElProperty::Type::INT_T);
            int index = spBox->value();
            b->setIcon(QIcon(LampImages::getImage(index)));
            pr.setValue(index);
            emit updateProperty(pr);
        });
        l->addWidget(spBox);
        l->addWidget(b);

        //fLayout->addRow(wName,spBox);
        fLayout->addRow(l);
        widgets["lamp_on_index"] = spBox;
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_off_index";});
    if(it!=properties.end()) { // иконка для выкл состояния
        QLabel *wName = new QLabel("выкл");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        int index = getIntFromProperty(*it);
        if(index<0 || index>=LampImages::getQuantity()) index = 0;

        QHBoxLayout *l = new QHBoxLayout();
        l->addWidget(wName);

        QPushButton *b = new QPushButton();
        b->setIcon(QIcon(LampImages::getImage(index)));
        b->setIconSize(QSize(24,24));

        spBox = new ViewSpinbox();
        spBox->setStyleSheet(spinHeight);
        spBox->setRange(0,LampImages::getQuantity()-1);
        spBox->setValue(index);
        //spBox->set
        connect(spBox,&ViewSpinbox::valChanged,[this,spBox,b](){
            ElProperty pr("lamp_off_index",ElProperty::Type::INT_T);
            int index = spBox->value();
            b->setIcon(QIcon(LampImages::getImage(index)));
            pr.setValue(index);
            emit updateProperty(pr);
        });
        l->addWidget(spBox);
        l->addWidget(b);

        //fLayout->addRow(wName,spBox);
        fLayout->addRow(l);
        widgets["lamp_off_index"] = spBox;
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="dig_count";});
    if(it!=properties.end()) { // число символов
        auto it2 = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="after_point";});
        if(it2!=properties.end()) {
            fLayout->addRow(new QLabel(), new QLabel());
            QLabel *wNameType = new QLabel("число знакомест");
            wNameType->setStyleSheet(textColor);
            wNameType->setFont(QFont("Times", textSize, QFont::Bold));
            wNameType->setAlignment(Qt::AlignVCenter);

            QLabel *wNameIndex = new QLabel("после точки");
            wNameIndex->setStyleSheet(textColor);
            wNameIndex->setFont(QFont("Times", textSize, QFont::Bold));
            wNameIndex->setAlignment(Qt::AlignVCenter);

            QSpinBox *spBox1 = new QSpinBox();
            spBox1->setFont(QFont("Times", textSize-2));
            spBox1->setRange(1,7);
            int cnt = getIntFromProperty(*it);
            spBox1->setValue(cnt);

            QSpinBox *spBox2 = new QSpinBox();
            spBox2->setFont(QFont("Times", textSize-2));
            spBox2->setRange(0,3);
            int afterPoint = getIntFromProperty(*it2);
            spBox2->setValue(afterPoint);

            connect(spBox1,QOverload<int>::of(&QSpinBox::valueChanged),[this,spBox1,spBox2](){
                ElProperty pr("dig_count",ElProperty::Type::INT_T);
                int cnt = spBox1->value();
                pr.setValue(cnt);
                emit updateProperty(pr);
                if(spBox2->value()>cnt) spBox2->setValue(cnt);
                else if(spBox2->value()==cnt) spBox2->setValue(cnt-1);
            });

            connect(spBox2,QOverload<int>::of(&QSpinBox::valueChanged),[this,spBox1,spBox2](){

                ElProperty pr("after_point",ElProperty::Type::INT_T);
                int afterPoint = spBox2->value();
                if(spBox1->value()==afterPoint) {
                    afterPoint--;
                    spBox2->setValue(afterPoint);
                }
                pr.setValue(afterPoint);
                emit updateProperty(pr);
            });

            fLayout->addRow(wNameType,spBox1);
            fLayout->addRow(wNameIndex,spBox2);
            widgets["dig_count"] = spBox1;
            widgets["after_point"] = spBox2;
        }
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="num_var_type";});
    if(it!=properties.end()) { // тип привязки
        auto it2 = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="num_var_index";});
        if(it2!=properties.end()) {
            fLayout->addRow(new QLabel(), new QLabel());
            QLabel *wNameType = new QLabel("привязка");
            wNameType->setStyleSheet(textColor);
            wNameType->setFont(QFont("Times", textSize, QFont::Bold));
            wNameType->setAlignment(Qt::AlignVCenter);

            QLabel *wNameIndex = new QLabel("номер");
            wNameIndex->setStyleSheet(textColor);
            wNameIndex->setFont(QFont("Times", textSize, QFont::Bold));
            wNameIndex->setAlignment(Qt::AlignVCenter);

            QComboBox *box = new QComboBox();
            box->setFont(QFont("Times", textSize-1, QFont::Bold));
            box->addItems(QStringList()<<"AI"<<"IBR"<<"IIR"<<"ILR");
            int vType = getIntFromProperty(*it);
            if(vType<0 || vType>=4) vType = 0;
            box->setCurrentIndex(vType);

            QSpinBox *spBox = new QSpinBox();
            spBox->setFont(QFont("Times", textSize-2));
            if(box->currentIndex()==0) spBox->setRange(1,98);
            else if(box->currentIndex()==1) spBox->setRange(1,384);
            else if(box->currentIndex()==2) spBox->setRange(1,224);
            else if(box->currentIndex()==3) spBox->setRange(1,24);
            int vIndex = getIntFromProperty(*it2);
            spBox->setValue(vIndex);

            connect(box,QOverload<int>::of(&QComboBox::currentIndexChanged),[this,box,spBox](){
                ElProperty pr("num_var_type",ElProperty::Type::INT_T);
                int index = box->currentIndex();
                pr.setValue(index);
                emit updateProperty(pr);
                if(index==0) spBox->setRange(1,98);
                else if(index==1) spBox->setRange(1,384);
                else if(index==2) spBox->setRange(1,224);
                else if(index==3) spBox->setRange(1,24);
            });

            connect(spBox,QOverload<int>::of(&QSpinBox::valueChanged),[this,spBox](){
                ElProperty pr("num_var_index",ElProperty::Type::INT_T);
                int index = spBox->value();
                pr.setValue(index);
                emit updateProperty(pr);
            });

            fLayout->addRow(wNameType,box);
            fLayout->addRow(wNameIndex,spBox);
            widgets["num_var_type"] = box;
            widgets["num_var_index"] = spBox;
        }
    }

    it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="image_index";});
    if(it!=properties.end()) { // scalable image
        QLabel *wName = new QLabel("изображение");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        int index = getIntFromProperty(*it);
        if(index<0 || index>=ScalImages::getQuantity()) index = 0;

        QHBoxLayout *l = new QHBoxLayout();
        l->addWidget(wName);

        QPushButton *b = new QPushButton();
        b->setIcon(QIcon(ScalImages::getImage(index)));
        b->setIconSize(QSize(24,24));

        spBox = new ViewSpinbox();
        spBox->setStyleSheet(spinHeight);
        spBox->setRange(0,ScalImages::getQuantity()-1);
        spBox->setValue(index);
        connect(spBox,&ViewSpinbox::valChanged,[this,spBox,b](){
            ElProperty pr("image_index",ElProperty::Type::INT_T);
            int index = spBox->value();
            b->setIcon(QIcon(ScalImages::getImage(index)));
            pr.setValue(index);
            emit updateProperty(pr);
        });
        l->addWidget(spBox);
        l->addWidget(b);

        fLayout->addRow(l);
        widgets["image_index"] = spBox;
    }

    layout->addLayout(fLayout);
}

void PropertiesView::updateRect(int x, int y, int w, int h, int lw)
{
    auto it = widgets.find("x");
    if(it!=widgets.end()) {
        QSpinBox *spBox = dynamic_cast<QSpinBox*>(it->second);
        if(spBox) spBox->setValue(x);
    }
    it = widgets.find("y");
    if(it!=widgets.end()) {
        QSpinBox *spBox = dynamic_cast<QSpinBox*>(it->second);
        if(spBox) spBox->setValue(y);
    }
    it = widgets.find("width");
    if(it!=widgets.end()) {
        QSpinBox *spBox = dynamic_cast<QSpinBox*>(it->second);
        if(spBox) spBox->setValue(w);
    }
    it = widgets.find("height");
    if(it!=widgets.end()) {
        QSpinBox *spBox = dynamic_cast<QSpinBox*>(it->second);
        if(spBox) spBox->setValue(h);
    }
    it = widgets.find("line_width");
    if(it!=widgets.end()) {
        QSpinBox *spBox = dynamic_cast<QSpinBox*>(it->second);
        if(spBox) spBox->setValue(lw);
    }
}

void PropertiesView::clearProperties()
{
    widgets.clear();
    clearLayout(layout);
}

void PropertiesView::setPLCConfig(const PLCConfig &plcConf)
{
    plc = plcConf;
}
