/*********************************************************************************
  *
  *
  *FileName:           NetworkReplyTempObjManager.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:		QNetworkReply临时对象管理
**********************************************************************************/
#ifndef NetworkReplyTempObjManager_h__
#define NetworkReplyTempObjManager_h__

#include <QtCore/QObject>
#include <QtCore/QVariant>

class QNetworkReply;
class QTimer;
class CNetworkReplyTempObjManager : public QObject
{   
	Q_OBJECT
public:
	typedef QMap<QNetworkReply*, QTimer*>		MapReplyToTimer;

public: 
	CNetworkReplyTempObjManager(QObject *parent = 0);
	virtual ~CNetworkReplyTempObjManager();

public:
	//增加临时Reply, nTimeOut超时时间(毫秒)(-1为无限长), replyData:用于区分标示Reply对象
	bool addTempReply(QNetworkReply* pReply, int nTimeOut = -1, const QVariant& replyData = QVariant());
    
	//移除临时Reply
    void removeTempReply(QNetworkReply* pReply);
    
	//清除临时Reply
    void clearTempReply();

private:
	void releaseReplyObj(QNetworkReply *pReply);
	void releaseTimerObj(QTimer *pTimer);

private slots:
	void onTimeOut();

signals:
	void replyTimeOut(const QVariant& replyData);

private: 
    QMutex														m_mutexShareReplyList;
	MapReplyToTimer											m_mapReplyToTimer;
};
#endif // NetworkReplyTempObjManager_h__
