/*********************************************************************************
  * 
  *
  *FileName:           TwitterShareObject.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-30
  *
  *Description:		Twitter分享对象
**********************************************************************************/
#ifndef TwitterShareObject_h__
#define TwitterShareObject_h__

#include "ShareFrameBase.h"

class QNetworkAccessManager;
class CTwitterShareObject : public CShareFrameBase
{   
	Q_OBJECT

public: 
	CTwitterShareObject(const QString& strConsumerKey,			//在twitter上申请的用户Key
		const QString& strConsumerSecret,		//在twitter上申请的用户秘钥
		QObject *parent = 0);
	~CTwitterShareObject();

public:
	//
	void setOauthToken(const QString& val) { m_strOauthToken = val; }
	QString oauthToken() const { return m_strOauthToken; }

	//
	void setOauthTokenSecret(const QString& val) { m_strOauthTokenSecret = val; }
	QString oauthTokenSecret() const { return m_strOauthTokenSecret; }

	//登出
	void signOut() 
	{ 
		m_strOauthToken.clear(); 
		m_strOauthTokenSecret.clear();
	}

public slots:
	//分享相片
	bool share(IShareParam* pParam);

private:
	//分享
	bool shareToTwitter(const QString& strDescriptionStr,  const QString& strFilePath);

protected slots:
	//上传请求结束处理
	void onReplyFinishedUpload();


private: 
	const QString					m_strConsumerKey;										//
	const QString					m_strConsumerSecret;

	QString								m_strOauthTokenSecret;
	QString								m_strOauthToken;						//
};
#endif // TwitterShareObject_h__
