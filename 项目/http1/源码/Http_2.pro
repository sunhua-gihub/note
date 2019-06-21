#-------------------------------------------------
#
# Project created by QtCreator 2019-03-19T09:25:49
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Http_2
TEMPLATE = app


SOURCES += main.cpp\
        mainui.cpp \
    linkagewidget.cpp

HEADERS  += mainui.h \
    linkagewidget.h

FORMS    += mainui.ui \
    linkagewidget.ui

RESOURCES += \
    icon.qrc \
    qss.qrc
