#include "FlickrAuthorizeWebview.h"
#include "SharePublicFuncDefine.h"
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QCryptographicHash>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include "qsingleton.h"

CFlickrAuthorizeWebview::CFlickrAuthorizeWebview(const QString& secret, const QString& apiKey, QWidget *parent)
: CAuthorizeWebviewBase(parent)
, m_strSecret(secret)
, m_strAPIKey(apiKey)
, m_bIsGetTokenByForbStarted(false)
{
}


CFlickrAuthorizeWebview::~CFlickrAuthorizeWebview()
{

}


void CFlickrAuthorizeWebview::onPageLoadFinished(ShareLibrary::EPageLoadResult eResult)
{
	CAuthorizeWebviewBase::onPageLoadFinished(eResult);

	if (ShareLibrary::Result_Success == eResult)
	{
		const QString strUrl = urlString();

		qDebug() << "Flickr page load: " << strUrl;


		if (strUrl == "https://www.flickr.com/services/auth/" && !m_bIsGetTokenByForbStarted)
		{
			startGetTokenByForb();
		}
	}

}

void CFlickrAuthorizeWebview::startGetTokenByForb()
{
	//Url
	QString flickrUrl("https://api.flickr.com/services/rest/?");
	flickrUrl += QString("method=flickr.auth.getToken");
	flickrUrl += QString("&api_key=") + m_strAPIKey;
	flickrUrl += QString("&frob=") + m_strForb;

	QString test = m_strSecret + "api_key" + m_strAPIKey + "frob" + m_strForb + "methodflickr.auth.getToken";
	QByteArray md5 = QCryptographicHash::hash(test.toUtf8(), QCryptographicHash::Md5);
	QString api_sig(md5.toHex().constData());
	flickrUrl += QString("&api_sig=") + api_sig;

	QNetworkRequest request = ShareLibrary::createSSLRequest(flickrUrl);

	QNetworkReply* pTempReply = Singleton<QNetworkAccessManager>::instance().get(request);

	if (pTempReply)
	{
		replyTempObjectManager().addTempReply(pTempReply);
		connect(pTempReply, SIGNAL(finished()), this, SLOT(onReplayFinishedGetToken()));
	}
	else
	{
		onPageLoadFinished(ShareLibrary::Result_OtherError);
	}
}

void CFlickrAuthorizeWebview::onReplayFinishedGetToken()
{
	QNetworkReply* rep = dynamic_cast<QNetworkReply*>(sender());

	if (rep)
	{
		if (QNetworkReply::NoError == rep->error())
		{
			QString all = QString::fromUtf8(rep->readAll());

			if (all.contains("token"))
			{
				int fisrt = all.indexOf("token");
				int last = all.lastIndexOf("token");
				QString token = all.mid(fisrt + 6, (last - 2) - (fisrt + 6));
				fisrt = all.indexOf("nsid=");
				last = all.lastIndexOf(" username");
				QString userid = all.mid(fisrt + 6, (last - 1) - (fisrt + 6));

				m_strTokenKey = token;
				m_strUserID = userid;

				setResultCode(ShareLibrary::Result_Success);
				setState(ShareLibrary::ShowSuccess);

				emit tokenReceived();

				startDelayClose();
			}
			else
			{
				onPageLoadFinished(ShareLibrary::Result_FailedToAuth);
			}
		}
		else
		{
			onPageLoadFinished(ShareLibrary::Result_FailedToAuth);
		}
	}
	else
	{
		onPageLoadFinished(ShareLibrary::Result_OtherError);
	}
}
void CFlickrAuthorizeWebview::startToAuthorize()
{
	CAuthorizeWebviewBase::startToAuthorize();

	setState(ShareLibrary::ShowDefault);

	m_bIsGetTokenByForbStarted = false;

	startToGetFrob();
}

QString CFlickrAuthorizeWebview::authorizationUrl() const
{
	QString authUrl("https://www.flickr.com/services/auth/?api_key=");
	authUrl += m_strAPIKey;
	authUrl += "&perms=write";
	authUrl += "&frob=" + m_strForb;

	QString test = m_strSecret + "api_key" + m_strAPIKey + "frob" + m_strForb + "permswrite";

	QByteArray md5 = QCryptographicHash::hash(test.toUtf8(), QCryptographicHash::Md5);
	QString api_sig1(md5.toHex().constData());
	authUrl += "&api_sig=" + api_sig1;

	return authUrl;
}

void CFlickrAuthorizeWebview::onPageLoadProcess(int process)
{
	CAuthorizeWebviewBase::onPageLoadProcess(process);

	if (isFirstLoad())
	{
		setState(ShareLibrary::ShowProcess);
	}
}

void CFlickrAuthorizeWebview::reset()
{
	setState(ShareLibrary::ShowDefault);

	loadUrl("");
}

void CFlickrAuthorizeWebview::startToGetFrob()
{
	//Url
	QString flickrUrl("https://www.flickr.com/services/rest/?");
	flickrUrl += QString("method=flickr.auth.getFrob");
	flickrUrl += QString("&api_key=") + m_strAPIKey;

	//
	QString tempString = m_strSecret + "api_key" + m_strAPIKey + "methodflickr.auth.getFrob";
	QByteArray md5 = QCryptographicHash::hash(tempString.toUtf8(), QCryptographicHash::Md5);
	QString api_sig(md5.toHex().constData());

	flickrUrl += QString("&api_sig=") + api_sig;

	//

	QNetworkRequest request = ShareLibrary::createSSLRequest(flickrUrl);
	QNetworkReply* pTempReply = Singleton<QNetworkAccessManager>::instance().get(request);
	if (pTempReply)
	{
		replyTempObjectManager().addTempReply(pTempReply);
		connect(pTempReply, SIGNAL(finished()), this, SLOT(onReplayFinishedGetFrob()));
	}
	else
	{
		onPageLoadFinished(ShareLibrary::Result_FailedToAuth);
	}
}

void CFlickrAuthorizeWebview::onReplayFinishedGetFrob()
{
	QNetworkReply* rep = dynamic_cast<QNetworkReply*>(sender());

	if (rep && rep->error() == QNetworkReply::NoError)
	{
		QString all = QString::fromUtf8(rep->readAll());

		if (all.contains("frob"))
		{
			int fisrt = all.indexOf("frob");
			int last = all.lastIndexOf("frob");
			QString frob1 = all.mid(fisrt + 5, (last - 2) - (fisrt + 5));
			m_strForb = frob1;

			const QString url = authorizationUrl();
			
			loadUrl(url);
		}
		else
		{
			onPageLoadFinished(ShareLibrary::Result_FailedToAuth);
		}
	}
	else
	{
		onPageLoadFinished(ShareLibrary::Result_FailedToAuth);
	}
}

void CFlickrAuthorizeWebview::clearToken()
{
	m_strUserID.clear();
	m_strTokenKey.clear();
}

