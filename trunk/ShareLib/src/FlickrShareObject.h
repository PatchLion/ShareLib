/*********************************************************************************
  * 
  *
  *FileName:           FlickrShareObject.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-30
  *
  *Description:		Flickr�������
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
	//����Token
	void setTokenKey(const QString& tokenKey);
	QString tokenKey() const { return m_strTokenKey; }

	//UserID
	void setUserID(const QString& strUserID) { m_strUserID = strUserID; }
	QString userID() const { return m_strUserID; }

	//�ǳ�
	void signOut() { m_strTokenKey.clear(); }

public slots:
	//������Ƭ
	bool share(IShareParam* pParam);

	//ˢ�����
	void refreshAlbumList();

private:
	//����
	bool shareToFlickr(const QString& strTitle, const QString& strDescriptionStr, const QString& strAlbumsStr, const QString& strFilePath, int nPrivacy);

	//��ʼ�ϴ�ͼƬ�����
	void startUploadPhotoToAlbum();


signals:
	//��ȡ���������Ϣ
	void albumsInfoRefreshFinished(bool bSuccess, const ShareLibrary::MapAlbumInfo& mapAlbumInfo);

protected slots:
	//��������������
	void onReplayFinishedAlbum();

	//�ϴ������������
	void onReplayFinishedUpload();

	//�ϴ�ͼƬ������������
	void onReplayFinishedTransform();

private: 
	const QString											m_strSecret;							
	const QString											m_strAPIKey;

	QString													m_strTempAlbumID;				//���ID
	QString													m_strCurrentPhotoID;			
	QString													m_strTokenKey;						//Token
	QString													m_strUserID;							//User ID
};
#endif // FlickrShareObject_h__
