#ifndef SHARETEST_H
#define SHARETEST_H

#include <QtCore/QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

#include "ui_ShareTest.h"
#include "ShareStructDefine.h"
#include "FacebookShare.h"
#include "FlickrShare.h"
#include "TwitterShare.h"
class QThread;
class CAuthorizeWebview;
class ShareTest : public QWidget
{
	Q_OBJECT

public:
	ShareTest(QWidget *parent = 0);
	~ShareTest();

private slots:
	void on_pushButtonUploadImage_clicked();
	void on_pushButtonLogout_clicked();
	void on_pushButtonFBLogin_clicked();

	void on_pushButtonFlickrUploadImage_clicked();
	void on_pushButtonFlickrLogout_clicked();	
	void on_pushButtonFlickrLogin_clicked();
	
	void on_pushButtonTwitterUploadImage_clicked();
	void on_pushButtonTwitterLogout_clicked();
	void on_pushButtonTwitterLogin_clicked();

	void onFacebookTokenReceived();
	void onFacebookShareFinished(bool bSuccess, const  QString& strDesc);
	void onFacebookAlbumsInfoUpdate(bool bSuccess, const ShareLibrary::MapAlbumInfo& mapAlbumInfo);

	void onFlickrTokenReceived();
	void onFlickrShareFinished(bool bSuccess, const  QString& strDesc);
	void onFlickrAlbumsInfoUpdate(bool bSuccess, const ShareLibrary::MapAlbumInfo& mapAlbumInfo);

	void onTwitterTokenReceived();
	void onTwitterShareFinished(bool bSuccess, const  QString& strDesc);

private:
	void setFacebookTokenEnabled(bool bEnabled);
	void setFlickrTokenEnabled(bool bEnabled);
	void setTwitterTokenEnabled(bool bEnabled);

private:
	Ui::ShareTestClass ui;
	bool										m_bFBAlbumListRefreshed;
	bool										m_bFlickrAlbumListRefreshed;

	CFacebookShareParam	m_fbParam;
	CFlickrShareParam			m_flickrParam;
	CTwitterShareParam			m_twitterParam;

	CFacebookShare*				m_pFBShare;
	CFlickrShare*						m_pFlickrShare;
	CTwitterShare*					m_pTwitterShare;
};

#endif // SHARETEST_H
