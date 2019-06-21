#-------------------------------------------------
#
# Project created by QtCreator 2018-12-26T18:25:05
#
#-------------------------------------------------

QT       += core gui axcontainer

CONFIG += qaxcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChangeFileName
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ExcelBase.cpp \
    QVariantListModel.cpp \
    judgepic.cpp \
    pathsortfilter.cpp \
    judgename.cpp \
    helpdlg.cpp

HEADERS  += mainwindow.h \
    ExcelBase.h \
    QVariantListModel.h \
    judgepic.h \
    pathsortfilter.h \
    judgename.h \
    helpdlg.h

FORMS    += mainwindow.ui \
    helpdlg.ui

RC_ICONS = k3.ico

RESOURCES += \
    file.qrc
