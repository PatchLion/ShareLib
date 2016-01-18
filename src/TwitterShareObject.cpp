#include "TwitterShareObject.h"
#include "ShareStructDefine.h"
#include "SharePublicFuncDefine.h"
#include <QtNetwork/QNetworkReply>
#include <QtCore/QFile>
#include <QtNetwork/QHttpMultiPart>
#include "TwitterShare.h"
#include <QtCore/QTimer>

CTwitterShareObject::CTwitterShareObject(const QString& strConsumerKey, /*在twitter上申请的用户Key */ const QString& strConsumerSecret, /*在twitter上申请的用户秘钥 */ QObject *parent /*= 0*/)
	: CShareFrameBase(parent)
	, m_strConsumerKey(strConsumerKey)
	, m_strConsumerSecret(strConsumerSecret)
{
}

CTwitterShareObject::~CTwitterShareObject()
{
}

bool CTwitterShareObject::shareToTwitter(const QString& strDescriptionStr, const QString& strFilePath)
{
	QFile* pFile = new QFile(strFilePath);

	if (!pFile->open(QIODevice::ReadOnly))
	{
		delete pFile;
		pFile = 0;
		emit shareFinished(false, "Can not open the image file!");

		return false;
	}

	QUrl url("https://api.twitter.com/1.1/statuses/update_with_media.json");

	QByteArray oauthHeader = CTwitterShare::generateAuthorizationHeader(url, ShareLibrary::Post, m_strOauthToken, m_strOauthTokenSecret, m_strConsumerKey, m_strConsumerSecret);

	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	QHttpPart statusPart;
	statusPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"status\""));
	statusPart.setBody(strDescriptionStr.toUtf8());
	multiPart->append(statusPart);

	QHttpPart imagePart;
	imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"media[]\""));
	imagePart.setBodyDevice(pFile);
	pFile->setParent(multiPart);
	multiPart->append(imagePart);

	QString strPlaceID = "df51dec6f4ee2b2c";

	if (!strPlaceID.isEmpty()) {
		QHttpPart placeIDPart;
		placeIDPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"place_id\""));
		placeIDPart.setBody(strPlaceID.toUtf8());
		multiPart->append(placeIDPart);
	}

	if (true) {
		QHttpPart displayCoordinatesPart;
		displayCoordinatesPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"display_coordinates\""));
		displayCoordinatesPart.setBody("true");
		multiPart->append(displayCoordinatesPart);
	}

	QNetworkRequest req;
	ShareLibrary::createSSLRequest(req, url);
	req.setRawHeader("Authorization", oauthHeader);

	QNetworkReply *pTempReply = networkAccessManager().post(req, multiPart);
	multiPart->setParent(pTempReply);

	if (pTempReply)
	{
		pTempReply->setParent(&networkAccessManager());
		QTimer::singleShot(DEFAULT_TIMEOUT_INTERVAL, pTempReply, SLOT(abort()));
		//replyTempObjectManager().addTempReply(pTempReply, SHARE_IMAGE_TIMEOUT);
		connect(pTempReply, SIGNAL(finished()), this, SLOT(onReplyFinishedUpload()));
		return true;
	}
	else
	{
		emit shareFinished(false, tr("Failed to create network request!", "ShareLib"));
		return false;
	}
}

void CTwitterShareObject::onReplyFinishedUpload()
{
	QNetworkReply* rep = dynamic_cast<QNetworkReply*>(sender());


	if (!rep)
	{
		emit shareFinished(false, tr("Inner problem!", "ShareLib"));
		return;
	}

	QNetworkReply::NetworkError errorCode = rep->error();
	if (QNetworkReply::NoError == errorCode)
	{
		QString all = QString::fromUtf8(rep->readAll());

		if (all.contains("error") || all.isEmpty())
		{
			qWarning() << all;
			emit shareFinished(false, tr("Return error from Twitter!", "ShareLib"));
		}
		else if (all.contains("id") && all.contains("source"))
		{
			emit shareFinished(true, "");
		}
	}
	else
	{
		switch (errorCode)
		{
		case QNetworkReply::OperationCanceledError:
		{
			if (!isUserCancel())
			{
				emit shareFinished(false, tr("Request time out!", "ShareLib"));
			}
			else
			{
				emit shareFinished(false, tr("Cancelled by user!", "ShareLib"));
			}
			
		}
		break;
		default:
		{
			emit shareFinished(false, tr("Request error code: %1", "ShareLib").arg(rep->error()));
		}
		break;
		}
	}

}

bool CTwitterShareObject::share(IShareParam* pParam)
{
	if (!CShareFrameBase::share(pParam))
	{
		return false;
	}

	CTwitterShareParam* pTwitterParam = dynamic_cast<CTwitterShareParam*>(pParam);

	if (pTwitterParam)
	{
		return shareToTwitter(pTwitterParam->description, pTwitterParam->filePath);
	}

	return false;
	
}
