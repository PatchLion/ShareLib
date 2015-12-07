/*********************************************************************************
  * 
  *
  *FileName:           ShareFrameBase.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:			分享框架基类
**********************************************************************************/
#ifndef ShareFrameBase_h__
#define ShareFrameBase_h__

#include "ShareMacroDefine.h"
#include "ShareStructDefine.h"
#include "NetworkReplyTempObjManager.h"

#define  SHARE_IMAGE_TIMEOUT		(50 * 1000)  //分享超时

class QNetworkAccessManager;
class CShareFrameBase : public QObject
{   
	Q_OBJECT

public: 
	CShareFrameBase(QObject *parent=0);
	virtual ~CShareFrameBase();


public slots:
	//分享相片
	virtual bool share(IShareParam* pParam);

protected:
	CNetworkReplyTempObjManager& replyTempObjectManager() { return m_replyObjectManager; }
	QNetworkAccessManager* networkAccessManager() { return m_pAccessMgr; }

	private slots :
		//处理超时
		void onReplyTimeOut(const QVariant& replyData);

signals:
	//分享结果
	void shareFinished(bool bSuccess, const  QString& strDesc);



private:
	QNetworkAccessManager					*m_pAccessMgr;
	CNetworkReplyTempObjManager		m_replyObjectManager;		//
};
#endif // ShareFrameBase_h__
