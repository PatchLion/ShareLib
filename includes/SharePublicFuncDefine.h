#ifndef SharePublicFuncDefine_h__
#define SharePublicFuncDefine_h__

#include "ShareMacroDefine.h"
#include <QtNetwork/QNetworkRequest>
#include <QtCore/QString>

namespace ShareLibrary
{
	//获取状态描述
	//QString getStateDescription(ShareLibrary::EAuthorizeState eState);

	//获取错误描述
	//QString getErrorCodeDescription(ShareLibrary::EShareResultCode eCode);

	//获取Flickr权限名称 ShareLibrary::EFlickrPrivacy
	QString getFlickrPrivacyDescription(int nPrivacy);

	//根据Flickr权限获标识值  ShareLibrary::EFlickrPrivacy
	void getFlagValueByPrivacy(int nPrivacy, int& isFamily, int& isFriend, int& isPublic);

	//生成当前时间戳
	QString generateTimeStampString();
	uint generateTimeStamp();

	//生成OAuth中使用的Nonce
	QByteArray generateNonce();

	//HMAC-SHA1加密
	QByteArray hmacSha1(const QByteArray& message, const QByteArray& key);

	//获取网页请求类型字符串
	QString getHttpTypeString(ShareLibrary::HttpMethod eMethod);

	//创建加密请求
	QNetworkRequest createSSLRequest(const QString& strUrl);
	void createSSLRequest(QNetworkRequest& request, const QUrl& url);

	//获取页面加载结果字符串
	QString getPageLoadResultString(ShareLibrary::EPageLoadResult eResult);

	//解析Facebook相册信息
	bool readFacebookAlbumsInfoByByteArray(const QByteArray& json, ShareLibrary::MapAlbumInfo& mapAlbumInfo);

	//解析Flickr相册信息
	bool readFlickrAlbumsInfoByByteArray(const QByteArray& json, ShareLibrary::MapAlbumInfo& mapAlbumInfo);

	//移动窗体到屏幕中间
	void moveWidgetToScreenCenter(QWidget* pWidget);
}
#endif // SharePublicFuncDefine_h__
