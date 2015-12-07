/*********************************************************************************
  * 
  *
  *FileName:           FacebookShare.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-30
  *
  *Description:		FaceBook分享
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
	//设置Token
	void setTokenKey(const QString& tokenKey);

	//分享相片
	void share(IShareParam* pParam);

	//刷新相册
	void refreshAlbumList();
	
	//登出
	void signOut();

	//检测授权（如果没有授权，则显示授权网页）
	void checkAuthorize();
	bool isAuthorized() const;

	//Token Key
	QString tokenKey() const;

	//有效时间戳
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
	const QString											m_strClientID;	

	QThread													*m_pShareThread;							//分享线程

	CFacebookShareObject						*m_pShareObject;							//分享对象
	CFaceBookAuthorizeWebview				*m_pAuthorizeWebview;				//授权网页窗体
};
#endif // FacebookShare_h__
