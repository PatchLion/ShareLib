/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           AuthorizeWebviewBase.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-14
  *
  *Description:			请求授权网页接口基类
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
	//清空Token
	virtual void clearToken() = 0;

	//设置显示的状态图标
	void setStatePixmap(ShareLibrary::EAuthorizePageState state, const QPixmap& pixmap);

	//设置背景颜色
	void setBackgroundColor(const QColor& color);

	//设置文字字体
	void setTextFont(const QFont& font) { m_textFont = font; update(); }
	QFont textFont() const { return m_textFont; }

	//设置文字颜色
	void setTextColor(const QColor& color){ m_textColor = color; update(); }
	QColor textColor() const{ return m_textColor; }

	//Cookie
	virtual void clearCookie();

	//重置
	virtual void reset() = 0;
protected:
	void paintEvent(QPaintEvent *pEvent);
	void resizeEvent(QResizeEvent *event);
	void closeEvent (QCloseEvent* pEvent);
	void hideEvent(QHideEvent *pEvent);
    void showEvent(QShowEvent* pEvent);
	QSize sizeHint() const;

	CNetworkReplyTempObjManager& replyTempObjectManager() { return m_replyObjectManager; }


	//开始请求授权
	virtual void startToAuthorize();

	//状态
	ShareLibrary::EAuthorizePageState state() const;
	void setState(ShareLibrary::EAuthorizePageState eState);

	//加载Url
	void loadUrl(const QString& strURL);
	QString urlString() const;

	//网页内容(去掉HTML标记)
	QString pagePlainText() const;

	//第一次加载完成
	bool isFirstLoad() const  { return m_bIsFirstLoad; }

	//设置最后的错误结果
	void setResultCode(ShareLibrary::EPageLoadResult eLastResult);

	//启动延时关闭
	void startDelayClose();

private:
	void initWebview();

	//设置背景图片
	void setBackgroundImage(const QPixmap& pixmap);

protected slots :
	//页面加载完毕(根据eResult判断成功或失败)
	virtual void onPageLoadFinished(ShareLibrary::EPageLoadResult eResult);

	//页面加载进度
	virtual void onPageLoadProcess(int process);

	//授权地址
	virtual QString authorizationUrl() const{ return ""; }

	//延时关闭
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
	ShareLibrary::EAuthorizePageState	m_ePageState;																						//网页状态
	ShareLibrary::EPageLoadResult			m_eLastResult;																						//最后的结果代码
	int															m_nCurrentProcess;																				//当前进度值
	QPixmap													m_stateIconImage[ShareLibrary::AUTHPAGE_STATECOUNT];		//状态Icon
	QPixmap													m_backgroundImage;																			//背景图片
	CWebviewContainer*							m_pWebview;
	QColor														m_backgroundColor;
};
#endif // AuthorizeWebviewBase_h__
