TEMPLATE = app
TARGET = ShareTest

win32{
    DESTDIR = ../bin/$(Platform)/$(Configuration)/
    LIBS += -L../lib/$(Platform)/$(Configuration)/

    QT += webkitwidgets
}

macx{
    LIBS += -L../lib/
    LIBS += -framework Cocoa -framework WebKit
}
QT += core gui script widgets network

LIBS += -lShareLib

UI_DIR = ../temp/GeneratedFiles
RCC_DIR = ../temp/GeneratedFiles
MOC_DIR = ../temp/GeneratedFiles

INCLUDEPATH += ../temp/GeneratedFiles ../includes

HEADERS += ./*.h
SOURCES += ./*.cpp
FORMS += ./*.ui
RESOURCES += ./*.qrc
