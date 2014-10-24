#-------------------------------------------------
#
# Project created by QtCreator 2014-10-21T22:05:57
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = VisualizationDataFromComPort
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SettingsDialog.cpp \
    Receiver.cpp \
    QCustomPlot.cpp

HEADERS  += MainWindow.h \
    SettingsDialog.h \
    Receiver.h \
    PortError.h \
    QCustomPlot.h

FORMS    += MainWindow.ui \
    SettingsDialog.ui
