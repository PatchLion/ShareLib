/*********************************************************************************
  *
  *
  *FileName:           FlickrShare.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-9-28
  *
  *Description:		Flickr分享
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
	//设置Token
	void setTokenKey(const QString& tokenKey);
	//Token Key
	QString tokenKey() const;

	//UserID
	void setUserID(const QString& strUserID);
	QString userID() const;

	//分享相片
	void share(IShareParam* pParam);

	//刷新相册
	void refreshAlbumList();
	
	//登出
	void signOut();

	//检测授权（如果没有授权，则显示授权网页）
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
	//获取到了相册信息
	void albumsInfoRefreshFinished(bool bSuccess, const ShareLibrary::MapAlbumInfo& mapAlbumInfo);

	//分享结果
	void shareFinished(bool bSuccess, const  QString& strDesc);

	//接收到Token
	void tokenReceived();

	//////////////////////////////////////////////////////////////////////////
	//内部信号（请勿连接下列信号）
	void shareSignal(IShareParam* pParam);
	void refreshAlbumListSignal();

private:
	const QString											m_strSecret;	
	const QString											m_strAPIKey;

	QThread													*m_pShareThread;							//分享线程

	CFlickrShareObject						*m_pShareObject;							//分享对象
	CFlickrAuthorizeWebview				*m_pAuthorizeWebview;				//授权网页窗体
};
#endif // FlickrShare_h__
