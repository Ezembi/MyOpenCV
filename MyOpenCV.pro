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
    DescriptorFactory.cpp \
    DirectLinerTransformation.cpp

HEADERS += \
    MyQImage.h \
    Assistant.h \
    GlobalNames.h \
    PyramidLevel.h \
    PyramidOctave.h \
    Pyramid.h \
    Descriptor.h \
    DescriptorFactory.h \
    DirectLinerTransformation.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Programs/GnuWin32/lib/ -lgsl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Programs/GnuWin32/lib/ -lgsld

INCLUDEPATH += $$PWD/../../../../Programs/GnuWin32/include
DEPENDPATH += $$PWD/../../../../Programs/GnuWin32/include
