#ifndef SharePublicFuncDefine_h__
#define SharePublicFuncDefine_h__

#include "ShareMacroDefine.h"
#include <QtNetwork/QNetworkRequest>
#include <QtCore/QString>

namespace ShareLibrary
{
	//��ȡ״̬����
	//QString getStateDescription(ShareLibrary::EAuthorizeState eState);

	//��ȡ��������
	//QString getErrorCodeDescription(ShareLibrary::EShareResultCode eCode);

	//��ȡFlickrȨ������ ShareLibrary::EFlickrPrivacy
	QString getFlickrPrivacyDescription(int nPrivacy);

	//����FlickrȨ�޻��ʶֵ  ShareLibrary::EFlickrPrivacy
	void getFlagValueByPrivacy(int nPrivacy, int& isFamily, int& isFriend, int& isPublic);

	//���ɵ�ǰʱ���
	QString generateTimeStampString();
	uint generateTimeStamp();

	//����OAuth��ʹ�õ�Nonce
	QByteArray generateNonce();

	//HMAC-SHA1����
	QByteArray hmacSha1(const QByteArray& message, const QByteArray& key);

	//��ȡ��ҳ���������ַ���
	QString getHttpTypeString(ShareLibrary::HttpMethod eMethod);

	//������������
	QNetworkRequest createSSLRequest(const QString& strUrl);
	void createSSLRequest(QNetworkRequest& request, const QUrl& url);

	//��ȡҳ����ؽ���ַ���
	QString getPageLoadResultString(ShareLibrary::EPageLoadResult eResult);

	//����Facebook�����Ϣ
	bool readFacebookAlbumsInfoByByteArray(const QByteArray& json, ShareLibrary::MapAlbumInfo& mapAlbumInfo);

	//����Flickr�����Ϣ
	bool readFlickrAlbumsInfoByByteArray(const QByteArray& json, ShareLibrary::MapAlbumInfo& mapAlbumInfo);

	//�ƶ����嵽��Ļ�м�
	void moveWidgetToScreenCenter(QWidget* pWidget);
}
#endif // SharePublicFuncDefine_h__
