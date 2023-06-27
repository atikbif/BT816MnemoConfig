QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RC_ICONS = display.ico
VERSION = 1.0.0.0

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QMAKE_TARGET_COMPANY = Modern Technical Solutions Co
QMAKE_TARGET_PRODUCT = Display Editor 1.0
QMAKE_TARGET_DESCRIPTION = Display Editor
QMAKE_TARGET_COPYRIGHT = Modern Technical Solutions

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Elements/Properties/elproperty.cpp \
    Elements/Properties/filledrectpropertiespattern.cpp \
    Elements/Properties/propertiesview.cpp \
    Elements/Widgets/circleitem.cpp \
    Elements/Widgets/filledcircleitem.cpp \
    Elements/Widgets/filledrectitem.cpp \
    Elements/Widgets/lampimages.cpp \
    Elements/Widgets/lampitem.cpp \
    Elements/Widgets/numberitem.cpp \
    Elements/Widgets/rectitem.cpp \
    Elements/Widgets/scalableimageitem.cpp \
    Elements/Widgets/scalimages.cpp \
    Elements/Widgets/textitem.cpp \
    dialogaddeditablevar.cpp \
    dialogprojectconfig.cpp \
    displayscene.cpp \
    editablesysvariables.cpp \
    jsonplcconfigreader.cpp \
    main.cpp \
    mainwindow.cpp \
    plcconfig.cpp \
    typical_fonts.cpp

HEADERS += \
    Elements/Properties/elproperty.h \
    Elements/Properties/filledrectpropertiespattern.h \
    Elements/Properties/propertiesview.h \
    Elements/Widgets/circleitem.h \
    Elements/Widgets/colorvalue.h \
    Elements/Widgets/filledcircleitem.h \
    Elements/Widgets/filledrectitem.h \
    Elements/Widgets/lampimages.h \
    Elements/Widgets/lampitem.h \
    Elements/Widgets/movepoint.h \
    Elements/Widgets/numberitem.h \
    Elements/Widgets/rectitem.h \
    Elements/Widgets/scalableimageitem.h \
    Elements/Widgets/scalimages.h \
    Elements/Widgets/selectionmode.h \
    Elements/Widgets/textitem.h \
    dialogaddeditablevar.h \
    dialogprojectconfig.h \
    displayscene.h \
    editablesysvariables.h \
    jsonplcconfigreader.h \
    mainwindow.h \
    plcconfig.h \
    plcconfigreader.h \
    typical_fonts.h

FORMS += \
    dialogaddeditablevar.ui \
    dialogprojectconfig.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
