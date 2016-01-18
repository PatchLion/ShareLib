#include "TwitterShare.h"
#include "ShareStructDefine.h"
#include "SharePublicFuncDefine.h"
#include "ShareMacroDefine.h"
#include "TwitterAuthorizeWebview.h"
#include "TwitterShareObject.h"
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QApplication>
#include <QtCore/QThread>
#include <QtCore/QUrlQuery>
#include <QtCore/QDebug>
#include <QtCore/QThreadPool>

CTwitterShare::CTwitterShare(const QString& strConsumerKey, /*在twitter上申请的用户Key */ const QString& strConsumerSecret, /*在twitter上申请的用户秘钥 */ QObject *parent /*= 0*/)
	: QObject(parent)
	, m_pAuthorizeWebview(0)
	, m_pShareObject(0)
	, m_pShareThread(0)
	, m_strConsumerKey(strConsumerKey)
	, m_strConsumerSecret(strConsumerSecret)
{
	m_pShareThread = new QThread(this);

	m_pShareObject = new CTwitterShareObject(m_strConsumerKey, m_strConsumerSecret);
	connect(this, &CTwitterShare::shareSignal, m_pShareObject, &CTwitterShareObject::share, Qt::QueuedConnection);
	connect(m_pShareObject, &CTwitterShareObject::shareFinished, this, &CTwitterShare::shareFinished, Qt::QueuedConnection);

	m_pShareObject->moveToThread(m_pShareThread);
	m_pShareThread->start();
}



CTwitterShare::~CTwitterShare()
{
	if (m_pShareThread)
	{
		m_pShareThread->quit();
		m_pShareThread->wait();
		//SAFE_DELETE_QBJECT(m_pShareThread);
	}

	releaseWebView();
	SAFE_DELETE_QBJECT(m_pShareObject);
    
    QThreadPool::globalInstance()->waitForDone();
}

void CTwitterShare::showAuthorizeWebView()
{

	if (!isAuthorized() && webView())
	{
		applyWebViewStyle();

		webView()->clearCookie();

		ShareLibrary::moveWidgetToScreenCenter(webView());

		webView()->exec();
	}
}

void CTwitterShare::checkAuthorize()
{
	showAuthorizeWebView();
}

void CTwitterShare::onTokenReceived()
{
	if (m_pShareObject && m_pAuthorizeWebview)
	{
		m_pShareObject->setOauthToken(m_pAuthorizeWebview->oauthToken());
		m_pShareObject->setOauthTokenSecret(m_pAuthorizeWebview->oauthTokenSecret());

		emit tokenReceived();
	}
	
}

void CTwitterShare::share(IShareParam* pParam)
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


void CTwitterShare::signOut()
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

bool CTwitterShare::isAuthorized() const
{
	if (m_pShareObject)
	{
		return !m_pShareObject->oauthToken().isEmpty() 
			&& !m_pShareObject->oauthTokenSecret().isEmpty();
	}
	return false;
}




QByteArray CTwitterShare::generateAuthorizationHeader(const QUrl& url, ShareLibrary::HttpMethod method, const QString& strOauthToken, const QString& strOauthTokenSecret, const QString& strConsumerKey, const QString& strConsumerSecret)
{
	if (strOauthToken.isEmpty() && strOauthTokenSecret.isEmpty())
		qDebug() << "OAuth tokens are empty!";

	QByteArray timeStamp = ShareLibrary::generateTimeStampString().toUtf8();
	QByteArray nonce = ShareLibrary::generateNonce();

	QByteArray sigBase = CTwitterShare::generateSignatureBase(url, method, timeStamp, nonce, strConsumerKey, strOauthToken);
	QByteArray signature = CTwitterShare::generateSignatureHMACSHA1(sigBase, strConsumerSecret, strOauthTokenSecret);

	QByteArray header;
	header += "OAuth ";
	header += "oauth_consumer_key=\"" + strConsumerKey.toUtf8() + "\",";
	if (!strOauthToken.isEmpty())
		header += "oauth_token=\"" + strOauthToken.toUtf8() + "\",";
	header += "oauth_signature_method=\"HMAC-SHA1\",";
	header += "oauth_signature=\"" + signature + "\",";
	header += "oauth_timestamp=\"" + timeStamp + "\",";
	header += "oauth_nonce=\"" + nonce + "\",";
	header += "oauth_version=\"1.0\"";

	qDebug() << "OAuth header:" << header;

	return header;
}

QByteArray CTwitterShare::generateSignatureHMACSHA1(const QByteArray& signatureBase, const QString& strConsumerSecret, const QString& strOauthTokenSecret)
{
	//OAuth spec. 9.2 http://oauth.net/core/1.0/#anchor16
	QByteArray key = (strConsumerSecret + '&' + strOauthTokenSecret).toUtf8();
	QByteArray result = ShareLibrary::hmacSha1(signatureBase, key);
	QByteArray resultBE64 = result.toBase64();
	QByteArray resultPE = resultBE64.toPercentEncoding();
	return resultPE;
}


QByteArray CTwitterShare::generateSignatureBase(const QUrl& url, ShareLibrary::HttpMethod method, const QByteArray& timestamp, const QByteArray& nonce, const QString& strConsumerKey, const QString& strOauthToken)
{
	//OAuth spec. 9.1 http://oauth.net/core/1.0/#anchor14

	//OAuth spec. 9.1.1

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QUrlQuery query(url);
	QList<QPair<QString, QString> > urlParameters = query.queryItems();
	QList<QString> normParameters;

	QListIterator<QPair<QString, QString> > i(urlParameters);
	while (i.hasNext()){
		QPair<QString, QString> queryItem = i.next();
		QString normItem = queryItem.first + '=' + queryItem.second;
		normParameters.append(normItem);
	}

	//consumer key
	normParameters.append("oauth_consumer_key=" + strConsumerKey);

	//token
	if (!strOauthToken.isEmpty()){
		normParameters.append("oauth_token=" + strOauthToken);
	}

	//signature method, only HMAC_SHA1
	normParameters.append("oauth_signature_method=HMAC-SHA1");
	//time stamp
	normParameters.append("oauth_timestamp=" + timestamp);
	//nonce
	normParameters.append("oauth_nonce=" + nonce);
	//version
	normParameters.append("oauth_version=1.0");

	//OAuth spec. 9.1.1.1
	qSort(normParameters);



	QByteArray normString;
	QListIterator<QString> j(normParameters);
	while (j.hasNext()) {
		normString += j.next().toUtf8().toPercentEncoding();
		normString += "%26";
	}
	normString.chop(3);

	//OAuth spec. 9.1.2
	QString urlScheme = url.scheme();
	QString urlPath = url.path();
	QString urlHost = url.host();
	QByteArray normUrl = urlScheme.toUtf8() + "://" + urlHost.toUtf8() + urlPath.toUtf8();

	QByteArray httpm = ShareLibrary::getHttpTypeString(method).toUtf8();

	//OAuth spec. 9.1.3
	return httpm + '&' + normUrl.toPercentEncoding() + '&' + normString;

#else

	QList<QPair<QByteArray, QByteArray> > urlParameters = url.encodedQueryItems();
	QList<QByteArray> normParameters;

	QListIterator<QPair<QByteArray, QByteArray> > i(urlParameters);
	while (i.hasNext()){
		QPair<QByteArray, QByteArray> queryItem = i.next();
		QByteArray normItem = queryItem.first + '=' + queryItem.second;
		normParameters.append(normItem);
	}

	//consumer key
	normParameters.append(QByteArray("oauth_consumer_key=") + m_strConsumerKey.toUtf8());

	//token
	if (!m_strOauthToken.isEmpty()){
		normParameters.append(QByteArray("oauth_token=") + m_strOauthToken.toUtf8());
	}

	//signature method, only HMAC_SHA1
	normParameters.append(QByteArray("oauth_signature_method=HMAC-SHA1"));
	//time stamp
	normParameters.append(QByteArray("oauth_timestamp=") + timestamp);
	//nonce
	normParameters.append(QByteArray("oauth_nonce=") + nonce);
	//version
	normParameters.append(QByteArray("oauth_version=1.0"));

	//OAuth spec. 9.1.1.1
	qSort(normParameters);



	QByteArray normString;
	QListIterator<QByteArray> j(normParameters);
	while (j.hasNext()) {
		normString += j.next().toPercentEncoding();
		normString += "%26";
	}
	normString.chop(3);

	//OAuth spec. 9.1.2
	QString urlScheme = url.scheme();
	QString urlPath = url.path();
	QString urlHost = url.host();
	QByteArray normUrl = urlScheme.toUtf8() + "://" + urlHost.toUtf8() + urlPath.toUtf8();

	QByteArray httpm = ShareLibrary::getHttpTypeString(method).toUtf8();

	//OAuth spec. 9.1.3
	return httpm + '&' + normUrl.toPercentEncoding() + '&' + normString;
#endif

}

CAuthorizeWebviewBase* CTwitterShare::webView()
{
	if (!m_pAuthorizeWebview)
	{
		m_pAuthorizeWebview = new CTwitterAuthorizeWebview(m_strConsumerKey, m_strConsumerSecret);
		connect(m_pAuthorizeWebview, &CTwitterAuthorizeWebview::tokenReceived, this, &CTwitterShare::onTokenReceived, Qt::QueuedConnection);
		connect(m_pAuthorizeWebview, &CTwitterAuthorizeWebview::widgetClose, this, &CTwitterShare::onWebViewClose, Qt::QueuedConnection);
	}

	return m_pAuthorizeWebview;
}

void CTwitterShare::setOauthToken(const QString& val)
{
	if (m_pShareObject)
	{
		m_pShareObject->setOauthToken(val);
	}
}

QString CTwitterShare::oauthToken() const
{
	if (m_pShareObject)
	{
		return m_pShareObject->oauthToken();
	}
	return "";
}

void CTwitterShare::setOauthTokenSecret(const QString& val)
{
	if (m_pShareObject)
	{
		m_pShareObject->setOauthTokenSecret(val);
	}
}

QString CTwitterShare::oauthTokenSecret() const
{
	if (m_pShareObject)
	{
		return m_pShareObject->oauthTokenSecret();
	}
	return "";
}

void CTwitterShare::releaseWebView()
{
	if (m_pAuthorizeWebview)
	{
        qDebug() << "Release facebook authorize webview!";
		//disconnect(m_pAuthorizeWebview);
		SAFE_DELETE_QBJECT(m_pAuthorizeWebview);
	}
}

void CTwitterShare::onWebViewClose()
{
	releaseWebView();
}
