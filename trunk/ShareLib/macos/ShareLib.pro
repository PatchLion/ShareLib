TEMPLATE = lib
CONFIG += staticlib
TARGET = ShareLib
INCLUDEPATH += ../Include

Debug: DESTDIR = ../lib/mac/debug
Release: DESTDIR = ../lib/mac/release

UI_DIR = ../temp/GeneratedFiles
RCC_DIR = ../temp/GeneratedFiles
MOC_DIR = ../temp/GeneratedFiles

# Input
HEADERS += ../Include/FacebookShare.h \
           ../Include/FlickrShare.h \
           ../Include/IWebviewContainer.h \
           ../Include/IWebviewStyle.h \
           ../Include/NetworkReplyTempObjManager.h \
           ../Include/ShareFrameBase.h \
           ../Include/ShareMacroDefine.h \
           ../Include/SharePublicFuncDefine.h \
           ../Include/ShareStructDefine.h \
           ../Include/TwitterShare.h \
           ../Include/WebviewContainer.h \
           ../src/AuthorizeWebviewBase.h \
           ../src/call_once.h \
           ../src/FaceBookAuthorizeWebview.h \
           ../src/FacebookShareObject.h \
           ../src/FlickrAuthorizeWebview.h \
           ../src/FlickrShareObject.h \
           ../src/qsingleton.h \
           ../src/TwitterAuthorizeWebview.h \
           ../src/TwitterShareObject.h

SOURCES += ../src/AuthorizeWebviewBase.cpp \
           ../src/FaceBookAuthorizeWebview.cpp \
           ../src/FacebookShare.cpp \
           ../src/FacebookShareObject.cpp \
           ../src/FlickrAuthorizeWebview.cpp \
           ../src/FlickrShare.cpp \
           ../src/FlickrShareObject.cpp \
           ../src/IWebviewStyle.cpp \
           ../src/NetworkReplyTempObjManager.cpp \
           ../src/ShareFrameBase.cpp \
           ../src/SharePublicFuncDefine.cpp \
           ../src/ShareStructDefine.cpp \
           ../src/TwitterAuthorizeWebview.cpp \
           ../src/TwitterShare.cpp \
           ../src/TwitterShareObject.cpp


 HEADERS += ../src/CNotifyWebView.h
 OBJECTIVE_SOURCES += ../src/WebviewContainer.mm \
               ../src/CNotifyWebView.mm

RESOURCES += ../shareimages.qrc
