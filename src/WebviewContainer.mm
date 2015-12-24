#include "WebviewContainer.h"

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import <QtWidgets/QLayout>
#import <QtCore/QDebug>
#import "CNotifyWebView.h"


CWebviewContainer::CWebviewContainer(QWidget *parent)
: QMacCocoaViewContainer(0, parent)
{
    CNotifyWebView* webView = [[CNotifyWebView alloc] init];
    [webView setContainer: this];
    

    setCocoaView(webView);
    
    if (layout())
    {
        layout()->setSpacing(0);
        layout()->setMargin(0);
    }
    
    setTimeOutInterval(DEFAULT_TIMEOUT_INTERVAL);
    
    m_loadTimer.setSingleShot(true);
    connect(&m_loadTimer, &QTimer::timeout, this, &CWebviewContainer::onLoadTimeOut);
}

CWebviewContainer::~CWebviewContainer()
{
    qDebug() << "CWebviewContainer release!";
    id webView = (id)cocoaView();
    bool bSuc = [webView isMemberOfClass: [CNotifyWebView class]] ;
    if(bSuc)
    {
        qDebug() << "webView setContainer = 0!";
        [webView setContainer: 0];
        [webView release];
    }
    setCocoaView(0);
    qDebug() << "CWebviewContainer setCocoaView = 0!";
}

QString CWebviewContainer::pagePlainText() const
{
    id webView = (id)cocoaView();
    bool bSuc = [webView isMemberOfClass: [CNotifyWebView class]] ;
    if(bSuc)
    {
        WebDataSource *source = [[webView mainFrame] dataSource];
        NSData *data = [source data];
        NSString *str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
        
        QByteArray tempPlainText;
        return tempPlainText.fromPercentEncoding([str UTF8String]);
    }
    else
    {
        return "";
    }
    
}
QString CWebviewContainer::urlString() const
{
    id webView = (id)cocoaView();
    bool bSuc = [webView isMemberOfClass: [CNotifyWebView class]] ;
    if(bSuc)
    {
        NSString* strUrl = [webView mainFrameURL];
        
        QByteArray tempUrl;
        
        return tempUrl.fromPercentEncoding([strUrl UTF8String]);
    }
    else
    {
        return "";
    }
}

void CWebviewContainer::loadUrl(const QString& strUrl)
{
    id webView = (id)cocoaView();
    bool bSuc = [webView isMemberOfClass: [CNotifyWebView class]] ;
    if(bSuc)
    {
        [webView setMainFrameURL: [NSString stringWithUTF8String: strUrl.toUtf8().data()]];
        NSLog(@"CWebviewContainer::setUrl: %s", strUrl.toUtf8().data());
    }
}

void CWebviewContainer::onPageLoadFinished(ShareLibrary::EPageLoadResult eResult)
{
    stopLoadTimer();
    
    emit pageLoadFinished(eResult);
}

void CWebviewContainer::clearCookie()
{
   // NSString *cookiesUrl = [NSString stringWithUTF8String:strUrlHead.toUtf8().data()];

    NSHTTPCookie *cookie;
    NSHTTPCookieStorage *cookieJar = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    /*NSArray *cookieAry = [cookieJar cookiesForURL: [NSURL URLWithString: cookiesUrl]];
    
    for (cookie in cookieAry) {
        //NSLog(@"delete cookie: %@ - %@", [cookie key], [cookie value]);
        [cookieJar deleteCookie: cookie];
    }*/
    
    for (cookie in [cookieJar cookies]) {
        //NSLog(@"delete cookie: %@ - %@", [cookie key], [cookie value]);
        [cookieJar deleteCookie: cookie];
    }

}
void CWebviewContainer::onLoadTimeOut()
{
    stopLoadTimer();
    
    onPageLoadFinished(ShareLibrary::Result_TimeOut);
}

void CWebviewContainer::startLoadTimer()
{
    if (m_loadTimer.isActive())
    {
        m_loadTimer.stop();
    }
    m_loadTimer.start();
}

void CWebviewContainer::stopLoadTimer()
{
    if (m_loadTimer.isActive())
    {
        m_loadTimer.stop();
    }
}

void CWebviewContainer::onPageLoadProcess(int process)
{
    emit pageLoadProcess(process);
}

void CWebviewContainer::onLoadFinished(bool bOK)
{
    if (bOK)
    {
        onPageLoadFinished(ShareLibrary::Result_Success);
    }
    /*
     else
     {
     onPageLoadFinished(ShareLibrary::Result_OtherError);
     }
    
    if (page() && page()->mainFrame())
    {
        page()->mainFrame()->setScrollBarValue(Qt::Vertical, page()->mainFrame()->scrollBarMinimum(Qt::Vertical));
        page()->mainFrame()->setScrollBarValue(Qt::Horizontal, page()->mainFrame()->scrollBarMinimum(Qt::Horizontal));
        //page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
        //page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    }*/
}
