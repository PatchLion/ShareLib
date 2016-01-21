#include "webviewinqt.h"
#include <WebKit/WebKit.h>
#include <Webkit/WebView.h>
#include <QDebug>
#include "customwebview.h"


WebViewInQt::WebViewInQt(QWidget *parent) :
    QMacCocoaViewContainer(0, parent)
{
    @autoreleasepool{

    NSRect rect = {{0, 0}, {parent->width(), parent->height()}};

    CustomWebView *cWebView = [[CustomWebView alloc]initWithObjects:rect frameName:nil groupName:nil target:this];
    setCocoaView(cWebView);

    [cWebView release];

    }
}

WebViewInQt::WebViewInQt(QString loadUrl, QWidget *parent) :
    QMacCocoaViewContainer(0, parent)
{
    @autoreleasepool{

    NSString *strUrl = loadUrl.toNSString();

    NSRect rect = {{0, 0}, {parent->width(), parent->height()}};

    CustomWebView *cWebView = [[CustomWebView alloc]initWithObjects:rect frameName:nil groupName:nil target:this];

    [[cWebView mainFrame] loadRequest: [NSURLRequest requestWithURL:[NSURL URLWithString:strUrl]
                                        cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:60]];
    setCocoaView(cWebView);

    [cWebView release];

    }

}


void WebViewInQt::loadRequest(const QString& loadUrl)
{
    @autoreleasepool{

     CustomWebView *cWebView = this->cocoaView();
     //NSString *strUrl = loadUrl.toNSString();

     //qDebug() << "WebViewInQt::loadRequest: " << loadUrl;
    /* [[cWebView mainFrame] loadRequest:
         [NSURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithString:strUrl]]]];*/
     [cWebView setMainFrameURL: [NSString stringWithUTF8String:loadUrl.toUtf8().data()]];

     }
}

void WebViewInQt::loadFinish(const QString &strUrl, const QString &strHtml)
{
    emit signalLoadFinish(strUrl, strHtml);
}

void WebViewInQt::loadError()
{
    emit signalLoadError();
}

void WebViewInQt::urlChanged(const QString &strUrl)
{
    emit signalUrlChanged(strUrl);
}
