#include "IWebviewStyle.h"
#include "AuthorizeWebviewBase.h"

void IWebviewStyle::setStatePixmap(ShareLibrary::EAuthorizePageState state, const QPixmap& pixmap)
{
	m_statePixmap[state] = pixmap;
}

void IWebviewStyle::setBackgroundColor(const QColor& color)
{
	m_bgColor = color;
}

void IWebviewStyle::setTextFont(const QFont& font)
{
	m_font = font;
}

void IWebviewStyle::setTextColor(const QColor& color)
{
	m_textColor = color;
}

bool IWebviewStyle::applyWebViewStyle()
{
	if (!webView())
	{
		return false;
	}

	for (int nIndex = 0; nIndex < ShareLibrary::AUTHPAGE_STATECOUNT; nIndex++)
	{
		ShareLibrary::EAuthorizePageState state = static_cast<ShareLibrary::EAuthorizePageState>(nIndex);
		webView()->setStatePixmap(state, m_statePixmap[nIndex]);
	}

	webView()->setBackgroundColor(m_bgColor);
	webView()->setTextColor(m_textColor);
	webView()->setTextFont(m_font);
	webView()->setFixedSize(m_webViewSize);
	webView()->setWindowTitle(m_strWebViewTitle);

	webView()->repaint();

	return true;
}

void IWebviewStyle::setAuthorizeWebViewSize(const QSize& size)
{
	m_webViewSize = size;
}

void IWebviewStyle::setWebViewTitle(const QString& strTitle)
{
	m_strWebViewTitle = strTitle;
}
