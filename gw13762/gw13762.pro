#-------------------------------------------------
#
# Project created by QtCreator 2013-09-08T11:06:45
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gw13762
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    GDW1376_2.h

FORMS    += mainwindow.ui

RC_FILE += myico.rc

LIBS += ../build/GDW1376_2.dll

OTHER_FILES += \
    myico.rc

RESOURCES += \
    zk.qrc
