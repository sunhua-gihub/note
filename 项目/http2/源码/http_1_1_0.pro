#-------------------------------------------------
#
# Project created by QtCreator 2019-05-27T09:20:35
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = http_1_1_0
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    httpmanage.cpp \
    filechoose.cpp \
    personmanage.cpp

HEADERS  += widget.h \
    httpmanage.h \
    filechoose.h \
    personmanage.h

FORMS    += widget.ui \
    filechoose.ui \
    personmanage.ui

RESOURCES += \
    qss.qrc
