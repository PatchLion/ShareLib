TEMPLATE = lib
CONFIG += staticlib
TARGET = ShareLib

INCLUDEPATH += ../includes ../temp/GeneratedFiles

QT += core widgets

macx{
    LIBS += -framework Cocoa -framework WebKit

    HEADERS += ../src/mac/*.h

    OBJECTIVE_SOURCES += ../src/mac/*.mm

    OBJECTS_DIR = ../temp

    DESTDIR = ../lib/
}

win32{
    QT += webkitwidgets

    DESTDIR = ../lib/$(Platform)/$(Configuration)/
    OBJECTS_DIR = ../temp/$(Platform)/$(Configuration)/

    SOURCES += ../src/win/*.cpp
}

UI_DIR = ../temp/GeneratedFiles
RCC_DIR = ../temp/GeneratedFiles
MOC_DIR = ../temp/GeneratedFiles

#
FORMS += ../src/*.ui

# Input
HEADERS += ../includes/*.h \
           ../src/*.h \

#
SOURCES += ../src/*.cpp

#
RESOURCES += ../Resources/*.qrc
