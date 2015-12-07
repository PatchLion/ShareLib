/*********************************************************************************
  * 
  *
  *FileName:           FaceBookAuthorizeWebview.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-29
  *
  *Description:		FaceBook��Ȩ��ҳ
**********************************************************************************/
#ifndef FaceBookAuthorizeWebview_h__
#define FaceBookAuthorizeWebview_h__

#include "ShareMacroDefine.h"
#include "AuthorizeWebviewBase.h"

class CWebviewContainer;
class CFaceBookAuthorizeWebview : public CAuthorizeWebviewBase
{   
	Q_OBJECT

public: 
	CFaceBookAuthorizeWebview(const QString& clientID, QWidget *parent = 0);
	~CFaceBookAuthorizeWebview();

public:
	//���Token
	virtual void clearToken();

	//Token Key
	QString tokenKey() const { return m_strTokenKey; }

	//��Чʱ���
	uint expiresTimeStamp() const { return m_nExpiresTimeStamp; }

	//ClientID
	QString clientID() const { return m_strClientID; }

	//����
	void reset();
protected:
	//��ʼ������Ȩ
	virtual void startToAuthorize();

	 //��Ȩ��ҳ
	QString authorizationUrl() const;

private:


protected slots :
	//ҳ��������(����eResult�жϳɹ���ʧ��)
	virtual void onPageLoadFinished(ShareLibrary::EPageLoadResult eResult);

	//ҳ����ؽ���
	virtual void onPageLoadProcess(int process);


signals:
	//���յ�Token
	void tokenReceived();

	//�û�ȡ��
	void userCancel();

private:
	const QString				m_strClientID;						//�ͻ���ID

	QString						m_strCancelUrl;						//ȡ��URL
	QString						m_strTokenKey;						//��ȡ����TokenKey
	uint								m_nExpiresTimeStamp;			//Token��Чʱ���
};
#endif // FaceBookAuthorizeWebview_h__
