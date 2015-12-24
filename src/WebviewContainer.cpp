#include "WebviewContainer.h"
#include <QtCore/QTimer>
#include "SharePublicFuncDefine.h"
#include "ShareMacroDefine.h"

CWebviewContainer::CWebviewContainer(QWidget* pParent)
: QWebView(pParent)
{
	connect(this, &QWebView::loadFinished, this, &CWebviewContainer::onLoadFinished);
	connect(this, &QWebView::loadProgress, this, &CWebviewContainer::onPageLoadProcess);

	qRegisterMetaType<ShareLibrary::EPageLoadResult>("ShareLibrary::EPageLoadResult");

	setTimeOutInterval(DEFAULT_TIMEOUT_INTERVAL);

	m_loadTimer.setSingleShot(true);
	connect(&m_loadTimer, &QTimer::timeout, this, &CWebviewContainer::onLoadTimeOut);
}

CWebviewContainer::~CWebviewContainer()
{
	disconnect(this);
}

void CWebviewContainer::loadUrl(const QString& strUrl)
{
	QUrl url(strUrl);

/*
	if (url.isValid())
	{*/
		QNetworkRequest request(url);
		if ("HTTPS" == url.scheme().toUpper())
		{
			ShareLibrary::createSSLRequest(request, url);
		}
		else
		{
			request.setUrl(url);
		}
	
		startLoadTimer();
		load(request);
/*
	}
	else
	{
		onPageLoadFinished(ShareLibrary::Result_InvaildURL);
	}*/
}

QString CWebviewContainer::urlString() const
{
	return url().isEmpty() ? "" : url().toDisplayString();
}

void CWebviewContainer::clearCookie()
{
	if (page() && page()->networkAccessManager())
	{
		page()->networkAccessManager()->setCookieJar(new QNetworkCookieJar);
	}
}

QString CWebviewContainer::pagePlainText() const
{
	if (page() && page()->currentFrame())
	{
		return page()->currentFrame()->toHtml();
	}

	return "";
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
	}*/

	if (page() && page()->mainFrame())
	{
		page()->mainFrame()->setScrollBarValue(Qt::Vertical, page()->mainFrame()->scrollBarMinimum(Qt::Vertical));
		page()->mainFrame()->setScrollBarValue(Qt::Horizontal, page()->mainFrame()->scrollBarMinimum(Qt::Horizontal));
		//page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
		//page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	}
}

void CWebviewContainer::onSslErrors( QNetworkReply *reply, const QList<QSslError> &errors )
{
	//onPageLoadFinished(ShareLibrary::Result_SSLError);

	for (int nIndex = 0; nIndex < errors.size(); nIndex++)
	{
		qDebug() << errors[nIndex].errorString();
	}

	reply->ignoreSslErrors(errors);
}

void CWebviewContainer::onPageLoadFinished(ShareLibrary::EPageLoadResult eResult)
{
	stopLoadTimer();

	emit pageLoadFinished(eResult);
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

void CWebviewContainer::onLoadTimeOut()
{
	stopLoadTimer();

	onPageLoadFinished(ShareLibrary::Result_TimeOut);
}


