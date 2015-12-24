/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           TwitterAuthorizeWebview.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:		Twitter��Ȩ��ҳ
**********************************************************************************/
#ifndef TwitterAuthorizeWebview_h__
#define TwitterAuthorizeWebview_h__

#include "ShareMacroDefine.h"
#include "AuthorizeWebviewBase.h"

class CWebviewContainer;
class CTwitterAuthorizeWebview : public CAuthorizeWebviewBase
{   
	Q_OBJECT

public: 
	CTwitterAuthorizeWebview(const QString& strConsumerKey,			//��twitter��������û�Key
													const QString& strConsumerSecret,		//��twitter��������û���Կ, 
													QWidget *parent = 0);
	~CTwitterAuthorizeWebview();

public:
	//���Token
	virtual void clearToken();

	//
	QString oauthToken() const { return m_strOauthToken; }
	
	//
	QString oauthTokenSecret() const { return m_strOauthTokenSecret; }

	//����
	void reset();

protected:
	//��ʼ������Ȩ
	virtual void startToAuthorize();

	 //��Ȩ��ҳ
	QString authorizationUrl() const;

	//��ʼ����OAToken
	void startRequestOauthToken();

private:
	//��ȡToken��Ϣ
	void parseTokens(const QByteArray& response);

	//��ʼ��ȡAccess token
	void startRequestAccessToken(const QString& strPin);

protected slots :
	//ҳ��������(����eResult�жϳɹ���ʧ��)
	virtual void onPageLoadFinished(ShareLibrary::EPageLoadResult eResult);

	//ҳ����ؽ���
	virtual void onPageLoadProcess(int process);

	//��ȡOauthToken�������
	void onReplayFinishedOauthToken();


	//��ȡAccessToken�������
	void onReplayFinishedAccessToken();

signals:
	//���յ�Token
	void tokenReceived();

	//�û�ȡ��
	void userCancel();


private:
	bool									m_bStartToRequestAccessToken;				//
	const QString					m_strConsumerKey;										//
	const QString					m_strConsumerSecret;

	QString								m_strOauthTokenSecret;
	QString								m_strOauthToken;						//

};
#endif // TwitterAuthorizeWebview_h__
