#include "FaceBookAuthorizeWebview.h"
#include "SharePublicFuncDefine.h"
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
CFaceBookAuthorizeWebview::CFaceBookAuthorizeWebview(const QString& clientID, QWidget *parent)
: CAuthorizeWebviewBase(parent)
, m_strClientID(clientID)
{
}


CFaceBookAuthorizeWebview::~CFaceBookAuthorizeWebview()
{

}


void CFaceBookAuthorizeWebview::onPageLoadFinished(ShareLibrary::EPageLoadResult eResult)
{
	CAuthorizeWebviewBase::onPageLoadFinished(eResult);

	if (ShareLibrary::Result_Success == eResult)
	{
		const QString strUrl = urlString();

		qDebug() << "Facebook page load: " << strUrl;

		if (!strUrl.isEmpty())
		{
			if (strUrl.contains("login.php?skip_api_login="))
			{
				//登录字符串中解析“取消”的Url
				QString strKey1("cancel_url=");
				QString strKey2("&display=popup");
				int nStartIndex = strUrl.indexOf(strKey1);
				int nEndIndex = strUrl.indexOf(strKey2);

				m_strCancelUrl = strUrl.mid(nStartIndex + strKey1.size(), strUrl.size() - nStartIndex - strKey1.size() - strKey2.size());
				m_strCancelUrl = QUrl::fromPercentEncoding(m_strCancelUrl.toLocal8Bit());
				
				return;
			}

			if (strUrl == m_strCancelUrl)
			{

				close();

				onPageLoadFinished(ShareLibrary::Result_UserCancel);
				emit userCancel();


				return;
			}

			if (strUrl.contains(QString("access_token")))
			{
				QStringList retList = strUrl.split("#");
				if (retList.count() == 0)
				{
					return;
				}
				QString ret = retList.at(1);

				//access_token=XXXX & expires_in=YYYY
				QStringList listInfo = ret.split("&");

				if (listInfo.size() < 2)
				{
					return;
				}

				QString key = listInfo.at(0); //token
				m_strTokenKey = key;

				QString strExpires = listInfo[1].split("=")[1];
				m_nExpiresTimeStamp = QDateTime::currentDateTime().toTime_t() + strExpires.toUInt() - 10;

				if (!m_strTokenKey.isEmpty() && m_nExpiresTimeStamp > 0)
				{
					setResultCode(ShareLibrary::Result_Success);
					setState(ShareLibrary::ShowSuccess);

					emit tokenReceived();

					startDelayClose();
				}
				else
				{
					onPageLoadFinished(ShareLibrary::Result_OtherError);
				}
			}
		}
	}
	
}

void CFaceBookAuthorizeWebview::startToAuthorize()
{
	CAuthorizeWebviewBase::startToAuthorize();

	setState(ShareLibrary::ShowDefault);

	QString url = authorizationUrl();
	
	loadUrl(url);
}

QString CFaceBookAuthorizeWebview::authorizationUrl() const
{
	QString urls("https://www.facebook.com/dialog/oauth?");
	urls += QString("client_id=%1&").arg(m_strClientID);
	urls += QString("redirect_uri=https://www.facebook.com/connect/login_success.html&");
	urls += QString("response_type=token&");
	urls += QString("display=popup&");
	urls += QString("scope=user_photos, publish_actions");

	return urls;
}

void CFaceBookAuthorizeWebview::onPageLoadProcess(int process)
{
	CAuthorizeWebviewBase::onPageLoadProcess(process);

	if (isFirstLoad())
	{
		setState(ShareLibrary::ShowProcess);
	}
}

void CFaceBookAuthorizeWebview::reset()
{
	setState(ShareLibrary::ShowDefault);

	loadUrl("");
}

void CFaceBookAuthorizeWebview::clearToken()
{
	m_strTokenKey.clear();
	m_nExpiresTimeStamp = 0;
}
