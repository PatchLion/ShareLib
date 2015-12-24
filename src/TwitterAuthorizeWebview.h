/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           TwitterAuthorizeWebview.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:		Twitter授权网页
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
	CTwitterAuthorizeWebview(const QString& strConsumerKey,			//在twitter上申请的用户Key
													const QString& strConsumerSecret,		//在twitter上申请的用户秘钥, 
													QWidget *parent = 0);
	~CTwitterAuthorizeWebview();

public:
	//清空Token
	virtual void clearToken();

	//
	QString oauthToken() const { return m_strOauthToken; }
	
	//
	QString oauthTokenSecret() const { return m_strOauthTokenSecret; }

	//重置
	void reset();

protected:
	//开始请求授权
	virtual void startToAuthorize();

	 //授权网页
	QString authorizationUrl() const;

	//开始请求OAToken
	void startRequestOauthToken();

private:
	//读取Token信息
	void parseTokens(const QByteArray& response);

	//开始获取Access token
	void startRequestAccessToken(const QString& strPin);

protected slots :
	//页面加载完毕(根据eResult判断成功或失败)
	virtual void onPageLoadFinished(ShareLibrary::EPageLoadResult eResult);

	//页面加载进度
	virtual void onPageLoadProcess(int process);

	//获取OauthToken结果处理
	void onReplayFinishedOauthToken();


	//获取AccessToken结果处理
	void onReplayFinishedAccessToken();

signals:
	//接收到Token
	void tokenReceived();

	//用户取消
	void userCancel();


private:
	bool									m_bStartToRequestAccessToken;				//
	const QString					m_strConsumerKey;										//
	const QString					m_strConsumerSecret;

	QString								m_strOauthTokenSecret;
	QString								m_strOauthToken;						//

};
#endif // TwitterAuthorizeWebview_h__
