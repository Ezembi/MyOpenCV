QT += core
QT += gui

CONFIG += c++14

TARGET = MyOpenCV
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    MyQImage.cpp \
    PyramidLevel.cpp \
    PyramidOctave.cpp \
    Pyramid.cpp \
    Descriptor.cpp \
    DescriptorFactory.cpp

HEADERS += \
    MyQImage.h \
    Assistant.h \
    GlobalNames.h \
    PyramidLevel.h \
    PyramidOctave.h \
    Pyramid.h \
    Descriptor.h \
    DescriptorFactory.h
