/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           AuthorizeWebviewBase.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-14
  *
  *Description:			������Ȩ��ҳ�ӿڻ���
**********************************************************************************/
#ifndef AuthorizeWebviewBase_h__
#define AuthorizeWebviewBase_h__
#include "ShareMacroDefine.h"
#include "NetworkReplyTempObjManager.h"

#include <QtCore/qglobal.h>
#if(QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#else
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#endif

class CWebviewContainer;
class CAuthorizeWebviewBase : public QDialog
{   
	Q_OBJECT

public: 
	CAuthorizeWebviewBase(QWidget *parent = 0);
	~CAuthorizeWebviewBase();

public:
	//���Token
	virtual void clearToken() = 0;

	//������ʾ��״̬ͼ��
	void setStatePixmap(ShareLibrary::EAuthorizePageState state, const QPixmap& pixmap);

	//���ñ�����ɫ
	void setBackgroundColor(const QColor& color);

	//������������
	void setTextFont(const QFont& font) { m_textFont = font; update(); }
	QFont textFont() const { return m_textFont; }

	//����������ɫ
	void setTextColor(const QColor& color){ m_textColor = color; update(); }
	QColor textColor() const{ return m_textColor; }

	//Cookie
	virtual void clearCookie();

	//����
	virtual void reset() = 0;
protected:
	void paintEvent(QPaintEvent *pEvent);
	void resizeEvent(QResizeEvent *event);
	void closeEvent (QCloseEvent* pEvent);
	void hideEvent(QHideEvent *pEvent);
    void showEvent(QShowEvent* pEvent);
	QSize sizeHint() const;

	CNetworkReplyTempObjManager& replyTempObjectManager() { return m_replyObjectManager; }


	//��ʼ������Ȩ
	virtual void startToAuthorize();

	//״̬
	ShareLibrary::EAuthorizePageState state() const;
	void setState(ShareLibrary::EAuthorizePageState eState);

	//����Url
	void loadUrl(const QString& strURL);
	QString urlString() const;

	//��ҳ����(ȥ��HTML���)
	QString pagePlainText() const;

	//��һ�μ������
	bool isFirstLoad() const  { return m_bIsFirstLoad; }

	//�������Ĵ�����
	void setResultCode(ShareLibrary::EPageLoadResult eLastResult);

	//������ʱ�ر�
	void startDelayClose();

private:
	void initWebview();

	//���ñ���ͼƬ
	void setBackgroundImage(const QPixmap& pixmap);

protected slots :
	//ҳ��������(����eResult�жϳɹ���ʧ��)
	virtual void onPageLoadFinished(ShareLibrary::EPageLoadResult eResult);

	//ҳ����ؽ���
	virtual void onPageLoadProcess(int process);

	//��Ȩ��ַ
	virtual QString authorizationUrl() const{ return ""; }

	//��ʱ�ر�
	void onDelayClose();

signals:
	void widgetClose();

	
private:
	CNetworkReplyTempObjManager		m_replyObjectManager;		//
	bool															m_bIsFirstLoad;
	QColor													m_textColor;
	QFont														m_textFont;
	QRect														m_centerIconRect;
	QPoint													m_textPos;
	ShareLibrary::EAuthorizePageState	m_ePageState;																						//��ҳ״̬
	ShareLibrary::EPageLoadResult			m_eLastResult;																						//���Ľ������
	int															m_nCurrentProcess;																				//��ǰ����ֵ
	QPixmap													m_stateIconImage[ShareLibrary::AUTHPAGE_STATECOUNT];		//״̬Icon
	QPixmap													m_backgroundImage;																			//����ͼƬ
	CWebviewContainer*							m_pWebview;
	QColor														m_backgroundColor;
};
#endif // AuthorizeWebviewBase_h__
