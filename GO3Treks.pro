#-------------------------------------------------
#
# Project created by QtCreator 2014-06-05T16:49:55
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += serialport
TARGET = GO3Treks
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pomserialportwidget.cpp

HEADERS  += mainwindow.h \
    pomserialportwidget.h

FORMS    += mainwindow.ui \
    pomserialportwidget.ui

OTHER_FILES +=

RESOURCES += \
    resedit.qrc
