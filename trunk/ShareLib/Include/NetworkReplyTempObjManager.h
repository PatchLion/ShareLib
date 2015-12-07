/*********************************************************************************
  *
  *
  *FileName:           NetworkReplyTempObjManager.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:		QNetworkReply��ʱ�������
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
	//������ʱReply, nTimeOut��ʱʱ��(����)(-1Ϊ���޳�), replyData:�������ֱ�ʾReply����
	bool addTempReply(QNetworkReply* pReply, int nTimeOut = -1, const QVariant& replyData = QVariant());
    
	//�Ƴ���ʱReply
    void removeTempReply(QNetworkReply* pReply);
    
	//�����ʱReply
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
