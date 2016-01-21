

#ifndef __CWebviewContainer__
#define __CWebviewContainer__

#include <QtCore/qglobal.h>
#include <QtCore/QTimer>
#include "ShareMacroDefine.h"

#ifdef Q_OS_MAC
//#include <QtWidgets/QMacCocoaViewContainer>
#include "webviewinqt.h"
#endif

#ifdef Q_OS_WIN
#if(QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#else
#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#endif
#include <QtNetwork/QSslError>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkCookieJar>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#endif

#include "IWebviewContainer.h"

#ifdef Q_OS_MAC
class CWebviewContainer : public WebViewInQt, public IWebviewContainer
#else //if Q_OS_WIN
class CWebviewContainer : public QWebView, public IWebviewContainer
#endif
{
    Q_OBJECT

public:
    CWebviewContainer(QWidget* pParent = 0);
    ~CWebviewContainer();

public:
	//超时时间
	void setTimeOutInterval(int nInterval) { m_loadTimer.setInterval(nInterval); }
	int timeOutInterval() const{ return m_loadTimer.interval(); };

	//加载Url
	virtual void loadUrl(const QString& strURL);
    virtual QString urlString() const;

	//网页内容(去掉HTML标记)
	virtual QString pagePlainText() const;

	//Cookie
	virtual void clearCookie();

	//页面加载完毕(根据eResult判断成功或失败)
	void onPageLoadFinished(ShareLibrary::EPageLoadResult eResult);


public slots:
	//页面加载进度
	void onPageLoadProcess(int process);

	void onLoadFinished(bool bOK);
    
    //加载超时
    void onLoadTimeOut();


private slots:
#ifdef Q_OS_MAC
    void onPageLoadFinishMac(const QString &strUrl, const QString &strHtml);
    void onPageLoadErrorMac();
    void onPageUrlChangedMac(const QString &strUrl);
#endif

protected:
	//开始计时器(如果已启动，则重置), nInterval周期(毫秒)
	void startLoadTimer();


	//停止计时器
	void stopLoadTimer();


private slots:
#ifdef Q_OS_WIN
	void onSslErrors( QNetworkReply *reply, const QList<QSslError> &errors );
#endif



signals:
	//页面加载完毕(根据eResult判断成功或失败)
	void pageLoadFinished(ShareLibrary::EPageLoadResult eResult);

	//页面加载进度
    void pageLoadProcess(int process);

private:
	QTimer		m_loadTimer;

#ifdef Q_OS_MAC
    QString     m_urlString;
    QString     m_htmlString;
#endif
};

#endif /* defined(__CWebviewContainer__) */
