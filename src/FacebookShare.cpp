#include "FacebookShare.h"
#include "ShareStructDefine.h"
#include "SharePublicFuncDefine.h"
#include "ShareMacroDefine.h"
#include "FaceBookAuthorizeWebview.h"
#include "FacebookShareObject.h"
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QApplication>
#include <QtCore/QThread>

CFacebookShare::CFacebookShare(const QString& clientID, QObject *parent/*=0*/)
	: QObject(parent)
	, m_pAuthorizeWebview(0)
	, m_pShareObject(0)
	, m_strClientID(clientID)
	, m_pShareThread(0)
{
	m_pShareThread = new QThread(this);

	m_pShareObject = new CFacebookShareObject;
	connect(m_pShareObject, &CFacebookShareObject::albumsInfoRefreshFinished, this, &CFacebookShare::albumsInfoRefreshFinished, Qt::QueuedConnection);
	connect(m_pShareObject, &CFacebookShareObject::shareFinished, this, &CFacebookShare::shareFinished, Qt::QueuedConnection);
	connect(this, &CFacebookShare::shareSignal, m_pShareObject, &CFacebookShareObject::share, Qt::QueuedConnection);
	connect(this, &CFacebookShare::refreshAlbumListSignal, m_pShareObject, &CFacebookShareObject::refreshAlbumList, Qt::QueuedConnection);

	m_pShareObject->moveToThread(m_pShareThread);
	m_pShareThread->start();
}
 

CFacebookShare::~CFacebookShare()
{
	if (m_pShareThread)
	{
		m_pShareThread->quit();
		m_pShareThread->wait();
		SAFE_DELETE_QBJECT(m_pShareThread);
	}
	releaseWebView();
	SAFE_DELETE_QBJECT(m_pShareObject);
}

void CFacebookShare::showAuthorizeWebView()
{
	if (!isAuthorized() && webView())
	{
		applyWebViewStyle();

		webView()->clearCookie();

		ShareLibrary::moveWidgetToScreenCenter(webView());

		webView()->exec();
	}
}

void CFacebookShare::checkAuthorize()
{
	if (!isAuthorized())
	{
		showAuthorizeWebView();
	}
}

void CFacebookShare::onTokenReceived()
{
	if (m_pShareObject && m_pAuthorizeWebview)
	{
		m_pShareObject->setTokenKey(m_pAuthorizeWebview->tokenKey());
	
		emit tokenReceived();
	}
}

void CFacebookShare::setTokenKey(const QString& tokenKey)
{
	if (m_pShareObject)
	{
		m_pShareObject->setTokenKey(tokenKey);
	}
}

void CFacebookShare::share(IShareParam* pParam)
{
	if (!isAuthorized())
	{
		checkAuthorize();
	}
	else
	{
		emit shareSignal(pParam);
	}
}

void CFacebookShare::refreshAlbumList()
{
	emit refreshAlbumListSignal();
}

void CFacebookShare::signOut()
{
	if (m_pShareObject)
	{
		m_pShareObject->signOut();
	}
	if (m_pAuthorizeWebview)
	{
		m_pAuthorizeWebview->reset();
	}
	if (!isAuthorized())
	{
		checkAuthorize();
	}
}

bool CFacebookShare::isAuthorized() const
{
	if (m_pShareObject)
	{
		return !m_pShareObject->tokenKey().isEmpty();
	}
	return false;
}

QString CFacebookShare::tokenKey() const
{
	if (m_pAuthorizeWebview)
	{
		return m_pAuthorizeWebview->tokenKey();
	}
	else
	{
		return "";
	}
}

uint CFacebookShare::expiresTimeStamp() const
{
	if (m_pAuthorizeWebview)
	{
		return m_pAuthorizeWebview->expiresTimeStamp();
	}
	else
	{
		return 0;
	}
}


CAuthorizeWebviewBase* CFacebookShare::webView()
{
	if (!m_pAuthorizeWebview)
	{
		m_pAuthorizeWebview = new CFaceBookAuthorizeWebview(m_strClientID);
		connect(m_pAuthorizeWebview, &CFaceBookAuthorizeWebview::tokenReceived, this, &CFacebookShare::onTokenReceived, Qt::QueuedConnection);
		connect(m_pAuthorizeWebview, &CFaceBookAuthorizeWebview::widgetClose, this, &CFacebookShare::onWebViewClose, Qt::QueuedConnection);
	}

	return m_pAuthorizeWebview;
}
void CFacebookShare::releaseWebView()
{
	if (m_pAuthorizeWebview)
	{
		disconnect(m_pAuthorizeWebview);
		SAFE_DELETE_QBJECT(m_pAuthorizeWebview);
	}
}


void CFacebookShare::onWebViewClose()
{
	releaseWebView();
}
