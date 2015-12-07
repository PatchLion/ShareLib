/*********************************************************************************
  * 
  *
  *FileName:           ShareFrameBase.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:			�����ܻ���
**********************************************************************************/
#ifndef ShareFrameBase_h__
#define ShareFrameBase_h__

#include "ShareMacroDefine.h"
#include "ShareStructDefine.h"
#include "NetworkReplyTempObjManager.h"

#define  SHARE_IMAGE_TIMEOUT		(50 * 1000)  //����ʱ

class QNetworkAccessManager;
class CShareFrameBase : public QObject
{   
	Q_OBJECT

public: 
	CShareFrameBase(QObject *parent=0);
	virtual ~CShareFrameBase();


public slots:
	//������Ƭ
	virtual bool share(IShareParam* pParam);

protected:
	CNetworkReplyTempObjManager& replyTempObjectManager() { return m_replyObjectManager; }
	QNetworkAccessManager* networkAccessManager() { return m_pAccessMgr; }

	private slots :
		//����ʱ
		void onReplyTimeOut(const QVariant& replyData);

signals:
	//������
	void shareFinished(bool bSuccess, const  QString& strDesc);



private:
	QNetworkAccessManager					*m_pAccessMgr;
	CNetworkReplyTempObjManager		m_replyObjectManager;		//
};
#endif // ShareFrameBase_h__
