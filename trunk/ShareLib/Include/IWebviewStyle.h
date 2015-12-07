/*********************************************************************************
  * 
  *
  *FileName:           IWebviewStyle.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-9-30
  *
  *Description:		����webview����ʽ
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
		m_font.setFamily(QString::fromLocal8Bit("΢���ź�"));
		m_font.setPointSize(18);
		m_bgColor.setNamedColor("#262626");
	}

public:
	//������ʾ��״̬ͼ��(�����applyWebViewStyle()����Ч)
	void setStatePixmap(ShareLibrary::EAuthorizePageState state, const QPixmap& pixmap);
	QPixmap statePixmap(ShareLibrary::EAuthorizePageState state) const { return m_statePixmap[state]; }

	//���ñ�����ɫ(�����applyWebViewStyle()����Ч)
	void setBackgroundColor(const QColor& color);
	QColor backgroundColor() const { return m_bgColor; }

	//������������(�����applyWebViewStyle()����Ч)
	void setTextFont(const QFont& font);
	QFont textFont() const { return m_font; }

	//����������ɫ(�����applyWebViewStyle()����Ч)
	void setTextColor(const QColor& color);
	QColor textColor() const { return m_textColor; }

	//��Ȩ��ҳ�����С(�����applyWebViewStyle()����Ч)
	void setAuthorizeWebViewSize(const QSize& size);
	QSize authorizeWebViewSize() const { return m_webViewSize; }

	//��ҳ�������(�����applyWebViewStyle()����Ч)
	void setWebViewTitle(const QString& strTitle);
	QString webViewTitle() const { return m_strWebViewTitle; }

	//Ӧ����ʽ����
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
