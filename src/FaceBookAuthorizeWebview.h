/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           FaceBookAuthorizeWebview.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:		FaceBook授权网页
**********************************************************************************/
#ifndef FaceBookAuthorizeWebview_h__
#define FaceBookAuthorizeWebview_h__

#include "ShareMacroDefine.h"
#include "AuthorizeWebviewBase.h"
#include <QtCore/QTimer>
class CWebviewContainer;
class CFaceBookAuthorizeWebview : public CAuthorizeWebviewBase
{   
	Q_OBJECT

public: 
	CFaceBookAuthorizeWebview(const QString& clientID, QWidget *parent = 0);
	~CFaceBookAuthorizeWebview();

public:
	//清空Token
	virtual void clearToken();

	//Token Key
	QString tokenKey() const { return m_strTokenKey; }

	//有效时间戳
	uint expiresTimeStamp() const { return m_nExpiresTimeStamp; }

	//ClientID
	QString clientID() const { return m_strClientID; }

	//重置
	void reset();
protected:
	//开始请求授权
	virtual void startToAuthorize();

	 //授权网页
	QString authorizationUrl() const;

private:


protected slots :
	//页面加载完毕(根据eResult判断成功或失败)
	virtual void onPageLoadFinished(ShareLibrary::EPageLoadResult eResult);

	//页面加载进度
	virtual void onPageLoadProcess(int process);

    //
    void onLoadUrlTimeout();
signals:
	//接收到Token
	void tokenReceived();

	//用户取消
	void userCancel();

private:
	const QString				m_strClientID;						//客户端ID

    QTimer                              m_loadUrlTimer;
	QString						m_strCancelUrl;						//取消URL
	QString						m_strTokenKey;						//获取到的TokenKey
	uint								m_nExpiresTimeStamp;			//Token有效时间戳
};
#endif // FaceBookAuthorizeWebview_h__
