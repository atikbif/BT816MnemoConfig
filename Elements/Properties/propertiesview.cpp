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
#include <QRegExp>
#include "../Widgets/numberitem.h"

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

void PropertiesView::drawXYProperties(QFormLayout *fLayout)
{
    QString textColor = "QLabel { color : white}";
    QString spinHeight = "QSpinBox { height : 24}";
    int textSize = 12;

    layout->addSpacing(50);
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
}

void PropertiesView::drawWidthHeightProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    QString spinHeight = "QSpinBox { height : 24}";
    int textSize = 12;

    QLabel *wName = nullptr;
    ViewSpinbox *spBox = nullptr;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="change";});
    if(it!=properties.end()) {
        int val = ElProperty::getIntFromProperty(*it);
        ChangeMode chMode = static_cast<ChangeMode>(val);
        spBox = new ViewSpinbox();
        spBox->setStyleSheet(spinHeight);
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
}

void PropertiesView::drawColorProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="line_color";});
    if(it!=properties.end()) { // цвет линии

        fLayout->addRow(new QLabel(), new QLabel());
        QLabel *wName = new QLabel("цвет");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        QString col = ElProperty::getStringFromProperty(*it);
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
}

void PropertiesView::drawOnColorProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="on_color";});
    if(it!=properties.end()) {
        QLabel *wName = new QLabel("цвет вкл");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        QString col = ElProperty::getStringFromProperty(*it);
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
}

void PropertiesView::drawOffColorProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="off_color";});
    if(it!=properties.end()) {
        QLabel *wName = new QLabel("цвет выкл");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        QString col = ElProperty::getStringFromProperty(*it);
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
}

void PropertiesView::drawLinkBoolTypeProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_bool_type";});
    if(it!=properties.end()) {

        QComboBox *varTypeBox = new QComboBox();
        varTypeBox->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::DI));
        varTypeBox->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::DO));
        varTypeBox->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::CLUSTER_BIT));
        varTypeBox->addItem(SysVar::getDiscreteVarTypeString(DiscreteVarType::NET_BIT));

        int index = ElProperty::getIntFromProperty(*it);
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
}

void PropertiesView::drawLinkBoolIndexProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_bool_index";});
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

                int index = ElProperty::getIntFromProperty(*it);
                if(index>=0 && index<varNameBox->count()) varNameBox->setCurrentIndex(index);

                connect(varNameBox,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
                    ElProperty pr("link_bool_index",ElProperty::Type::INT_T);
                    if(index>=0) {
                        pr.setValue(index);
                        emit updateProperty(pr);
                    }
                });
                connect(comboBoxVarType,QOverload<int>::of(&QComboBox::currentIndexChanged),[this, properties](int index){
                    Q_UNUSED(index)
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
}

void PropertiesView::drawTextValueProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";

    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="text_value";});
    if(it!=properties.end()) { // текст
        fLayout->addRow(new QLabel(), new QLabel());
        QLabel *wName = new QLabel("текст");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        QLineEdit *textWidget = new QLineEdit(ElProperty::getStringFromProperty(*it));

        connect(textWidget,&QLineEdit::textChanged,[this](const QString &val){
            ElProperty pr("text_value",ElProperty::Type::STRING_T);
            pr.setValue(val);
            emit updateProperty(pr);
        });
        fLayout->addRow(wName,textWidget);
        widgets["text_value"] = textWidget;
    }
}

void PropertiesView::drawEngFontProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="eng_font_index";});
    if(it!=properties.end()) { // стандартный шрифт латиница
        QLabel *wName = new QLabel("шрифт");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        int f = ElProperty::getIntFromProperty(*it);

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
        widgets["eng_font_index"] = fontHeightBox;
    }
}

void PropertiesView::drawNumPatternProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="num_pattern";});
    if(it!=properties.end()) { // пример значения числа
        QLabel *wName = new QLabel("значение");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        QLineEdit *numValuePattern = new QLineEdit();

        numValuePattern->setText(ElProperty::getStringFromProperty(*it));

        connect(numValuePattern,&QLineEdit::textChanged,[this,numValuePattern](const QString &val){
            QString res = val;
            res.remove(QRegExp("[A-Za-zА-Яа-я]"));
            numValuePattern->setText(res);
            ElProperty pr("num_pattern",ElProperty::Type::STRING_T);
            pr.setValue(res);
            emit updateProperty(pr);
        });

        fLayout->addRow(wName,numValuePattern);
        widgets["num_pattern"] = numValuePattern;
    }
}

void PropertiesView::drawNumDividerProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="num_div";});
    if(it!=properties.end()) { // делитель числа
        QLabel *wName = new QLabel("делитель");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        QComboBox *dividerBox = new QComboBox();
        dividerBox->addItem("1");
        dividerBox->addItem("10");
        dividerBox->addItem("100");
        dividerBox->addItem("1000");

        int v = ElProperty::getIntFromProperty(*it);
        if((v<0) && (v>static_cast<int>(NumberDivider::Div1000))) v=0;

        dividerBox->setCurrentIndex(v);


        connect(dividerBox,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
            ElProperty pr("num_div",ElProperty::Type::INT_T);
            if(index>=0) {
                pr.setValue(index);
                emit updateProperty(pr);
            }
        });

        fLayout->addRow(wName,dividerBox);
        widgets["num_div"] = dividerBox;
    }
}

void PropertiesView::drawLinkAnalogueTypeProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_analogue_type";});
    if(it!=properties.end()) {

        QComboBox *varTypeBox = new QComboBox();
        varTypeBox->addItem(SysVar::getAnalogueVarTypeString(AnalogueVarType::RAW_AI));
        varTypeBox->addItem(SysVar::getAnalogueVarTypeString(AnalogueVarType::CALCULATED_AI));
        varTypeBox->addItem(SysVar::getAnalogueVarTypeString(AnalogueVarType::CLUSTER_REG));
        varTypeBox->addItem(SysVar::getAnalogueVarTypeString(AnalogueVarType::NET_REG));

        int index = ElProperty::getIntFromProperty(*it);
        if(index>=0 && index<varTypeBox->count()) varTypeBox->setCurrentIndex(index);

        connect(varTypeBox,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
            ElProperty pr("link_analogue_type",ElProperty::Type::INT_T);
            if(index>=0) {
                pr.setValue(index);
                emit updateProperty(pr);
            }
        });

        widgets["link_analogue_type"] = varTypeBox;

        QLabel *wName = new QLabel("тип переменной");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);
        fLayout->addRow(wName,varTypeBox);
    }
}

void PropertiesView::drawLinkAnalogueIndexProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_analogue_index";});
    if(it!=properties.end()) {

        auto widgIt = widgets.find("link_analogue_type");
        if(widgIt!=widgets.end()) {
            QComboBox *comboBoxVarType = dynamic_cast<QComboBox*>(widgIt->second);
            if(comboBoxVarType) {
                AnalogueVarType varType = SysVar::getAnalogueVarTypeFromString(comboBoxVarType->currentText());
                std::vector<Var> vars = plc.getAnalogueVarByType(varType);
                QComboBox *varNameBox = new QComboBox();
                for(const auto &v:vars) {
                    QString varName = v.sysName;
                    if(!v.userName.isEmpty()) {
                        varName += " (" + v.userName + ")";
                    }
                    varNameBox->addItem(varName);
                }

                int index = ElProperty::getIntFromProperty(*it);
                if(index>=0 && index<varNameBox->count()) varNameBox->setCurrentIndex(index);

                connect(varNameBox,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
                    ElProperty pr("link_analogue_index",ElProperty::Type::INT_T);
                    if(index>=0) {
                        pr.setValue(index);
                        emit updateProperty(pr);
                    }
                });
                connect(comboBoxVarType,QOverload<int>::of(&QComboBox::currentIndexChanged),[this, properties](int index){
                    Q_UNUSED(index)
                    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_analogue_type";});
                    if(it!=properties.end()) {
                        auto widgIt = widgets.find("link_analogue_type");
                        if(widgIt!=widgets.end()) {
                            QComboBox *comboBoxVarType = dynamic_cast<QComboBox*>(widgIt->second);
                            if(comboBoxVarType) {
                                AnalogueVarType varType = SysVar::getAnalogueVarTypeFromString(comboBoxVarType->currentText());
                                it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="link_analogue_index";});
                                if(it!=properties.end()) {
                                    auto widgNameIt = widgets.find("link_analogue_index");
                                    if(widgNameIt!=widgets.end()) {
                                        QComboBox *comboBoxVarName = dynamic_cast<QComboBox*>(widgNameIt->second);
                                        if(comboBoxVarName) {
                                            std::vector<Var> vars = plc.getAnalogueVarByType(varType);
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

                widgets["link_analogue_index"] = varNameBox;

                QLabel *wName = new QLabel("переменная");
                wName->setStyleSheet(textColor);
                wName->setFont(QFont("Times", textSize, QFont::Bold));
                wName->setAlignment(Qt::AlignVCenter);
                fLayout->addRow(wName,varNameBox);
            }
        }
    }
}

void PropertiesView::drawCyrFontProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="cyr_font_index";});
    if(it!=properties.end()) { // стандартный шртфт кириллица
        QLabel *wName = new QLabel("шрифт");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        int f = ElProperty::getIntFromProperty(*it);

        QComboBox *fontHeightBox = new QComboBox();
        for(int i=0;i<static_cast<int>(CyrFonts::LastEl);i++) {
            int height = getCyrFontHeight(static_cast<CyrFonts>(i));
            fontHeightBox->addItem(QString::number(height));
        }

        if(f>=0 && f<fontHeightBox->count()) {
            fontHeightBox->setCurrentIndex(f);
        }

        connect(fontHeightBox,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
            ElProperty pr("cyr_font_index",ElProperty::Type::INT_T);
            if(index>=0) {
                pr.setValue(index);
                emit updateProperty(pr);
            }
        });

        fLayout->addRow(wName,fontHeightBox);
        widgets["cyr_font_index"] = fontHeightBox;
    }
}

void PropertiesView::drawLampStateProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    int textSize = 12;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_state";});
    if(it!=properties.end()) { // состояние лампы
        fLayout->addRow(new QLabel(), new QLabel());
        QLabel *wName = new QLabel("вкл");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        bool ls = ElProperty::getBoolFromProperty(*it);

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
}

void PropertiesView::drawLampOnIndexProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    QString spinHeight = "QSpinBox { height : 20}";
    int textSize = 12;

    ViewSpinbox *spBox = nullptr;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_on_index";});
    if(it!=properties.end()) { // иконка для вкл состояния
        QLabel *wName = new QLabel("вкл");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        int index = ElProperty::getIntFromProperty(*it);
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

        fLayout->addRow(l);
        widgets["lamp_on_index"] = spBox;
    }
}

void PropertiesView::drawLampOffIndexProperties(QFormLayout *fLayout, const std::vector<ElProperty> &properties)
{
    QString textColor = "QLabel { color : white}";
    QString spinHeight = "QSpinBox { height : 20}";
    int textSize = 12;

    ViewSpinbox *spBox = nullptr;

    auto it = std::find_if(properties.begin(),properties.end(),[](ElProperty pr){return pr.getName()=="lamp_off_index";});
    if(it!=properties.end()) { // иконка для выкл состояния
        QLabel *wName = new QLabel("выкл");
        wName->setStyleSheet(textColor);
        wName->setFont(QFont("Times", textSize, QFont::Bold));
        wName->setAlignment(Qt::AlignVCenter);

        int index = ElProperty::getIntFromProperty(*it);
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
}

PropertiesView::PropertiesView(QObject *parent) : QObject(parent)
{
    layout = new QVBoxLayout();
}

void PropertiesView::setProperties(const std::vector<ElProperty> &properties)
{
    clearLayout(layout);

    QFormLayout *fLayout = new QFormLayout();
    drawXYProperties(fLayout);
    drawWidthHeightProperties(fLayout,properties);

    //    spBox = new ViewSpinbox();
    //    spBox->setStyleSheet(spinHeight);
    //    spBox->setRange(0,50);
    //    connect(spBox,&ViewSpinbox::valChanged,this,&PropertiesView::update_line_width);
    //    wName = new QLabel("толщина");
    //    wName->setStyleSheet(textColor);
    //    wName->setFont(QFont("Times", textSize, QFont::Bold));
    //    wName->setAlignment(Qt::AlignVCenter);
    //    fLayout->addRow(wName,spBox);
    //    widgets["line_width"] = spBox;

    drawColorProperties(fLayout,properties);
    drawOnColorProperties(fLayout,properties);
    drawOffColorProperties(fLayout,properties);
    drawLinkBoolTypeProperties(fLayout,properties);
    drawLinkBoolIndexProperties(fLayout,properties);
    drawTextValueProperties(fLayout,properties);
    drawEngFontProperties(fLayout,properties);
    drawNumPatternProperties(fLayout,properties);
    drawNumDividerProperties(fLayout,properties);
    drawLinkAnalogueTypeProperties(fLayout,properties);
    drawLinkAnalogueIndexProperties(fLayout,properties);
    drawCyrFontProperties(fLayout,properties);
    drawLampStateProperties(fLayout,properties);
    drawLampOnIndexProperties(fLayout,properties);
    drawLampOffIndexProperties(fLayout,properties);

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
