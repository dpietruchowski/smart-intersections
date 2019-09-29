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
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
        agents/CarAgent.cpp \
        agents/IntersectionManager.cpp \
        items/CollisionAreaItem.cpp \
        main.cpp \
        MainWindow.cpp \
        items/BaseItem.cpp \
        items/CarItem.cpp \
        items/PathItem.cpp \
        scene/CollisionPath.cpp \
        scene/IntersectionScene.cpp \
        scene/PainterPath.cpp \
        scene/Route.cpp \
        ui/CarTable.cpp \
        ui/IntersectionWidget.cpp \
        ui/TimerWidget.cpp \
        ui/TimespanRegisterWidget.cpp \
        ui/TimespanWindows.cpp \
        utils/reverse.cpp

HEADERS += \
        MainWindow.h \
        agents/CarAgent.h \
        agents/IntersectionManager.h \
        items/BaseItem.h \
        items/CarItem.h \
        items/CollisionAreaItem.h \
        items/PathItem.h \
        scene/CollisionPath.h \
        scene/IntersectionScene.h \
        scene/PainterPath.h \
        scene/Route.h \
        ui/CarTable.h \
        ui/IntersectionWidget.h \
        ui/TimerWidget.h \
        ui/TimespanRegisterWidget.h \
        ui/TimespanWindows.h \
        utils/reverse.h

FORMS += \
        MainWindow.ui \
        ui/IntersectionWidget.ui \
        ui/TimerWidget.ui \
        ui/TimespanRegisterWidget.ui

INCLUDEPATH += \
        scene \
        items \
        agents \
        ui \
        utils

RESOURCES += \
    icons.qrc \
    intersections.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
