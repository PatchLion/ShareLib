/*********************************************************************************
  * 
  *
  *FileName:           TwitterShareObject.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-30
  *
  *Description:		Twitter�������
**********************************************************************************/
#ifndef TwitterShareObject_h__
#define TwitterShareObject_h__

#include "ShareFrameBase.h"

class QNetworkAccessManager;
class CTwitterShareObject : public CShareFrameBase
{   
	Q_OBJECT

public: 
	CTwitterShareObject(const QString& strConsumerKey,			//��twitter��������û�Key
		const QString& strConsumerSecret,		//��twitter��������û���Կ
		QObject *parent = 0);
	~CTwitterShareObject();

public:
	//
	void setOauthToken(const QString& val) { m_strOauthToken = val; }
	QString oauthToken() const { return m_strOauthToken; }

	//
	void setOauthTokenSecret(const QString& val) { m_strOauthTokenSecret = val; }
	QString oauthTokenSecret() const { return m_strOauthTokenSecret; }

	//�ǳ�
	void signOut() 
	{ 
		m_strOauthToken.clear(); 
		m_strOauthTokenSecret.clear();
	}

public slots:
	//������Ƭ
	bool share(IShareParam* pParam);

private:
	//����
	bool shareToTwitter(const QString& strDescriptionStr,  const QString& strFilePath);

protected slots:
	//�ϴ������������
	void onReplyFinishedUpload();


private: 
	const QString					m_strConsumerKey;										//
	const QString					m_strConsumerSecret;

	QString								m_strOauthTokenSecret;
	QString								m_strOauthToken;						//
};
#endif // TwitterShareObject_h__
