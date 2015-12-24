/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           FacebookShareObject.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-30
  *
  *Description:		FaceBook�������
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
	//����Token
	void setTokenKey(const QString& tokenKey);
	QString tokenKey() const { return m_strTokenKey; }

	//�ǳ�
	void signOut() { m_strTokenKey.clear(); }

public slots:
	//������Ƭ
	bool share(IShareParam* pParam);

	//ˢ�����
	void refreshAlbumList();

private:
	//����
	bool shareToFacebook(const QString& strDescriptionStr, const QString& strAlbumsStr, const QString& strFilePath);

signals:
	//��ȡ���������Ϣ
	void albumsInfoRefreshFinished(bool bSuccess, const ShareLibrary::MapAlbumInfo& mapAlbumInfo);

protected slots:
	//��������������
	void onReplayFinishedAlbum();

	//�ϴ������������
	void onReplayFinishedUpload();

private: 
	QString													m_strTokenKey;						//Token
};
#endif // FacebookShareObject_h__
