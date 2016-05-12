#-------------------------------------------------
#
# Project created by QtCreator 2016-05-01T12:11:50
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Uart_IIC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialthread.cpp

HEADERS  += mainwindow.h \
    serialthread.h

FORMS    += mainwindow.ui

