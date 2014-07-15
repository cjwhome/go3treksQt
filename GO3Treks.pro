#-------------------------------------------------
#
# Project created by QtCreator 2014-06-05T16:49:55
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network
CONFIG += serialport
TARGET = GO3Treks
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pomserialportwidget.cpp \
    logger.cpp \
    loginwidget.cpp \
    serialwidget.cpp

HEADERS  += mainwindow.h \
    pomserialportwidget.h \
    logger.h \
    loginwidget.h \
    serialwidget.h

FORMS    += pomserialportwidget.ui \
    serialstep.ui \
    loginwidget.ui \
    serialwidget.ui

OTHER_FILES +=

RESOURCES += \
    resedit.qrc
