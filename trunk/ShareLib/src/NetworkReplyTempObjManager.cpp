#include "NetworkReplyTempObjManager.h"
#include "SharePublicFuncDefine.h"
#include "ShareMacroDefine.h"
#include <QtNetwork/QNetworkProxyFactory>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QTimer>

const char*  ROLE_REPLYDATA_EX = "ReplyData";
const char*  ROLE_REPLYDATA_OBJECT = "ReplyDataObject";
const char*  ROLE_TIMER_OBJECT = "TimerObject";

CNetworkReplyTempObjManager::CNetworkReplyTempObjManager( QObject *parent)
: QObject(parent)
{
}

CNetworkReplyTempObjManager::~CNetworkReplyTempObjManager()
{
	
}

bool CNetworkReplyTempObjManager::addTempReply(QNetworkReply* pReply, int nTimeOut, const QVariant& replyData)
{
	if (!pReply)
	{
		return false;
	}

	QMutexLocker locker(&m_mutexShareReplyList);

	MapReplyToTimer::iterator replyItor = m_mapReplyToTimer.find(pReply);

	if (m_mapReplyToTimer.end() == replyItor)
	{
		pReply->setParent(this);
		pReply->setProperty(ROLE_REPLYDATA_EX, replyData);

		if (nTimeOut > 0)
		{		
			QTimer* pTimer = new QTimer(pReply);
			connect(pTimer, &QTimer::timeout, this, &CNetworkReplyTempObjManager::onTimeOut);
		

			pTimer->setInterval(nTimeOut);
			pTimer->start();

			m_mapReplyToTimer.insert(pReply, pTimer);
		}
		else
		{
			m_mapReplyToTimer.insert(pReply, 0);
		}
	}

	return true;
}

void CNetworkReplyTempObjManager::removeTempReply( QNetworkReply* pReply )
{
	if (!pReply)
	{
		return;
	}

	QMutexLocker locker(&m_mutexShareReplyList);

	MapReplyToTimer::iterator replyItor = m_mapReplyToTimer.find(pReply);

	if (m_mapReplyToTimer.end() != replyItor)
	{
		//移除Reply对象
		QNetworkReply* pTempReply = replyItor.key();
		releaseReplyObj(pTempReply);

		//关闭Timer对象
		QTimer* pTimer = replyItor.value();
		releaseTimerObj(pTimer);

		m_mapReplyToTimer.erase(replyItor);
	}

	
	
}

void CNetworkReplyTempObjManager::onTimeOut()
{
	QMutexLocker locker(&m_mutexShareReplyList);

	QTimer* pTimer = dynamic_cast<QTimer*>(sender());

	if (pTimer)
	{
		MapReplyToTimer::iterator replyItor = m_mapReplyToTimer.begin();

		for (replyItor; m_mapReplyToTimer.end() != replyItor; replyItor++)
		{
			QTimer* pTempTimer = replyItor.value();

			if (pTempTimer == pTimer)
			{
				QNetworkReply* pReply = replyItor.key();

				if (pReply)
				{
					emit replyTimeOut(pReply->property(ROLE_REPLYDATA_EX));
				}
				releaseReplyObj(pReply);

				m_mapReplyToTimer.erase(replyItor);

				break;
			}
		}

		releaseTimerObj(pTimer);
	}
}

void CNetworkReplyTempObjManager::releaseReplyObj(QNetworkReply *pReply)
{
	if (pReply)
	{
		disconnect(pReply);
		/*
		if (pReply->isRunning())
		{
			pReply->abort();
		}

		SAFE_DELETE_QBJECT(pReply);
		*/
	}
}

void CNetworkReplyTempObjManager::releaseTimerObj(QTimer *pTimer)
{
	if (pTimer)
	{
		disconnect(pTimer);
		if (pTimer->isActive())
		{
			pTimer->stop();
		}

		//SAFE_DELETE_QBJECT(pTimer);
	}
}
