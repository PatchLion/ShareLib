/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           FacebookShareObject.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-30
  *
  *Description:		FaceBook分享对象
**********************************************************************************/
#ifndef FacebookShareObject_h__
#define FacebookShareObject_h__

#include "ShareFrameBase.h"

class QNetworkAccessManager;
class CFacebookShareObject : public CShareFrameBase
{   
	Q_OBJECT

public: 
	CFacebookShareObject(QObject *parent=0);
	~CFacebookShareObject();

public:
	//设置Token
	void setTokenKey(const QString& tokenKey);
	QString tokenKey() const { return m_strTokenKey; }

	//登出
	void signOut() { m_strTokenKey.clear(); }

public slots:
	//分享相片
	bool share(IShareParam* pParam);

	//刷新相册
	void refreshAlbumList();

private:
	//分享
	bool shareToFacebook(const QString& strDescriptionStr, const QString& strAlbumsStr, const QString& strFilePath);

signals:
	//获取到了相册信息
	void albumsInfoRefreshFinished(bool bSuccess, const ShareLibrary::MapAlbumInfo& mapAlbumInfo);

protected slots:
	//相册请求结束处理
	void onReplayFinishedAlbum();

	//上传请求结束处理
	void onReplayFinishedUpload();

private: 
	QString													m_strTokenKey;						//Token
};
#endif // FacebookShareObject_h__
