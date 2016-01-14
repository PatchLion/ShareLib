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
	for (int nIndex = 0; nIndex < m_listReply.size();nIndex++)
	{
		if (m_listReply[nIndex]->isRunning())
		{
			m_listReply[nIndex]->disconnect();
			m_listReply[nIndex]->abort();
		}
	}
}

bool CNetworkReplyTempObjManager::addTempReply(QNetworkReply* pReply, int nTimeOut)
{
	if (!pReply)
	{
		return false;
	}

	pReply->setParent(this);

	if (nTimeOut > 0)
	{
		qDebug() << "Start reply timeout timer!";
		QTimer::singleShot(nTimeOut, pReply, SLOT(abort()));
	}

	return true;
}
