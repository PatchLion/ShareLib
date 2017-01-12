#include "WebviewContainer.h"
#include <QtWidgets/QLayout>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <WebKit/WebKit.h>
CWebviewContainer::CWebviewContainer(QWidget *parent)
: WebViewInQt(0, parent)
{
    if (layout())
    {
        layout()->setSpacing(0);
        layout()->setMargin(0);
    }

    setTimeOutInterval(DEFAULT_TIMEOUT_INTERVAL);
    
    m_loadTimer.setSingleShot(true);
    connect(&m_loadTimer, &QTimer::timeout, this, &CWebviewContainer::onLoadTimeOut);

    connect(this, &CWebviewContainer::signalLoadFinish, this, &CWebviewContainer::onPageLoadFinishMac);
    connect(this, &CWebviewContainer::signalLoadError, this, &CWebviewContainer::onPageLoadErrorMac);
    connect(this, &CWebviewContainer::signalUrlChanged, this, &CWebviewContainer::onPageUrlChangedMac);
}

CWebviewContainer::~CWebviewContainer()
{

}

QString CWebviewContainer::pagePlainText() const
{
    /*
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
    }*/
    return m_htmlString;
}
QString CWebviewContainer::urlString() const
{
    /*
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
    }*/
    return m_urlString;
}

void CWebviewContainer::loadUrl(const QString& strUrl)
{
    //qDebug() << "webviewcontainer[load]: " << strUrl;
    m_urlString = strUrl;
    WebViewInQt::loadRequest(strUrl);
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
    else
    {
        onPageLoadFinished(ShareLibrary::Result_OtherError);
    }
}
void CWebviewContainer::onPageLoadFinishMac(const QString &strUrl, const QString &strHtml)
{
    //qDebug() << "webviewcontainer[finished]: " << strUrl;
    m_urlString = strUrl;
    m_htmlString = strHtml;
    onLoadFinished(true);
}

void CWebviewContainer::onPageLoadErrorMac()
{
    /*
    qWarning() << "webviewcontainer[error]!";
    onLoadFinished(false);*/
}

void CWebviewContainer::onPageUrlChangedMac(const QString &strUrl)
{
    //qWarning() << "AAAAA";
    //qDebug() << "webviewcontainer[url_change]: " << strUrl;
    m_urlString = strUrl;
    m_loadTimer.stop();
    //onLoadFinished(true);
}
