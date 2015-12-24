/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           FlickrAuthorizeWebview.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:		Flickr授权网页
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
	//清空Token
	virtual void clearToken();

	//Token Key
	QString tokenKey() const { return m_strTokenKey; }

	//
	QString userID() const { return m_strUserID; }
	void setUserID(const QString& val) { m_strUserID = val; }

	//重置
	void reset();

protected:
	//开始请求授权
	virtual void startToAuthorize();

	 //授权网页
	QString authorizationUrl() const;

	//获取用户授权(默认启动网页)
	void startGetUserAuth();

private:
	//开始获取Frob
	void startToGetFrob();

	//开始根据Forb获取Token
	void startGetTokenByForb();

protected slots :
	//页面加载完毕(根据eResult判断成功或失败)
	virtual void onPageLoadFinished(ShareLibrary::EPageLoadResult eResult);

	//页面加载进度
	virtual void onPageLoadProcess(int process);

	//Frob请求结束处理
	void onReplayFinishedGetFrob();

	//获取Token请求返回
	void onReplayFinishedGetToken();

signals:
	//接收到Token
	void tokenReceived();

	//用户取消
	void userCancel();

private:
	const QString					m_strSecret;
	const QString					m_strAPIKey;

	bool								m_bIsGetTokenByForbStarted;
	QString						m_strForb;
	QString						m_strTokenKey;						//获取到的TokenKey
	QString						m_strUserID;							//User ID

};
#endif // FlickrAuthorizeWebview_h__
