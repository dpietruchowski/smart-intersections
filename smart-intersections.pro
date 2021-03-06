#-------------------------------------------------
#
# Project created by QtCreator 2019-08-19T23:21:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = smart-intersections
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        modes/EditIntersectionMode.cpp \
        modes/ModeFilter.cpp \
        scene/BaseItem.cpp \
        scene/CarItem.cpp \
        scene/CarPathItem.cpp \
        scene/IntersectionScene.cpp \
        main.cpp \
        MainWindow.cpp \
        ui/IntersectionKit.cpp \
        ui/IntersectionWidget.cpp

HEADERS += \
        modes/EditIntersectionMode.h \
        modes/ModeFilter.h \
        scene/BaseItem.h \
        scene/CarItem.h \
        scene/CarPathItem.h \
        scene/IntersectionScene.h \
        MainWindow.h \ \
        ui/IntersectionKit.h \
        ui/IntersectionWidget.h

FORMS += \
        MainWindow.ui \
        ui/IntersectionKit.ui \
        ui/IntersectionWidget.ui

RESOURCES += \
    icons.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
