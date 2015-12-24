/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           FlickrAuthorizeWebview.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:		Flickr��Ȩ��ҳ
**********************************************************************************/
#ifndef FlickrAuthorizeWebview_h__
#define FlickrAuthorizeWebview_h__

#include "ShareMacroDefine.h"
#include "AuthorizeWebviewBase.h"

class CWebviewContainer;
class CFlickrAuthorizeWebview : public CAuthorizeWebviewBase
{   
	Q_OBJECT

public: 
	CFlickrAuthorizeWebview(const QString& secret, const QString& apiKey, QWidget *parent = 0);
	~CFlickrAuthorizeWebview();

public:
	//���Token
	virtual void clearToken();

	//Token Key
	QString tokenKey() const { return m_strTokenKey; }

	//
	QString userID() const { return m_strUserID; }
	void setUserID(const QString& val) { m_strUserID = val; }

	//����
	void reset();

protected:
	//��ʼ������Ȩ
	virtual void startToAuthorize();

	 //��Ȩ��ҳ
	QString authorizationUrl() const;

	//��ȡ�û���Ȩ(Ĭ��������ҳ)
	void startGetUserAuth();

private:
	//��ʼ��ȡFrob
	void startToGetFrob();

	//��ʼ����Forb��ȡToken
	void startGetTokenByForb();

protected slots :
	//ҳ��������(����eResult�жϳɹ���ʧ��)
	virtual void onPageLoadFinished(ShareLibrary::EPageLoadResult eResult);

	//ҳ����ؽ���
	virtual void onPageLoadProcess(int process);

	//Frob�����������
	void onReplayFinishedGetFrob();

	//��ȡToken���󷵻�
	void onReplayFinishedGetToken();

signals:
	//���յ�Token
	void tokenReceived();

	//�û�ȡ��
	void userCancel();

private:
	const QString					m_strSecret;
	const QString					m_strAPIKey;

	bool								m_bIsGetTokenByForbStarted;
	QString						m_strForb;
	QString						m_strTokenKey;						//��ȡ����TokenKey
	QString						m_strUserID;							//User ID

};
#endif // FlickrAuthorizeWebview_h__
