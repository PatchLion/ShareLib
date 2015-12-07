#include "FlickrShare.h"
#include "ShareStructDefine.h"
#include "SharePublicFuncDefine.h"
#include "ShareMacroDefine.h"
#include "FlickrAuthorizeWebview.h"
#include "FlickrShareObject.h"
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QApplication>
#include <QtCore/QThread>

CFlickrShare::CFlickrShare(const QString& secret, const QString& apiKey, QObject *parent/*=0*/)
	: QObject(parent)
	, m_pAuthorizeWebview(0)
	, m_pShareObject(0)
	, m_pShareThread(0)
	, m_strSecret(secret)
	, m_strAPIKey(apiKey)
{
	m_pShareThread = new QThread(this);

	m_pShareObject = new CFlickrShareObject(secret, apiKey);
	connect(m_pShareObject, &CFlickrShareObject::albumsInfoRefreshFinished, this, &CFlickrShare::albumsInfoRefreshFinished, Qt::QueuedConnection);
	connect(m_pShareObject, &CFlickrShareObject::shareFinished, this, &CFlickrShare::shareFinished, Qt::QueuedConnection);
	connect(this, &CFlickrShare::shareSignal, m_pShareObject, &CFlickrShareObject::share, Qt::QueuedConnection);
	connect(this, &CFlickrShare::refreshAlbumListSignal, m_pShareObject, &CFlickrShareObject::refreshAlbumList, Qt::QueuedConnection);

	m_pShareObject->moveToThread(m_pShareThread);
	m_pShareThread->start();
}
 

CFlickrShare::~CFlickrShare()
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

void CFlickrShare::showAuthorizeWebView()
{
	if (!isAuthorized() && webView())
	{
		applyWebViewStyle();

		webView()->clearCookie();

		ShareLibrary::moveWidgetToScreenCenter(webView());

		webView()->exec();
	}
}

void CFlickrShare::checkAuthorize()
{
	showAuthorizeWebView();
}

void CFlickrShare::onTokenReceived()
{
	if (m_pShareObject && m_pAuthorizeWebview)
	{
		m_pShareObject->setTokenKey(m_pAuthorizeWebview->tokenKey());
		m_pShareObject->setUserID(m_pAuthorizeWebview->userID());

		emit tokenReceived();
	}
}

void CFlickrShare::setTokenKey(const QString& tokenKey)
{
	if (m_pShareObject)
	{
		m_pShareObject->setTokenKey(tokenKey);
	}
}

void CFlickrShare::share(IShareParam* pParam)
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

void CFlickrShare::refreshAlbumList()
{
	emit refreshAlbumListSignal();
}

void CFlickrShare::signOut()
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

bool CFlickrShare::isAuthorized() const
{
	if (m_pShareObject)
	{
		return !m_pShareObject->tokenKey().isEmpty();
	}
	return false;
}

QString CFlickrShare::tokenKey() const
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

CAuthorizeWebviewBase* CFlickrShare::webView()
{
	if (!m_pAuthorizeWebview)
	{
		m_pAuthorizeWebview = new CFlickrAuthorizeWebview(m_strSecret, m_strAPIKey);
		connect(m_pAuthorizeWebview, &CFlickrAuthorizeWebview::tokenReceived, this, &CFlickrShare::onTokenReceived, Qt::QueuedConnection);
		connect(m_pAuthorizeWebview, &CFlickrAuthorizeWebview::widgetClose, this, &CFlickrShare::onWebViewClose, Qt::QueuedConnection);
	}

	return m_pAuthorizeWebview;
}

void CFlickrShare::setUserID(const QString& strUserID)
{
	if (m_pShareObject)
	{
		m_pShareObject->setUserID(strUserID);
	}
}

QString CFlickrShare::userID() const
{
	if (m_pShareObject)
	{
		return m_pShareObject->userID();
	}

	return "";
}

void CFlickrShare::releaseWebView()
{
	if (m_pAuthorizeWebview)
	{
		disconnect(m_pAuthorizeWebview);
		SAFE_DELETE_QBJECT(m_pAuthorizeWebview);
	}
}

void CFlickrShare::onWebViewClose()
{
	releaseWebView();
}
