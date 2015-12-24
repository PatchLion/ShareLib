#include "ShareTest.h"
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#endif
#include <QtCore/QThread>
#include "SharePublicFuncDefine.h"
#include "ShareStructDefine.h"
#include <QtWidgets/QMessageBox>

#define WAITING_IMAGE	":/Resources/images/waiting_bg.png"
#define FAILED_IMAGE	":/Resources/images/waiting_failed.png"

ShareTest::ShareTest(QWidget *parent)
	: QWidget(parent)
	, m_pFBShare(0)
	, m_bFBAlbumListRefreshed(false)
	, m_bFlickrAlbumListRefreshed(false)
	, m_pFlickrShare(0)
	, m_pTwitterShare(0)
{ 
	ui.setupUi(this);

	m_pFBShare = new CFacebookShare("475058642544447",  this);
	connect(m_pFBShare, &CFacebookShare::tokenReceived, this, &ShareTest::onFacebookTokenReceived);
	connect(m_pFBShare, &CFacebookShare::shareFinished, this, &ShareTest::onFacebookShareFinished);
	connect(m_pFBShare, &CFacebookShare::albumsInfoRefreshFinished, this, &ShareTest::onFacebookAlbumsInfoUpdate);
	m_pFBShare->setWebViewTitle("Fotor");


	m_pFlickrShare = new CFlickrShare("07bd5ddec780d678",  "e3be23e18e9eab250b0cc573291da226",  this);
	connect(m_pFlickrShare, &CFlickrShare::tokenReceived, this, &ShareTest::onFlickrTokenReceived);
	connect(m_pFlickrShare, &CFlickrShare::shareFinished, this, &ShareTest::onFlickrShareFinished);
	connect(m_pFlickrShare, &CFlickrShare::albumsInfoRefreshFinished, this, &ShareTest::onFlickrAlbumsInfoUpdate);
	m_pFlickrShare->setWebViewTitle("Fotor");


	m_pTwitterShare = new CTwitterShare("L6Z6Aa7zcfhj7e97Fsnkg", "KSkz0ibTwgCdFM5rUCSsZynqyi1gfoVnljE7v0qxcM", this);
	connect(m_pTwitterShare, &CTwitterShare::tokenReceived, this, &ShareTest::onTwitterTokenReceived);
	connect(m_pTwitterShare, &CTwitterShare::shareFinished, this, &ShareTest::onTwitterShareFinished);
	m_pTwitterShare->setWebViewTitle("Fotor");


	QList<ShareLibrary::EFlickrPrivacy> listPrivacy;
	listPrivacy << ShareLibrary::Public;
	listPrivacy << ShareLibrary::Friend;
	listPrivacy << ShareLibrary::Family;
	listPrivacy << ShareLibrary::Friend_and_Family;
	listPrivacy << ShareLibrary::Privacy;

	for (int nIndex = 0; nIndex < listPrivacy.size(); nIndex++)
	{
		ui.comboBoxFlickrPracy->addItem(ShareLibrary::getFlickrPrivacyDescription(listPrivacy[nIndex]), (int)listPrivacy[nIndex]);
	}

	ui.comboBoxFlickrPracy->setCurrentIndex(0);

	setFacebookTokenEnabled(false);
	setFlickrTokenEnabled(false);
	setTwitterTokenEnabled(false);
}

ShareTest::~ShareTest()
{

}


void ShareTest::onFlickrAlbumsInfoUpdate(bool bSuccess, const ShareLibrary::MapAlbumInfo& mapAlbumInfo)
{
	if (!bSuccess)
	{
		QMessageBox::information(this, "Failed!", "Get Flickr album information failed!");

		return;
	}

	ui.comboBoxFlickrAlbums->clear();

	QMap<QString, QString>::const_iterator itemItor = mapAlbumInfo.begin();
	for (itemItor; itemItor != mapAlbumInfo.end(); itemItor++)
	{
		ui.comboBoxFlickrAlbums->addItem(itemItor.value(), itemItor.key());
	}

	m_bFlickrAlbumListRefreshed = true;
}

void ShareTest::onFacebookAlbumsInfoUpdate(bool bSuccess, const ShareLibrary::MapAlbumInfo& mapAlbumInfo)
{
	if (!bSuccess)
	{
		QMessageBox::information(this, "Failed!", "Get FaceBook album information failed!");

		return;
	}

	ui.comboBoxAlbums->clear();

	QMap<QString, QString>::const_iterator itemItor = mapAlbumInfo.begin();
	for (itemItor; itemItor != mapAlbumInfo.end(); itemItor++)
	{
		ui.comboBoxAlbums->addItem(itemItor.value(), itemItor.key());
	}

	m_bFBAlbumListRefreshed = true;
}
/*

void ShareTest::onFacebookStateChanged( ShareLibrary::EAuthorizeState eState )
{	
	switch(eState)
	{
	case ShareLibrary::State_Authorised:							
		{			

			m_pFBShare->refreshAlbumList();
					
			ui.pushButtonUploadImage->setEnabled(true);
			ui.pushButtonLogout->setEnabled(true);
			ui.pushButtonFBLogin->setEnabled(false);
		}
		break;

	default:
		{
			ui.lineEditDesc->clear();
			ui.comboBoxAlbums->clear();
			ui.pushButtonUploadImage->setEnabled(false);
			ui.pushButtonLogout->setEnabled(false);
			ui.pushButtonFBLogin->setEnabled(true);
		}
		break;
	}

	ui.labelFBState->setText(ShareLibrary::getStateDescription(eState));
}




void ShareTest::onFacebookShareFinished( bool bSuccess, ShareLibrary::EReturnCode eCode )
{
	QString strMessage = bSuccess ? tr("Share successed!") : tr("Share failed : %1").arg(ShareLibrary::getErrorCodeDescription(eCode));

	QMessageBox::information(this, tr("share result"), strMessage);
}

void ShareTest::onFlickrStateChanged( ShareLibrary::EAuthorizeState eState )
{
	switch(eState)
	{
	case ShareLibrary::State_Authorised:							
		{			
		
			m_pFlickrShare->refreshAlbumList();
			

			ui.pushButtonFlickrUploadImage->setEnabled(true);
			ui.pushButtonFlickrLogout->setEnabled(true);
			ui.pushButtonFlickrLogin->setEnabled(false);
		}
		break;

	default:
		{
			ui.lineEditFlickrDesc->clear();
			ui.comboBoxFlickrAlbums->clear();
			ui.pushButtonFlickrUploadImage->setEnabled(false);
			ui.pushButtonFlickrLogout->setEnabled(false);
			ui.pushButtonFlickrLogin->setEnabled(true);
		}
		break;
	}

	ui.labelFlickrState->setText(ShareLibrary::getStateDescription(eState));
}

void ShareTest::onFlickrShareFinished( bool bSuccess, ShareLibrary::EReturnCode eCode )
{
	QString strMessage = bSuccess ? tr("Share successed!") : tr("Share failed : %1").arg(ShareLibrary::getErrorCodeDescription(eCode));

	QMessageBox::information(this, tr("share result"), strMessage);
}

void ShareTest::onFlickrError( ShareLibrary::EReturnCode eCode )
{
	qDebug() << "Flickr error: " << ShareLibrary::getErrorCodeDescription(eCode);
}
*/


void ShareTest::on_pushButtonUploadImage_clicked()
{
	const QString strFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.xpm *.jpg)"));

	if (!strFilePath.isEmpty() && m_pFBShare)
	{
		m_fbParam.description = ui.lineEditDesc->text();
		m_fbParam.albumID = ui.comboBoxFlickrAlbums->itemData(ui.comboBoxAlbums->currentIndex()).toString();
		m_fbParam.filePath = strFilePath;
		m_pFBShare->share(&m_fbParam);
		setEnabled(false);
	}
}


void ShareTest::on_pushButtonFlickrUploadImage_clicked()
{
	const QString strFilePath =  QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.xpm *.jpg)"));

	if (!strFilePath.isEmpty() && m_pFlickrShare)
	{
		m_flickrParam.description = ui.lineEditFlickrDesc->text();
		m_flickrParam.albumID = ui.comboBoxFlickrAlbums->itemData(ui.comboBoxFlickrAlbums->currentIndex()).toString();
		m_flickrParam.filePath = strFilePath;
		m_flickrParam.privacyType = ui.comboBoxFlickrPracy->itemData(ui.comboBoxFlickrPracy->currentIndex()).toInt();
		m_pFlickrShare->share(&m_flickrParam);

		setEnabled(false);
	}
}

void ShareTest::on_pushButtonFlickrLogout_clicked()
{
	if (m_pFlickrShare)
	{
		m_pFlickrShare->signOut();
	}

	setFlickrTokenEnabled(false);
}

void ShareTest::on_pushButtonTwitterUploadImage_clicked()
{
	const QString strFilePath =  QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.xpm *.jpg)"));

	if (!strFilePath.isEmpty() && m_pTwitterShare)
	{
		m_twitterParam.description = ui.lineEditTwitterDesc->text();
		m_twitterParam.filePath = strFilePath;
		m_pTwitterShare->share(&m_twitterParam);

		setEnabled(false);
	}
}

void ShareTest::on_pushButtonTwitterLogout_clicked()
{
	if (m_pTwitterShare)
	{
		m_pTwitterShare->signOut();
	}

	setTwitterTokenEnabled(false);
}
/*

void ShareTest::onTwitterStateChanged( ShareLibrary::EAuthorizeState eState )
{
	
	switch(eState)
	{
	case ShareLibrary::State_Authorised:							
		{			
			ui.pushButtonTwitterUploadImage->setEnabled(true);
			ui.pushButtonTwitterLogout->setEnabled(true);
			ui.pushButtonTwitterLogin->setEnabled(false);
		}
		break;

	default:
		{
			ui.lineEditTwitterDesc->clear();
			ui.pushButtonTwitterUploadImage->setEnabled(false);
			ui.pushButtonTwitterLogout->setEnabled(false);
			ui.pushButtonTwitterLogin->setEnabled(true);
		}
		break;
	}

	ui.labelTwitterState->setText(ShareLibrary::getStateDescription(eState));
}

void ShareTest::onTwitterShareFinished( bool bSuccess, ShareLibrary::EReturnCode eCode )
{
	QString strMessage = bSuccess ? tr("Share successed!") : tr("Share failed : %1").arg(ShareLibrary::getErrorCodeDescription(eCode));

	QMessageBox::information(this, tr("share result"), strMessage);
}

void ShareTest::onTwitterError( ShareLibrary::EReturnCode eCode )
{
	qDebug() << "Twitter error: " << ShareLibrary::getErrorCodeDescription(eCode);
}*/

void ShareTest::on_pushButtonFBLogin_clicked()
{
	if (m_pFBShare->isAuthorized())
	{
		m_pFBShare->refreshAlbumList();

		setFacebookTokenEnabled(true);
	}
	else
	{
		m_pFBShare->checkAuthorize();
	}
}

void ShareTest::on_pushButtonFlickrLogin_clicked()
{
	if (m_pFlickrShare->isAuthorized())
	{
		m_pFlickrShare->refreshAlbumList();

		setFlickrTokenEnabled(true);
	}
	else
	{
		m_pFlickrShare->checkAuthorize();
	}
}

void ShareTest::on_pushButtonTwitterLogin_clicked()
{
	if (m_pTwitterShare->isAuthorized())
	{
		setTwitterTokenEnabled(true);
	}
	else
	{
		m_pTwitterShare->checkAuthorize();
	}
}

void ShareTest::onFacebookTokenReceived()
{
	setFacebookTokenEnabled(true);
	m_pFBShare->refreshAlbumList();
}



void ShareTest::on_pushButtonLogout_clicked()
{
	if (m_pFBShare)
	{
		m_pFBShare->signOut();
	}

	setFacebookTokenEnabled(false);

	ui.comboBoxAlbums->clear();
}

void ShareTest::onFacebookShareFinished(bool bSuccess, const QString& strDesc)
{
	setEnabled(true);

	QMessageBox::information(this, "Share finished!", bSuccess ? "Share on FaceBook success" : "Share on FaceBook failed");
}
void ShareTest::setFacebookTokenEnabled(bool bEnabled)
{
	ui.pushButtonFBLogin->setEnabled(!bEnabled);
	ui.pushButtonUploadImage->setEnabled(bEnabled);
	ui.pushButtonLogout->setEnabled(bEnabled);
}

void ShareTest::setFlickrTokenEnabled(bool bEnabled)
{
	ui.pushButtonFlickrLogin->setEnabled(!bEnabled);
	ui.pushButtonFlickrUploadImage->setEnabled(bEnabled);
	ui.pushButtonFlickrLogout->setEnabled(bEnabled);
}

void ShareTest::setTwitterTokenEnabled(bool bEnabled)
{
	ui.pushButtonTwitterLogin->setEnabled(!bEnabled);
	ui.pushButtonTwitterUploadImage->setEnabled(bEnabled);
	ui.pushButtonTwitterLogout->setEnabled(bEnabled);
}

void ShareTest::onFlickrTokenReceived()
{
	setFlickrTokenEnabled(true);
	m_pFlickrShare->refreshAlbumList();
}

void ShareTest::onFlickrShareFinished(bool bSuccess, const QString& strDesc)
{
	setEnabled(true);

	QMessageBox::information(this, "Share finished!", bSuccess ? "Share on Flickr success" : "Share on Flickr failed");
}

void ShareTest::onTwitterTokenReceived()
{
	setTwitterTokenEnabled(true);
}

void ShareTest::onTwitterShareFinished(bool bSuccess, const QString& strDesc)
{
	setEnabled(true);

	QMessageBox::information(this, "Share finished!", bSuccess ? "Share on Twitter success" : "Share on Twitter failed");
}


