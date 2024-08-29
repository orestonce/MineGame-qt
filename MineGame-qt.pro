#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T21:11:10
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MineGame-qt
TEMPLATE = app


SOURCES += main.cpp\
    MineGame.cpp \
    MineGameWindow.cpp

HEADERS  += \
    MineGame.h \
    MineGameWindow.h

FORMS    += \
    MineGameWindow.ui
QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += \
    mine.rc

RC_FILE = mine.rc

RESOURCES += \
    image.qrc

