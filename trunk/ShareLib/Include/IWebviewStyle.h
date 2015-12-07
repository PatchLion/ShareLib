/*********************************************************************************
  * 
  *
  *FileName:           IWebviewStyle.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-9-30
  *
  *Description:		设置webview的样式
**********************************************************************************/
#ifndef IWebviewStyle_h__
#define IWebviewStyle_h__

#include <QtGui/QPixmap>
#include <QtGui/QFont>
#include "ShareMacroDefine.h"

class CAuthorizeWebviewBase;
class IWebviewStyle
{   
public:
	IWebviewStyle()
	{
		Q_INIT_RESOURCE(shareimages);

		setStatePixmap(ShareLibrary::ShowDefault, QPixmap(":/Resources/waiting.png"));
		setStatePixmap(ShareLibrary::ShowFailed, QPixmap(":/Resources/failed.png"));
		setStatePixmap(ShareLibrary::ShowProcess, QPixmap(":/Resources/waiting.png"));
		setStatePixmap(ShareLibrary::ShowSuccess, QPixmap(":/Resources/success.png"));
		m_webViewSize = QSize(800, 600);
		m_textColor = QColor("#484848");
		m_font.setWeight(2);
		m_font.setFamily(QString::fromLocal8Bit("微软雅黑"));
		m_font.setPointSize(18);
		m_bgColor.setNamedColor("#262626");
	}

public:
	//设置显示的状态图标(需调用applyWebViewStyle()后生效)
	void setStatePixmap(ShareLibrary::EAuthorizePageState state, const QPixmap& pixmap);
	QPixmap statePixmap(ShareLibrary::EAuthorizePageState state) const { return m_statePixmap[state]; }

	//设置背景颜色(需调用applyWebViewStyle()后生效)
	void setBackgroundColor(const QColor& color);
	QColor backgroundColor() const { return m_bgColor; }

	//设置文字字体(需调用applyWebViewStyle()后生效)
	void setTextFont(const QFont& font);
	QFont textFont() const { return m_font; }

	//设置文字颜色(需调用applyWebViewStyle()后生效)
	void setTextColor(const QColor& color);
	QColor textColor() const { return m_textColor; }

	//授权网页窗体大小(需调用applyWebViewStyle()后生效)
	void setAuthorizeWebViewSize(const QSize& size);
	QSize authorizeWebViewSize() const { return m_webViewSize; }

	//网页窗体标题(需调用applyWebViewStyle()后生效)
	void setWebViewTitle(const QString& strTitle);
	QString webViewTitle() const { return m_strWebViewTitle; }

	//应用样式设置
	bool applyWebViewStyle();

protected:
	virtual CAuthorizeWebviewBase* webView() = 0;
	
	virtual void releaseWebView() = 0;

private:
	QPixmap	m_statePixmap[ShareLibrary::AUTHPAGE_STATECOUNT];
	QColor		m_bgColor;
	QColor		m_textColor;
	QFont		m_font;
	QSize			m_webViewSize;
	QString		m_strWebViewTitle;
};
#endif // IWebviewStyle_h__
