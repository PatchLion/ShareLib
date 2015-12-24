#ifndef ShareMacroDefine_h__
#define ShareMacroDefine_h__

#include <QtCore/QMap>
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)		if(p) { delete p; p = 0;}
#endif // !SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)		if(p) { delete[] p; p = 0;}
#endif // !SAFE_DELETE_ARRAY

#ifndef SAFE_DELETE_QBJECT
#define SAFE_DELETE_QBJECT(p)		if(p) { p->deleteLater(); p = 0;}
#endif // !SAFE_DELETE_QBJECT


#define DEFAULT_TIMEOUT_INTERVAL			(60*1000) //默认超时时间

namespace ShareLibrary
{
	
	typedef QMap < QString, QString >		MapAlbumInfo;				//相册信息(相册名称-相册ID)		

	
	//Flickr分享权限
	enum EFlickrPrivacy
	{
		Public,
		Friend,
		Family,
		Friend_and_Family,
		Privacy,
	};

	//HTTP请求类型
	enum HttpMethod 
	{
		Get, 
		Post, 
		Put, 
		Delete,
	};

	//授权网页显示状态
	enum EAuthorizePageState
	{
		ShowDefault,						//默认状态
		ShowWebView,					//显示网页
		ShowFailed,						//显示错误
		ShowProcess,						//显示进度
		ShowSuccess,						//获取成功

		AUTHPAGE_STATECOUNT,		//授权网页显示状态数
	};



	enum EPageLoadResult
	{
		Result_Success,
		Result_TimeOut,
		Result_SSLError,
		Result_InvaildURL,
		Result_FailedToAuth,
		Result_UserCancel,
		Result_OtherError,
	};
}

#endif // ShareMacroDefine_h__
