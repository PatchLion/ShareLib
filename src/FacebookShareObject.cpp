#include "FacebookShareObject.h"
#include "ShareStructDefine.h"
#include "SharePublicFuncDefine.h"
#include <QtNetwork/QNetworkReply>
#include <QtCore/QFile>
#include <QtCore/QTimer>

CFacebookShareObject::CFacebookShareObject( QObject *parent/*=0*/ )
	: CShareFrameBase(parent)
{
}
 

CFacebookShareObject::~CFacebookShareObject()
{
}

void CFacebookShareObject::refreshAlbumList()
{
	QString getAlbumUrl = QString("https://graph.facebook.com/v2.3/me/albums?%1").arg(m_strTokenKey);

	QNetworkRequest request = ShareLibrary::createSSLRequest(getAlbumUrl);

	QNetworkReply* pTempReply = networkAccessManager().get(request);

	if (pTempReply)
	{
		pTempReply->setParent(&networkAccessManager());
		QTimer::singleShot(DEFAULT_TIMEOUT_INTERVAL, pTempReply, SLOT(abort()));
		//replyTempObjectManager().addTempReply(pTempReply);
		connect(pTempReply, SIGNAL(finished()), this, SLOT(onReplayFinishedAlbum()));
	}
	else
	{
		emit albumsInfoRefreshFinished(false, ShareLibrary::MapAlbumInfo());
	}
}

void CFacebookShareObject::onReplayFinishedAlbum()
{
	QNetworkReply* rep = dynamic_cast<QNetworkReply*>(sender());

	ShareLibrary::MapAlbumInfo mapAlbumInfo;
	bool bSuccess = false;
	if (rep)
	{
		
		if (QNetworkReply::NoError == rep->error())
		{
			bSuccess = ShareLibrary::readFacebookAlbumsInfoByByteArray(rep->readAll(), mapAlbumInfo);
		}
	}

	emit albumsInfoRefreshFinished(bSuccess, mapAlbumInfo);
}

void CFacebookShareObject::onReplayFinishedUpload()
{

	QNetworkReply* rep = dynamic_cast<QNetworkReply*>(sender());
	if (rep)
	{
		QNetworkReply::NetworkError errorCode = rep->error();
		if (QNetworkReply::NoError == errorCode)
		{
			QString all = QString::fromUtf8(rep->readAll());

			if (all.contains("\"id\""))
			{
				emit shareFinished(true, "");
			}
			else if (all.contains("error") || all.isEmpty())
			{
				qWarning() << all;
				emit shareFinished(false, tr("Return error from Facebook!", "ShareLib"));
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
	else
	{
		emit shareFinished(false, tr("Inner problem!", "ShareLib"));
	}
}

bool CFacebookShareObject::shareToFacebook(const QString& strDescriptionStr, const QString& strAlbumsStr, const QString& strFilePath)
{
	QFile file(strFilePath);
	
	if (file.open(QIODevice::ReadOnly))
	{
		QString faceBookUpLoadImageUrl("");
		if (strAlbumsStr.isEmpty())
		{
			faceBookUpLoadImageUrl = QString("https://graph.facebook.com/v2.3/me/photos?%1").arg(m_strTokenKey);
		}
		else
		{
			faceBookUpLoadImageUrl = QString("https://graph.facebook.com/v2.3/"+strAlbumsStr+"/photos?%1").arg(m_strTokenKey);
		}

		QString boundary("----eidevelop1010101010");
		QString contentType("multipart/form-data; boundary="+boundary);
		QByteArray data;

		// open file
		QString fileName = strFilePath;
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly))
		{
			return false;
		}
		data += QString("--"+boundary + "\r\n").toUtf8();
		data += QString("Content-Disposition: form-data; name=\"message\"\r\n\r\n").toUtf8();
		data += strDescriptionStr.toUtf8() + QString("\r\n");
		data += QString("--"+boundary + "\r\n").toUtf8();
		data += QString("Content-Disposition: form-data; name=\"source\";filename="+fileName+"\r\n").toUtf8();
		data += QString("Content-Type: image/jpeg\r\n\r\n").toUtf8();
		data += file.readAll();
		data += "\r\n";
		data += QString("--" + boundary + "\r\n").toUtf8();
		data += QString("--" + boundary + "\r\n").toUtf8();

		QNetworkRequest request = ShareLibrary::createSSLRequest(faceBookUpLoadImageUrl);
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(contentType));
		request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(QString::number(data.size())));

		QNetworkReply* pTempReply = networkAccessManager().post(request, data);

		if (pTempReply)
		{
			pTempReply->setParent(&networkAccessManager());
			QTimer::singleShot(DEFAULT_TIMEOUT_INTERVAL, pTempReply, SLOT(abort()));
			//replyTempObjectManager().addTempReply(pTempReply, SHARE_IMAGE_TIMEOUT);
			connect(pTempReply, SIGNAL(finished()), this, SLOT(onReplayFinishedUpload()));

			return true;
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

bool CFacebookShareObject::share(IShareParam* pParam)
{
	if (!CShareFrameBase::share(pParam))
	{
		return false;
	}

	CFacebookShareParam* pFacebookParam = dynamic_cast<CFacebookShareParam*>(pParam);

	if (pFacebookParam)
	{
		return shareToFacebook(pFacebookParam->description, pFacebookParam->albumID, pFacebookParam->filePath);
	}

	return false;
}

void CFacebookShareObject::setTokenKey(const QString& tokenKey)
{
	m_strTokenKey = tokenKey;
}

