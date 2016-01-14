/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           NetworkReplyTempObjManager.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:		QNetworkReply临时对象托管
**********************************************************************************/
#ifndef NetworkReplyTempObjManager_h__
#define NetworkReplyTempObjManager_h__

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QTimer>

class QNetworkReply;
class CNetworkReplyTempObjManager : public QObject
{   
	Q_OBJECT
public: 
	CNetworkReplyTempObjManager(QObject *parent = 0);
	virtual ~CNetworkReplyTempObjManager();

public:
	//增加临时Reply, nTimeOut超时时间(毫秒)(-1为无限长)
	bool addTempReply(QNetworkReply* pReply, int nTimeOut = -1);

private: 
	QList<QNetworkReply*>						m_listReply;
};
#endif // NetworkReplyTempObjManager_h__
