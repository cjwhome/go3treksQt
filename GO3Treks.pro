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
    logger.cpp \
    loginwidget.cpp \
    serialwidget.cpp \
    ozonedatawidget.cpp \
    carbondatawidget.cpp \
    kmlmakerwidget.cpp \
    blogwidget.cpp

HEADERS  += mainwindow.h \
    logger.h \
    loginwidget.h \
    serialwidget.h \
    ozonedatawidget.h \
    carbondatawidget.h \
    kmlmakerwidget.h \
    blogwidget.h

FORMS    += \
    loginwidget.ui \
    serialwidget.ui \
    ozonedatawidget.ui \
    carbondatawidget.ui \
    kmlmakerwidget.ui \
    blogwidget.ui

OTHER_FILES +=

RESOURCES += \
    resedit.qrc
