#include "FlickrShareObject.h"
#include "ShareStructDefine.h"
#include "SharePublicFuncDefine.h"
#include <QtNetwork/QNetworkReply>
#include <QtCore/QFile>
#include <QtCore/QTimer>
#include "sharefunmacro.h"
CFlickrShareObject::CFlickrShareObject(const QString& secret, const QString& apiKey, QObject *parent/*=0*/)
	: CShareFrameBase(parent)
	, m_strAPIKey(apiKey)
	, m_strSecret(secret)
{
	
}
 

CFlickrShareObject::~CFlickrShareObject()
{
}

void CFlickrShareObject::refreshAlbumList()
{
		//url
		QString strGetAlbumUrl = QString("https://api.flickr.com/services/rest/?method=flickr.photosets.getList&api_key=%1&user_id=%2").arg(m_strAPIKey).arg(m_strUserID);

		QNetworkRequest request = ShareLibrary::createSSLRequest(strGetAlbumUrl);

		QNetworkReply* pTempReply = networkAccessManager().get(request);

		if (pTempReply)
		{
			pTempReply->setParent(&networkAccessManager());
            START_REPLY_TIMER(pTempReply, DEFAULT_TIMEOUT_INTERVAL);
            //QTimer::singleShot(DEFAULT_TIMEOUT_INTERVAL, pTempReply, SLOT(abort()));
			//replyTempObjectManager().addTempReply(pTempReply);
			connect(pTempReply, SIGNAL(finished()), this, SLOT(onReplayFinishedAlbum()));
		}
		else
		{
			emit albumsInfoRefreshFinished(false, ShareLibrary::MapAlbumInfo());
		}
}

void CFlickrShareObject::onReplayFinishedAlbum()
{
	QNetworkReply* rep = dynamic_cast<QNetworkReply*>(sender());

	ShareLibrary::MapAlbumInfo mapAlbumInfo;
	bool bSuccess = false;
	if (rep)
	{
        STOP_REPLY_TIMER(rep);
		if (QNetworkReply::NoError == rep->error())
		{
			bSuccess = ShareLibrary::readFlickrAlbumsInfoByByteArray(rep->readAll(), mapAlbumInfo);
		}
        else
        {
            qWarning() << "Flickr album reply: " << rep->errorString();
        }
	}
	emit albumsInfoRefreshFinished(bSuccess, mapAlbumInfo);
}


void CFlickrShareObject::onReplayFinishedUpload()
{
	QNetworkReply* rep = dynamic_cast<QNetworkReply*>(sender());

	if (!rep)
	{
		emit shareFinished(false, tr("Inner problem!", "ShareLib"));
		return;
	}

    STOP_REPLY_TIMER(rep);

	QNetworkReply::NetworkError errorCode = rep->error();
	if (QNetworkReply::NoError == errorCode)
	{
		QString all = QString::fromUtf8(rep->readAll());

		if (all.contains("photoid"))
		{
			int firstindex = all.indexOf("<photoid>");
			int endindex = all.indexOf("</photoid>");
			m_strCurrentPhotoID = all.mid(firstindex + 9, endindex - firstindex - 9);
			startUploadPhotoToAlbum();
		}
		else
		{
			qWarning() << all;
			emit shareFinished(false, tr("Return error from Flickr!", "ShareLib"));
		}
	}
	else
	{
		switch (errorCode)
		{
		case QNetworkReply::OperationCanceledError:
		{
			if (isUserCancel())
			{
				emit shareFinished(false, tr("Cancelled by user!", "ShareLib").arg(rep->error()));
			}
			else
			{
				emit shareFinished(false, tr("Request time out!", "ShareLib").arg(rep->error()));
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

bool CFlickrShareObject::shareToFlickr(const QString& strTitle, const QString& strDescriptionStr, const QString& strAlbumsStr, const QString& strFilePath, int nPrivacy)
{
	m_strTempAlbumID = strAlbumsStr;

	QFile file(strFilePath);

	if (file.open(QIODevice::ReadOnly))
	{
		QString flickrUrl("https://api.flickr.com/services/upload/");
		QString filename = strFilePath;

		int nIsFamily, nIsFriend, nIsPublic;
		ShareLibrary::getFlagValueByPrivacy(nPrivacy, nIsFamily, nIsFriend, nIsPublic);

		QString boundary("---------------------------7d44e178b0434");
		QString contentType("multipart/form-data; boundary=" + boundary);
		QByteArray data;
		QString test = m_strSecret + "api_key" + m_strAPIKey;
		test += QString("auth_token") + m_strTokenKey;
		test += QString("description") + strDescriptionStr.toUtf8();
		test += QString("is_family") + QString::number(nIsFamily);
		test += QString("is_friend") + QString::number(nIsFriend);
		test += QString("is_public") + QString::number(nIsPublic);
		test += QString("title") + strTitle.toUtf8();

		QByteArray array1;
		array1.append(test);

		QByteArray md5 = QCryptographicHash::hash(array1, QCryptographicHash::Md5);
		QString api_sig1(md5.toHex().constData());
		QString url;
		data += QString("--" + boundary + "\r\n").toUtf8();
		data += QString("Content-Disposition: form-data; name=\"api_key\"\r\n\r\n").toUtf8();
		data += m_strAPIKey + QString("\r\n").toUtf8();				//api key
		data += QString("--" + boundary + "\r\n").toUtf8();

		data += QString("Content-Disposition: form-data; name=\"auth_token\"\r\n\r\n").toUtf8();
		data += m_strTokenKey + QString("\r\n").toUtf8();				//
		data += QString("--" + boundary + "\r\n").toUtf8();

		data += QString("Content-Disposition: form-data; name=\"description\"\r\n\r\n").toUtf8();
		data += QString(strDescriptionStr.toUtf8() + "\r\n");
		data += QString("--" + boundary + "\r\n").toUtf8();

		data += QString("Content-Disposition: form-data; name=\"title\"\r\n\r\n").toUtf8();
		data += QString(strTitle.toUtf8() + "\r\n");
		data += QString("--" + boundary + "\r\n").toUtf8();

		data += QString("Content-Disposition: form-data; name=\"is_family\"\r\n\r\n").toUtf8();
		data += QString(QString::number(nIsFamily) + "\r\n").toUtf8();
		data += QString("--" + boundary + "\r\n").toUtf8();

		data += QString("Content-Disposition: form-data; name=\"is_friend\"\r\n\r\n").toUtf8();
		data += QString(QString::number(nIsFriend) + "\r\n").toUtf8();
		data += QString("--" + boundary + "\r\n").toUtf8();

		data += QString("Content-Disposition: form-data; name=\"is_public\"\r\n\r\n").toUtf8();
		data += QString(QString::number(nIsPublic) + "\r\n").toUtf8();
		data += QString("--" + boundary + "\r\n").toUtf8();

		data += QString("Content-Disposition: form-data; name=\"api_sig\"\r\n\r\n").toUtf8();
		data += QString(api_sig1 + "\r\n");				//api_sig
		data += QString("--" + boundary + "\r\n").toUtf8();

		data += QString("Content-Disposition: form-data; name=\"photo\"; filename=\"" + filename + "\"\r\n").toUtf8();
		data += QString("Content-Type: image/jpeg\r\n\r\n").toUtf8();

		data += file.readAll();
		data += "\r\n";
		data += QString("--" + boundary + "--\r\n").toUtf8();

		QNetworkRequest request = ShareLibrary::createSSLRequest(flickrUrl);
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(contentType));
		request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(QString::number(data.size())));

		QNetworkReply* pTempReply = networkAccessManager().post(request, data);

		if (pTempReply)
		{
			pTempReply->setParent(&networkAccessManager());
            START_REPLY_TIMER(pTempReply, DEFAULT_TIMEOUT_INTERVAL);
            //QTimer::singleShot(DEFAULT_TIMEOUT_INTERVAL, pTempReply, SLOT(abort()));
			//replyTempObjectManager().addTempReply(pTempReply, SHARE_IMAGE_TIMEOUT);
			connect(pTempReply, SIGNAL(finished()), this, SLOT(onReplayFinishedUpload()));
		}
		else
		{
			emit shareFinished(false, tr("Failed to create network request!", "ShareLib"));
		}
	}
	else
	{
		emit shareFinished(false, tr("Can not open the image file!", "ShareLib"));
	}


	return false;
}

bool CFlickrShareObject::share(IShareParam* pParam)
{

	if (!CShareFrameBase::share(pParam))
	{
		return false;
	}

	CFlickrShareParam* pFlickrParam = dynamic_cast<CFlickrShareParam*>(pParam);

	if (pFlickrParam->albumID.isEmpty())
	{
		emit shareFinished(false, tr("You must select one album!", "ShareLib"));
		return false;
	}

	if (pFlickrParam)
	{
		return shareToFlickr(pFlickrParam->title, pFlickrParam->description, pFlickrParam->albumID, pFlickrParam->filePath, pFlickrParam->privacyType);
	}

	return false;
}

void CFlickrShareObject::setTokenKey(const QString& tokenKey)
{
	m_strTokenKey = tokenKey;
}

void CFlickrShareObject::startUploadPhotoToAlbum()
{
	if (m_strCurrentPhotoID.isEmpty())
	{
		emit shareFinished(false, tr("Inner problem!", "ShareLib"));
		return;
	}

	QString flickrUrl("https://api.flickr.com/services/rest/?method=flickr.photosets.addPhoto");

	QString test = m_strSecret + "api_key" + m_strAPIKey + "auth_token" + m_strTokenKey + "methodflickr.photosets.addPhoto" + "photo_id" + m_strCurrentPhotoID + "photoset_id" + m_strTempAlbumID;

	QByteArray md5 = QCryptographicHash::hash(test.toUtf8(), QCryptographicHash::Md5);
	QString api_sig1(md5.toHex().constData());

	QString boundary("---------------------------7d44e17sdf8b0434");
	QString contentType("multipart/form-data; boundary=" + boundary);
	QByteArray data;

	QString url;
	data += QString("--" + boundary + "\r\n").toUtf8();
	data += QString("Content-Disposition: form-data; name=\"api_key\"\r\n\r\n").toUtf8();
	data += m_strAPIKey + QString("\r\n").toUtf8();				//api key
	data += QString("--" + boundary + "\r\n").toUtf8();

	data += QString("Content-Disposition: form-data; name=\"auth_token\"\r\n\r\n").toUtf8();
	data += m_strTokenKey + QString("\r\n").toUtf8();				//
	data += QString("--" + boundary + "\r\n").toUtf8();

	data += QString("Content-Disposition: form-data; name=\"photoset_id\"\r\n\r\n").toUtf8();
	data += QString(m_strTempAlbumID + "\r\n").toUtf8();
	data += QString("--" + boundary + "\r\n").toUtf8();

	data += QString("Content-Disposition: form-data; name=\"photo_id\"\r\n\r\n").toUtf8();
	data += QString(m_strCurrentPhotoID + "\r\n").toUtf8();
	data += QString("--" + boundary + "\r\n").toUtf8();

	data += QString("Content-Disposition: form-data; name=\"api_sig\"\r\n\r\n").toUtf8();
	data += QString(api_sig1 + "\r\n");				//api_sig
	data += QString("--" + boundary + "\r\n").toUtf8();

	data += QString("--" + boundary + "--\r\n").toUtf8();


	QNetworkRequest request = ShareLibrary::createSSLRequest(flickrUrl);
	request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(contentType));
	request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(QString::number(data.size())));

	QNetworkReply* pTempReply = networkAccessManager().post(request, data);

	if (pTempReply)
	{
		pTempReply->setParent(&networkAccessManager());
        //QTimer::singleShot(DEFAULT_TIMEOUT_INTERVAL, pTempReply, SLOT(abort()));
        START_REPLY_TIMER(pTempReply, DEFAULT_TIMEOUT_INTERVAL);
		//replyTempObjectManager().addTempReply(pTempReply, SHARE_IMAGE_TIMEOUT);
		connect(pTempReply, SIGNAL(finished()), this, SLOT(onReplayFinishedTransform()));
	}
	else
	{
		emit shareFinished(false, tr("Failed to create network request!", "ShareLib"));
	}

}

void CFlickrShareObject::onReplayFinishedTransform()
{
	QNetworkReply* rep = dynamic_cast<QNetworkReply*>(sender());

	if (!rep)
	{
		emit shareFinished(false, tr("Inner problem!", "ShareLib"));
		return;
	}

    STOP_REPLY_TIMER(rep);

	if (QNetworkReply::NoError == rep->error())
	{
		QString all = QString::fromUtf8(rep->readAll());

		if (all.contains("rsp stat=\"ok\""))
		{
			emit shareFinished(true, "");
		}
		else
		{
			qWarning() << all;
			emit shareFinished(false, tr("Return error from Flickr!", "ShareLib"));
		}
	}
	else
	{
		QNetworkReply::NetworkError errorCode = rep->error();
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

