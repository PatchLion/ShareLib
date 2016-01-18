#include "ShareFrameBase.h"
#include <QtNetwork/QNetworkProxyFactory>
#include <QtNetwork/QNetworkAccessManager>

CShareFrameBase::CShareFrameBase(QObject *parent)
: QObject(parent)
, m_networkManager(0)
, m_bIsUserCancel(false)
{
	qRegisterMetaType<ShareLibrary::MapAlbumInfo>("ShareLibrary::MapAlbumInfo");
	QNetworkProxyFactory::setUseSystemConfiguration(true);
}

CShareFrameBase::~CShareFrameBase()
{

}

bool CShareFrameBase::share(IShareParam* pParam)
{
	if (!pParam)
	{
		emit shareFinished(false, tr("Invalid parameter!", "ShareLib"));
		return false;
	}

	return true;
}
