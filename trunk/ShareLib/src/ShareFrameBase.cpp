#include "ShareFrameBase.h"
#include <QtNetwork/QNetworkProxyFactory>
#include <QtNetwork/QNetworkAccessManager>

CShareFrameBase::CShareFrameBase(QObject *parent)
: QObject(parent)
, m_pAccessMgr(0)
{
	qRegisterMetaType<ShareLibrary::MapAlbumInfo>("ShareLibrary::MapAlbumInfo");

	m_pAccessMgr = new QNetworkAccessManager(this);

	QNetworkProxyFactory::setUseSystemConfiguration(true);

	connect(&replyTempObjectManager(), &CNetworkReplyTempObjManager::replyTimeOut, this, &CShareFrameBase::onReplyTimeOut, Qt::QueuedConnection);
}

CShareFrameBase::~CShareFrameBase()
{

}

bool CShareFrameBase::share(IShareParam* pParam)
{
	if (!pParam)
	{
		emit shareFinished(false, tr("Invalied param!", "ShareLib"));
		return false;
	}

	if (!networkAccessManager())
	{
		emit shareFinished(false, tr("Application error!", "ShareLib"));
		return false;
	}

	return true;
}

void CShareFrameBase::onReplyTimeOut(const QVariant& replyData)
{
	emit shareFinished(false, tr("Time out!", "ShareLib"));
}
