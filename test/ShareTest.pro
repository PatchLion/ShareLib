TEMPLATE = app
TARGET = ShareTest
DESTDIR = ../bin
QT += core gui script widgets network

LIBS += -lShareLib -L../lib/mac
LIBS += -framework Cocoa -framework WebKit

CONFIG += release
INCLUDEPATH += ./GeneratedFiles . ./../Include
DEPENDPATH += .

MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += ./obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

HEADERS += ./ShareTest.h
SOURCES += ./main.cpp ./ShareTest.cpp
FORMS += ./ShareTest.ui
RESOURCES += ./ShareTest.qrc
