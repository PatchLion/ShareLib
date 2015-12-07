/*********************************************************************************
  * 
  *
  *FileName:           FacebookShare.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-30
  *
  *Description:		FaceBook����
**********************************************************************************/
#ifndef FacebookShare_h__
#define FacebookShare_h__

#include <QtCore/QObject>
#include <QtCore/QSize>
#include "ShareMacroDefine.h"
#include "IWebviewStyle.h"

class CFacebookShareObject;
class CFaceBookAuthorizeWebview;
class IShareParam;
class CFacebookShare : public QObject, public IWebviewStyle
{   
	Q_OBJECT

public: 
	CFacebookShare(const QString& clientID, QObject *parent = 0);
	~CFacebookShare();

public:
	//����Token
	void setTokenKey(const QString& tokenKey);

	//������Ƭ
	void share(IShareParam* pParam);

	//ˢ�����
	void refreshAlbumList();
	
	//�ǳ�
	void signOut();

	//�����Ȩ�����û����Ȩ������ʾ��Ȩ��ҳ��
	void checkAuthorize();
	bool isAuthorized() const;

	//Token Key
	QString tokenKey() const;

	//��Чʱ���
	uint expiresTimeStamp() const;

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
	const QString											m_strClientID;	

	QThread													*m_pShareThread;							//�����߳�

	CFacebookShareObject						*m_pShareObject;							//�������
	CFaceBookAuthorizeWebview				*m_pAuthorizeWebview;				//��Ȩ��ҳ����
};
#endif // FacebookShare_h__
