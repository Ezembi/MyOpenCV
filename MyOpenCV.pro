QT += core
QT += gui

CONFIG += c++14

TARGET = MyOpenCV
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    MyQImage.cpp

HEADERS += \
    MyQImage.h \
    Assistant.h \
    lab_2.h \
    GlobalNames.h
