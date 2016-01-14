#include "TwitterAuthorizeWebview.h"
#include "SharePublicFuncDefine.h"
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include "qsingleton.h"
#include "TwitterShare.h"

#define TWITTER_REQUEST_TOKEN_URL								"https://api.twitter.com/oauth/request_token"
#define TWITTER_AUTHORIZE_URL											"https://api.twitter.com/oauth/authorize"
#define TWITTER_ACCESS_TOKEN_URL									"https://api.twitter.com/oauth/access_token"

CTwitterAuthorizeWebview::CTwitterAuthorizeWebview(const QString& strConsumerKey, /*在twitter上申请的用户Key */ const QString& strConsumerSecret, /*在twitter上申请的用户秘钥, */ QWidget *parent /*= 0*/)
	: CAuthorizeWebviewBase(parent)
	, m_strConsumerKey(strConsumerKey)
	, m_strConsumerSecret(strConsumerSecret)
	, m_bStartToRequestAccessToken(false)
{

}


CTwitterAuthorizeWebview::~CTwitterAuthorizeWebview()
{

}


void CTwitterAuthorizeWebview::onPageLoadFinished(ShareLibrary::EPageLoadResult eResult)
{
	CAuthorizeWebviewBase::onPageLoadFinished(eResult);

	if (ShareLibrary::Result_Success == eResult)
	{
		const QString strUrl = urlString();
		const QString strFramePlainText = pagePlainText();

		if (strUrl == "https://api.twitter.com/oauth/authorize")
		{
			if (strFramePlainText.contains("logged-out"))
			{
				//logout

				close();
			}
			else
			{
				if (!m_bStartToRequestAccessToken)
				{
					m_bStartToRequestAccessToken = true;
					qDebug() << "Twitter token page: " << strFramePlainText;

					QString strKey1 = "<code>";
					QString strKey2 = "</code>";

					int nIndex1 = strFramePlainText.indexOf(strKey1);
					int nIndex2 = strFramePlainText.indexOf(strKey2);

					QString pin = strFramePlainText.mid(nIndex1 + strKey1.size(), nIndex2 - nIndex1 - strKey1.size());

					qDebug() << "Twitter pin: " << pin;

					startRequestAccessToken(pin);
				}
			}
		}		
	}
}

void CTwitterAuthorizeWebview::startRequestAccessToken(const QString& strPin)
{
	QUrl url(TWITTER_ACCESS_TOKEN_URL);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QUrlQuery query;
	query.addQueryItem("oauth_verifier", strPin.toUtf8());
	url.setQuery(query);
#else
	url.addEncodedQueryItem("oauth_verifier", strPin.toUtf8());
#endif

	QByteArray oauthHeader = CTwitterShare::generateAuthorizationHeader(url, ShareLibrary::Post, m_strOauthToken, m_strOauthTokenSecret, m_strConsumerKey, m_strConsumerSecret);

	QNetworkRequest req;
	ShareLibrary::createSSLRequest(req, url);
	req.setRawHeader("Authorization", oauthHeader);
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QNetworkReply* pTempReply = Singleton<QNetworkAccessManager>::instance().post(req, QByteArray());

	if (pTempReply)
	{
		replyTempObjectManager().addTempReply(pTempReply);
		connect(pTempReply, SIGNAL(finished()), this, SLOT(onReplayFinishedAccessToken()), Qt::UniqueConnection);
	}
	else
	{
		onPageLoadFinished(ShareLibrary::Result_OtherError);
	}
}
void CTwitterAuthorizeWebview::onReplayFinishedAccessToken()
{
	QNetworkReply* rep = dynamic_cast<QNetworkReply*>(sender());

	if (rep && rep->error() == QNetworkReply::NoError)
	{
		const QByteArray response = rep->readAll();

		parseTokens(response);


		if (m_strOauthToken.isEmpty() || m_strOauthTokenSecret.isEmpty())
		{
			onPageLoadFinished(ShareLibrary::Result_FailedToAuth);
		}
		else
		{
			setResultCode(ShareLibrary::Result_Success);
			setState(ShareLibrary::ShowSuccess);

			emit tokenReceived();

			startDelayClose();
		}
	}
	else
	{
		onPageLoadFinished(ShareLibrary::Result_OtherError);
	}
}
void CTwitterAuthorizeWebview::startToAuthorize()
{
	CAuthorizeWebviewBase::startToAuthorize();

	setState(ShareLibrary::ShowDefault);

	m_bStartToRequestAccessToken = false;

	startRequestOauthToken();
}

void CTwitterAuthorizeWebview::startRequestOauthToken()
{
	QUrl url(TWITTER_REQUEST_TOKEN_URL);

	QByteArray oauthHeader = CTwitterShare::generateAuthorizationHeader(url, ShareLibrary::Post, m_strOauthToken, m_strOauthTokenSecret, m_strConsumerKey, m_strConsumerSecret);

	QNetworkRequest req;
	ShareLibrary::createSSLRequest(req, url);
	req.setRawHeader("Authorization", oauthHeader);
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QNetworkReply* pTempReply = Singleton<QNetworkAccessManager>::instance().post(req, QByteArray());//

	if (pTempReply)
	{
		replyTempObjectManager().addTempReply(pTempReply);

		connect(pTempReply, SIGNAL(finished()), this, SLOT(onReplayFinishedOauthToken()), Qt::UniqueConnection);
	}
	else
	{
		onPageLoadFinished(ShareLibrary::Result_OtherError);
	}
}


void CTwitterAuthorizeWebview::onReplayFinishedOauthToken()
{
	QNetworkReply* rep = dynamic_cast<QNetworkReply*>(sender());

	if (rep && rep->error() == QNetworkReply::NoError)
	{
		const QByteArray response = rep->readAll();

		parseTokens(response);

		if (m_strOauthToken.isEmpty() || m_strOauthTokenSecret.isEmpty())
		{
			onPageLoadFinished(ShareLibrary::Result_FailedToAuth);
		}
		else
		{
			const QString strUrl = authorizationUrl();

			loadUrl(strUrl);
		}
	}
	else
	{
		if (rep)
		{
			qDebug() << rep->errorString();
		}
		onPageLoadFinished(ShareLibrary::Result_OtherError);
	}
}

void CTwitterAuthorizeWebview::parseTokens(const QByteArray& response)
{
	//OAuth spec 5.3, 6.1.2, 6.3.2
	//use QUrl for parsing
	QByteArray parseQuery("http://parse.com?");

	QUrl parseUrl = QUrl::fromEncoded(parseQuery + response);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QUrlQuery query(parseUrl);
	m_strOauthToken = query.queryItemValue("oauth_token");
	m_strOauthTokenSecret = query.queryItemValue("oauth_token_secret");
#else
	m_strOauthToken = parseUrl.encodedQueryItemValue("oauth_token");
	m_strOauthTokenSecret = parseUrl.encodedQueryItemValue("oauth_token_secret");
#endif
}

QString CTwitterAuthorizeWebview::authorizationUrl() const
{
	QUrl authorizeUrl(TWITTER_AUTHORIZE_URL);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QUrlQuery query;
	query.addQueryItem("oauth_token", m_strOauthToken.toUtf8());
	query.addQueryItem("oauth_callback", "oob");

	authorizeUrl.setQuery(query);
#else
	authorizeUrl.addEncodedQueryItem("oauth_token", m_strOauthToken.toUtf8());
	authorizeUrl.addEncodedQueryItem("oauth_callback", "oob");
#endif

	return authorizeUrl.toString();
}

void CTwitterAuthorizeWebview::onPageLoadProcess(int process)
{
	CAuthorizeWebviewBase::onPageLoadProcess(process);

	if (isFirstLoad())
	{
		setState(ShareLibrary::ShowProcess);
	}
}

void CTwitterAuthorizeWebview::reset()
{
	setState(ShareLibrary::ShowDefault);

	loadUrl("");
}

void CTwitterAuthorizeWebview::clearToken()
{
	m_strOauthToken.clear();
	m_strOauthTokenSecret.clear();
}
