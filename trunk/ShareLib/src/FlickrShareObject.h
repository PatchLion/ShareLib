/*********************************************************************************
  * 
  *
  *FileName:           FlickrShareObject.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-30
  *
  *Description:		Flickr分享对象
**********************************************************************************/
#ifndef FlickrShareObject_h__
#define FlickrShareObject_h__

#include "ShareFrameBase.h"


class CFlickrShareObject : public CShareFrameBase
{   
	Q_OBJECT

public: 
	CFlickrShareObject(const QString& secret, const QString& apiKey, QObject *parent=0);
	~CFlickrShareObject();

public:
	//设置Token
	void setTokenKey(const QString& tokenKey);
	QString tokenKey() const { return m_strTokenKey; }

	//UserID
	void setUserID(const QString& strUserID) { m_strUserID = strUserID; }
	QString userID() const { return m_strUserID; }

	//登出
	void signOut() { m_strTokenKey.clear(); }

public slots:
	//分享相片
	bool share(IShareParam* pParam);

	//刷新相册
	void refreshAlbumList();

private:
	//分享
	bool shareToFlickr(const QString& strTitle, const QString& strDescriptionStr, const QString& strAlbumsStr, const QString& strFilePath, int nPrivacy);

	//开始上传图片到相册
	void startUploadPhotoToAlbum();


signals:
	//获取到了相册信息
	void albumsInfoRefreshFinished(bool bSuccess, const ShareLibrary::MapAlbumInfo& mapAlbumInfo);

protected slots:
	//相册请求结束处理
	void onReplayFinishedAlbum();

	//上传请求结束处理
	void onReplayFinishedUpload();

	//上传图片到相册结束处理
	void onReplayFinishedTransform();

private: 
	const QString											m_strSecret;							
	const QString											m_strAPIKey;

	QString													m_strTempAlbumID;				//相册ID
	QString													m_strCurrentPhotoID;			
	QString													m_strTokenKey;						//Token
	QString													m_strUserID;							//User ID
};
#endif // FlickrShareObject_h__
