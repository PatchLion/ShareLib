/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
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
//#include "NetworkReplyTempObjManager.h"
#include <QtNetwork/QNetworkAccessManager>

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
	//CNetworkReplyTempObjManager& replyTempObjectManager() { return m_replyObjectManager; }
	QNetworkAccessManager& networkAccessManager() { return m_networkManager; }

	bool isUserCancel() const { return m_bIsUserCancel; }
signals:
	//������
	void shareFinished(bool bSuccess, const  QString& strDesc);

private:
	QNetworkAccessManager					m_networkManager;
	bool														m_bIsUserCancel;
	//CNetworkReplyTempObjManager		m_replyObjectManager;		//
};
#endif // ShareFrameBase_h__
