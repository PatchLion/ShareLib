/*********************************************************************************
  * 
  *
  *FileName:           TwitterShare.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-30
  *
  *Description:		Twitter分享
**********************************************************************************/
#ifndef TwitterShare_h__
#define TwitterShare_h__

#include <QtCore/QObject>
#include <QtCore/QSize>
#include "ShareMacroDefine.h"
#include "IWebviewStyle.h"

class CTwitterShareObject;
class CTwitterAuthorizeWebview;
class IShareParam;
class CTwitterShare : public QObject, public IWebviewStyle
{   
	Q_OBJECT

public: 
	CTwitterShare(const QString& strConsumerKey,			//在twitter上申请的用户Key
		const QString& strConsumerSecret,		//在twitter上申请的用户秘钥
		QObject *parent = 0);
	~CTwitterShare();

public:
	//
	void setOauthToken(const QString& val);
	QString oauthToken() const;

	//
	void setOauthTokenSecret(const QString& val);
	QString oauthTokenSecret() const;

	//分享相片
	void share(IShareParam* pParam);

	//登出
	void signOut();

	//检测授权（如果没有授权，则显示授权网页）
	void checkAuthorize();
	bool isAuthorized() const;

	//生成头信息
	static QByteArray generateAuthorizationHeader(const QUrl& url, ShareLibrary::HttpMethod method, const QString& strOauthToken, const QString& strOauthTokenSecret, const QString& strConsumerKey, const QString& strConsumerSecret);

	//使用HMACSHA1加密信息
	static QByteArray generateSignatureHMACSHA1(const QByteArray& signatureBase, const QString& strConsumerSecret, const QString& strOauthTokenSecret);

	//生成签名信息
	static QByteArray generateSignatureBase(const QUrl& url, ShareLibrary::HttpMethod method, const QByteArray& timestamp, const QByteArray& nonce, const QString& strConsumerKey, const QString& strOauthToken);

protected:
	//
	virtual CAuthorizeWebviewBase* webView();

private:
	void showAuthorizeWebView();
	void releaseWebView();
private slots:
	void onTokenReceived();
	void onWebViewClose();

signals:
	//分享结果
	void shareFinished(bool bSuccess, const  QString& strDesc);

	//接收到Token
	void tokenReceived();

	//////////////////////////////////////////////////////////////////////////
	//内部信号（请勿连接下列信号）
	void shareSignal(IShareParam* pParam);

private:
	const QString					m_strConsumerKey;										//
	const QString					m_strConsumerSecret;

	QThread													*m_pShareThread;							//分享线程

	CTwitterShareObject						*m_pShareObject;							//分享对象
	CTwitterAuthorizeWebview				*m_pAuthorizeWebview;				//授权网页窗体
};
#endif // TwitterShare_h__
