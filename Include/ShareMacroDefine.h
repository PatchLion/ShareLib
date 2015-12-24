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


#define DEFAULT_TIMEOUT_INTERVAL			(60*1000) //Ĭ�ϳ�ʱʱ��

namespace ShareLibrary
{
	
	typedef QMap < QString, QString >		MapAlbumInfo;				//�����Ϣ(�������-���ID)		

	
	//Flickr����Ȩ��
	enum EFlickrPrivacy
	{
		Public,
		Friend,
		Family,
		Friend_and_Family,
		Privacy,
	};

	//HTTP��������
	enum HttpMethod 
	{
		Get, 
		Post, 
		Put, 
		Delete,
	};

	//��Ȩ��ҳ��ʾ״̬
	enum EAuthorizePageState
	{
		ShowDefault,						//Ĭ��״̬
		ShowWebView,					//��ʾ��ҳ
		ShowFailed,						//��ʾ����
		ShowProcess,						//��ʾ����
		ShowSuccess,						//��ȡ�ɹ�

		AUTHPAGE_STATECOUNT,		//��Ȩ��ҳ��ʾ״̬��
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
