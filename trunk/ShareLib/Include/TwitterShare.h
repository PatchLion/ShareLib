/*********************************************************************************
  * 
  *
  *FileName:           TwitterShare.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-30
  *
  *Description:		Twitter����
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
	CTwitterShare(const QString& strConsumerKey,			//��twitter��������û�Key
		const QString& strConsumerSecret,		//��twitter��������û���Կ
		QObject *parent = 0);
	~CTwitterShare();

public:
	//
	void setOauthToken(const QString& val);
	QString oauthToken() const;

	//
	void setOauthTokenSecret(const QString& val);
	QString oauthTokenSecret() const;

	//������Ƭ
	void share(IShareParam* pParam);

	//�ǳ�
	void signOut();

	//�����Ȩ�����û����Ȩ������ʾ��Ȩ��ҳ��
	void checkAuthorize();
	bool isAuthorized() const;

	//����ͷ��Ϣ
	static QByteArray generateAuthorizationHeader(const QUrl& url, ShareLibrary::HttpMethod method, const QString& strOauthToken, const QString& strOauthTokenSecret, const QString& strConsumerKey, const QString& strConsumerSecret);

	//ʹ��HMACSHA1������Ϣ
	static QByteArray generateSignatureHMACSHA1(const QByteArray& signatureBase, const QString& strConsumerSecret, const QString& strOauthTokenSecret);

	//����ǩ����Ϣ
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
	//������
	void shareFinished(bool bSuccess, const  QString& strDesc);

	//���յ�Token
	void tokenReceived();

	//////////////////////////////////////////////////////////////////////////
	//�ڲ��źţ��������������źţ�
	void shareSignal(IShareParam* pParam);

private:
	const QString					m_strConsumerKey;										//
	const QString					m_strConsumerSecret;

	QThread													*m_pShareThread;							//�����߳�

	CTwitterShareObject						*m_pShareObject;							//�������
	CTwitterAuthorizeWebview				*m_pAuthorizeWebview;				//��Ȩ��ҳ����
};
#endif // TwitterShare_h__
