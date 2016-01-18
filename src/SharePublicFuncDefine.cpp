#include "SharePublicFuncDefine.h"
#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QCryptographicHash>
#include <QtNetwork/QSslConfiguration>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
/*

QString ShareLibrary::getStateDescription( ShareLibrary::EAuthorizeState eState )
{
	switch(eState)
	{
	case ShareLibrary::State_Init:							//初始化错误
		{
			return QObject::tr("initialize failed");
		}
		break;

	case ShareLibrary::State_Unauthorized:										//初始状态
		{
			return QObject::tr("Unauthorized");
		}
		break;

	case ShareLibrary::State_Authorizing:							//获取Token中
		{
			return QObject::tr("request access token");
		}
		break;

	case ShareLibrary::State_CheckOldAuthorization:					//检验Token有效性中
		{
			return QObject::tr("verify token");
		}
		break;

	case ShareLibrary::State_Authorised:							//准备完毕(登录, token有效性验证成功)
		{
			return QObject::tr("ready");
		}
		break;


	case ShareLibrary::State_PreAuthorize:									//获取Frob中(Flickr特有)
		{
			return QObject::tr("prepare authorize");
		}
		break;


	default:
		{
			return QObject::tr("Unknown state");
		}
		break;
	}
}*/
/*

QString ShareLibrary::getErrorCodeDescription( ShareLibrary::EShareResultCode eCode )
{
	switch(eCode)
	{	
	case ShareLibrary::Succeed:										//成功
		{
			return QObject::tr("succeed");
		}
		break;
	case ShareLibrary::Error_InnerError:										//程序内部错误
		{
			return QObject::tr("application error");
		}
		break;

	case ShareLibrary::Error_InvalidAuth:									//无效的权限
		{
			return QObject::tr("unauthorized");
		}
		break;

	case ShareLibrary::Error_NetworkDisconnected:				//无网络连接
		{
			return QObject::tr("network disconnection");
		}
		break;

	case ShareLibrary::Error_OpenFileFailed:							//打开文件失败
		{
			return QObject::tr("open file failed");
		}
		break;

	case ShareLibrary::Error_GetAlbumsFailed:						//获取相册失败
		{
			return QObject::tr("get albums failed");
		}
		break;

	case ShareLibrary::Error_TimeOut:									//超时
		{
			return QObject::tr("time out");
		}
		break;

	case ShareLibrary::Error_SSLError:										//SSL错误
		{
			return QObject::tr("ssl error");
		}
		break;

	case ShareLibrary::Error_Other:												//其他错误
		{
			return QObject::tr("other error");
		}
		break;

	case ShareLibrary::Error_ParamError:												//参数错误
		{
			return QObject::tr("invaild param");
		}
		break;

	case ShareLibrary::Error_InvaildState:												//无效的状态
		{
			return QObject::tr("invaild state");
		}
		break;

	case ShareLibrary::Error_GetFrobFailed:												//获取Frob失败(Flickr特有)
		{
			return QObject::tr("failed to get frob");
		}
		break;

	case ShareLibrary::Error_GetAccessTokenFailed:												//获取Token失败
		{
			return QObject::tr("failed to get access token");
		}
		break;

	case ShareLibrary::Error_UploadImageFailed:												//上传照片失败
		{
			return QObject::tr("failed to upload image");
		}
		break;

	case ShareLibrary::Error_GetOauthTokenFailed:												//获取oauth token失败
		{
			return QObject::tr("failed to request ouath token");
		}
		break;

	case ShareLibrary::Error_UploadImageToAblumFailed:												//上传照片到相册失败
		{
			return QObject::tr("failed to upload image to ablum");
		}
		break;

	default:
		{
			return QObject::tr("Unknown problem!");
		}
		break;
	}
}*/

void ShareLibrary::moveWidgetToScreenCenter(QWidget* pWidget)
{

	if (pWidget)
	{
		QDesktopWidget* desktop = QApplication::desktop();

		QPoint pt = QPoint((desktop->width() - pWidget->width()) / 2, (desktop->height() - pWidget->height()) / 2);
		if (pWidget->parentWidget())
		{
			pt = pWidget->parentWidget()->mapFromGlobal(pt);
		}
		pWidget->move(pt);
	}
}
QString ShareLibrary::getFlickrPrivacyDescription( int nPrivacy )
{
	switch(nPrivacy)
	{
	case Public:
		{
			return QObject::tr("Public");
		}
		break;

	case Friend:
		{
			return QObject::tr("Friend");
		}
		break;

	case Family:
		{
			return QObject::tr("Family");
		}
		break;

	case Friend_and_Family:
		{
			return QObject::tr("Friend and Family");
		}
		break;

	case Privacy:
		{
			return QObject::tr("Privacy");
		}
		break;

	default:
		{
			return QObject::tr("Unknown privacy");
		}
		break;
	}
}

void ShareLibrary::getFlagValueByPrivacy( int nPrivacy, int& isFamily, int& isFriend, int& isPublic )
{
	switch(nPrivacy)
	{
	case Public:
		{
			isPublic=1;
			isFriend=0;
			isFamily=0;
		}
		break;

	case Friend:
		{
			isPublic=0;
			isFriend=1;
			isFamily=0;
		}
		break;

	case Family:
		{
			isPublic=0;
			isFriend=0;
			isFamily=1;
		}
		break;

	case Friend_and_Family:
		{
			isFamily=1;
			isFriend=1;
			isPublic=0;
		}
		break;

	case Privacy:
		{
			isPublic=0;
			isFriend=0;
			isFamily=0;
		}
		break;

	default:
		{
			isPublic=1;
			isFriend=0;
			isFamily=0;
		}
		break;
	}
}

QString ShareLibrary::generateTimeStampString()
{
	const uint seconds = generateTimeStamp();

	return QString("%1").arg(seconds);
}

uint ShareLibrary::generateTimeStamp()
{
	//OAuth spec. 8 http://oauth.net/core/1.0/#nonce
	QDateTime current = QDateTime::currentDateTime();
	const uint seconds = current.toTime_t();

	return seconds;
}

QByteArray ShareLibrary::generateNonce()
{
	//OAuth spec. 8 http://oauth.net/core/1.0/#nonce
	QByteArray chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	int max = chars.size();

	int len = 16;

	QByteArray nonce;
	for(int i = 0; i < len; ++i){
		nonce.append( chars[qrand() % max] );
	}

	return nonce;
}

QByteArray ShareLibrary::hmacSha1(const QByteArray& message, const QByteArray& key)
{
	QByteArray normKey;

	if (key.size() > 64) {
		normKey = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
	} else {
		normKey = key; // no need for zero padding ipad and opad are filled with zeros
	}

	unsigned char* K = (unsigned char *)normKey.constData();

	unsigned char ipad[65];
	unsigned char opad[65];

	memset(ipad, 0, 65);
	memset(opad, 0, 65);

	memcpy(ipad, K, normKey.size());
	memcpy(opad, K, normKey.size());

	for (int i = 0; i < 64; ++i) {
		ipad[i] ^= 0x36;
		opad[i] ^= 0x5c;
	}

	QByteArray context;
	context.append((const char *)ipad, 64);
	context.append(message);

	QByteArray sha1 = QCryptographicHash::hash(context, QCryptographicHash::Sha1);

	context.clear();
	context.append((const char *)opad, 64);
	context.append(sha1);

	sha1.clear();

	sha1 = QCryptographicHash::hash(context, QCryptographicHash::Sha1);

	return sha1;
}

QString ShareLibrary::getHttpTypeString( ShareLibrary::HttpMethod eMethod )
{
	switch(eMethod)
	{
	case ShareLibrary::Get:
		{
			return "GET";
		}
		break;

	case ShareLibrary::Post:
		{
			return "POST";
		}
		break;

	case ShareLibrary::Put:
		{
			return "PUT";
		}
		break;

	case ShareLibrary::Delete:
		{
			return "DELETE";
		}
		break;
	}

	return "";
}

QNetworkRequest ShareLibrary::createSSLRequest(const QString& strUrl)
{
	QNetworkRequest request;

	createSSLRequest(request, QUrl(strUrl));

	return request;
}

void ShareLibrary::createSSLRequest(QNetworkRequest& request, const QUrl& url)
{
	request.setUrl(url);

	QSslConfiguration sslConfig = request.sslConfiguration();
	sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
	sslConfig.setProtocol(QSsl::AnyProtocol);

	request.setSslConfiguration(sslConfig);
}

QString ShareLibrary::getPageLoadResultString(EPageLoadResult eResult)
{
	switch (eResult)
	{
	case ShareLibrary::Result_Success:				return QObject::tr("Success!", "ShareLib");
	case ShareLibrary::Result_TimeOut:			return QObject::tr("Time out!", "ShareLib");
	case ShareLibrary::Result_SSLError:				return QObject::tr("SSL Error!!", "ShareLib");
	case ShareLibrary::Result_InvaildURL:			return QObject::tr("Invalid URL!", "ShareLib");
	case ShareLibrary::Result_OtherError:		return QObject::tr("Failed to load page!", "ShareLib");
	case ShareLibrary::Result_FailedToAuth:	return QObject::tr("Failed to authorize!", "ShareLib");
	case ShareLibrary::Result_UserCancel:		return QObject::tr("Cancelled by user!", "ShareLib");
	default:															return QObject::tr("Unknown problem!", "ShareLib");
	}
}

bool ShareLibrary::readFacebookAlbumsInfoByByteArray(const QByteArray& json, ShareLibrary::MapAlbumInfo& mapAlbumInfo)
{
	mapAlbumInfo.clear();

	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(json, &json_error);
	if (json_error.error != QJsonParseError::NoError)
	{
		return false;
	}
	if (!parse_doucment.isObject())
	{
		return false;
	}

	QJsonObject obj = parse_doucment.object();
	if (!obj.contains("data"))
	{
		return false;
	}

	QJsonArray albumArray = obj.value("data").toArray();

	if (!albumArray.isEmpty())
	{
		for (int nIndex = 0; nIndex < albumArray.size(); nIndex++)
		{
			QJsonValue classValue = albumArray.at(nIndex);

			if (classValue.isObject())
			{
				QJsonObject tempObject = classValue.toObject();
				bool bCanUpload = false;
				QString strID, strName;

				if (tempObject.contains("can_upload"))
				{
					bCanUpload = tempObject.value("can_upload").toBool();
				}
				if (tempObject.contains("id"))
				{
					strID = tempObject.value("id").toString();
				}
				if (tempObject.contains("name"))
				{
					strName = tempObject.value("name").toString();
				}

				if (bCanUpload && !strID.isEmpty() && !strName.isEmpty())
				{
					mapAlbumInfo.insert(strID, strName);
				}
			}
		}

		
	}
	return true;
}

bool ShareLibrary::readFlickrAlbumsInfoByByteArray(const QByteArray& json, MapAlbumInfo& mapAlbumInfo)
{
	mapAlbumInfo.clear();
	if (!json.isEmpty())
	{
		QString tempStr = json;
		int indexfi = tempStr.indexOf("<photosets");
		int indexla = tempStr.indexOf("<photoset id=");
		QString totalLine = tempStr.mid(indexfi, indexla - indexfi);
		indexfi = totalLine.indexOf("total=\"");
		indexla = totalLine.indexOf("\">");
		QString albumNumStr = totalLine.mid(indexfi + 7, indexla - indexfi - 7);

		int albumNum = albumNumStr.toInt();
		int first = tempStr.indexOf("<photoset id");
		int end = tempStr.lastIndexOf("</photosets>");
		QString tempsubStr = tempStr.mid(first, end - first);
		QStringList firstStr = tempsubStr.split("</photoset>");

		for (int i = 0; i < albumNum; i++)
		{
			QString tempStr1 = firstStr[i];
			int idfirst = tempStr1.indexOf("id=");
			int idend = tempStr1.indexOf(" primary=");
			QString idStr = tempStr1.mid(idfirst + 4, idend - idfirst - 5);

			int titlefisrt = tempStr1.indexOf("<title>");
			int titleend = tempStr1.indexOf("</title>");
			QString titleStr = tempStr1.mid(titlefisrt + 7, titleend - titlefisrt - 7);

			mapAlbumInfo.insert(idStr, titleStr);
		}
		return true;
	}
	
	return false;
}
