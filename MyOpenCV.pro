QT += core
QT += gui

CONFIG += c++14

TARGET = MyOpenCV
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    MyPicture.cpp \
    MyQImage.cpp

HEADERS += \
    MyPicture.h \
    MyQImage.h \
    Defines.h
