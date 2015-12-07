/*********************************************************************************
  *
  *
  *FileName:           FlickrShare.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-9-28
  *
  *Description:		Flickr����
**********************************************************************************/
#ifndef FlickrShare_h__
#define FlickrShare_h__

#include <QtCore/QObject>
#include <QtCore/QSize>
#include "ShareMacroDefine.h"
#include "IWebviewStyle.h"

class CFlickrShareObject;
class CFlickrAuthorizeWebview;
class IShareParam;
class CFlickrShare : public QObject, public IWebviewStyle
{   
	Q_OBJECT

public: 
	CFlickrShare(const QString& secret, const QString& apiKey, QObject *parent = 0);
	~CFlickrShare();

public:
	//����Token
	void setTokenKey(const QString& tokenKey);
	//Token Key
	QString tokenKey() const;

	//UserID
	void setUserID(const QString& strUserID);
	QString userID() const;

	//������Ƭ
	void share(IShareParam* pParam);

	//ˢ�����
	void refreshAlbumList();
	
	//�ǳ�
	void signOut();

	//�����Ȩ�����û����Ȩ������ʾ��Ȩ��ҳ��
	void checkAuthorize();
	bool isAuthorized() const;



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
	//��ȡ���������Ϣ
	void albumsInfoRefreshFinished(bool bSuccess, const ShareLibrary::MapAlbumInfo& mapAlbumInfo);

	//������
	void shareFinished(bool bSuccess, const  QString& strDesc);

	//���յ�Token
	void tokenReceived();

	//////////////////////////////////////////////////////////////////////////
	//�ڲ��źţ��������������źţ�
	void shareSignal(IShareParam* pParam);
	void refreshAlbumListSignal();

private:
	const QString											m_strSecret;	
	const QString											m_strAPIKey;

	QThread													*m_pShareThread;							//�����߳�

	CFlickrShareObject						*m_pShareObject;							//�������
	CFlickrAuthorizeWebview				*m_pAuthorizeWebview;				//��Ȩ��ҳ����
};
#endif // FlickrShare_h__
