#include "AuthorizeWebviewBase.h"
#include "WebviewContainer.h"
#include "SharePublicFuncDefine.h"
#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtGui/QFontMetrics>

CAuthorizeWebviewBase::CAuthorizeWebviewBase(QWidget *parent)
: QDialog(parent)
, m_pWebview(0)
, m_ePageState(ShareLibrary::ShowDefault)
, m_nCurrentProcess(0)
, m_bIsFirstLoad(true)
, m_backgroundColor("#262626")
, m_bUserCancel(false)
{
	m_textColor = QColor("#484848");
	m_textFont.setWeight(2);
	m_textFont.setFamily(QString::fromLocal8Bit("Î¢ÈíÑÅºÚ"));
	m_textFont.setPointSize(18);
  
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	initWebview();
}


CAuthorizeWebviewBase::~CAuthorizeWebviewBase()
{
	//disconnect(this);
}

void CAuthorizeWebviewBase::closeEvent(QCloseEvent* pEvent)
{
	emit widgetClose();
	QDialog::closeEvent(pEvent);
}

void CAuthorizeWebviewBase::setState(ShareLibrary::EAuthorizePageState eState)
{
	m_ePageState = eState;

	switch(eState)
	{
	case ShareLibrary::ShowDefault:
	case ShareLibrary::ShowFailed:
	case ShareLibrary::ShowProcess:
	case ShareLibrary::ShowSuccess:
		{

			setStyleSheet(QString("background-color: %1").arg(m_backgroundColor.name()));

			if (m_pWebview)
			{
				m_pWebview->setVisible(false);
			}
		}
		break;

	case ShareLibrary::ShowWebView:
		{
			setStyleSheet("background-color: #FFFFFF");

			if (m_pWebview)
			{
				m_pWebview->setVisible(true);
			}
		}
		break;
	}

	repaint();
}

void CAuthorizeWebviewBase::showEvent(QShowEvent* pEvent)
{
	QDialog::showEvent(pEvent);

	setState(ShareLibrary::ShowDefault);

	startToAuthorize();
}

void CAuthorizeWebviewBase::paintEvent( QPaintEvent* pEvent )
{
	QPainter painter(this);

	switch (state())
	{
	case ShareLibrary::ShowDefault:
	case ShareLibrary::ShowFailed:
	case ShareLibrary::ShowProcess:
	case ShareLibrary::ShowSuccess:
	{
		painter.drawPixmap(rect(), m_backgroundImage);

		QPixmap tempImage = m_stateIconImage[m_ePageState].scaled(m_centerIconRect.size(), Qt::KeepAspectRatio);
		painter.drawPixmap(m_centerIconRect.center().x() - tempImage.width() / 2, m_centerIconRect.center().y() - tempImage.height()/2, tempImage);
		QString strDesc;
		if (ShareLibrary::ShowDefault == state())
		{
			strDesc = tr("Loading authorization page......", "ShareLib");
		}
		else if (ShareLibrary::ShowFailed == state())
		{
			strDesc = ShareLibrary::getPageLoadResultString(m_eLastResult);
		}
		else if (ShareLibrary::ShowProcess == state())
		{
			strDesc = tr("Loading...... %1%", "ShareLib").arg(m_nCurrentProcess);
		}
		else if (ShareLibrary::ShowSuccess == state())
		{
			strDesc = tr("Successfully authorized!", "ShareLib");
		}

		painter.setFont(m_textFont);
		painter.setPen(m_textColor);
		const QFontMetrics fontM(m_textFont);
		const QRect textRect = fontM.boundingRect(strDesc);

		painter.drawText(m_textPos.x() - textRect.width()/2, m_textPos.y(), strDesc);


	}
		break;
	}
	
}

void CAuthorizeWebviewBase::initWebview()
{
	if (!m_pWebview)
	{
		m_pWebview = new CWebviewContainer(this);
        m_pWebview->hide();
		m_pWebview->loadUrl("");
		connect(m_pWebview, &CWebviewContainer::pageLoadProcess, this, &CAuthorizeWebviewBase::onPageLoadProcess);
		connect(m_pWebview, &CWebviewContainer::pageLoadFinished, this, &CAuthorizeWebviewBase::onPageLoadFinished);
		QHBoxLayout* pMainLayout = new QHBoxLayout(this);
		pMainLayout->setMargin(0);
		pMainLayout->setSpacing(0);
		pMainLayout->addWidget(m_pWebview);
	}
}

void CAuthorizeWebviewBase::hideEvent( QHideEvent *pEvent )
{
	QDialog::hideEvent(pEvent);
}

ShareLibrary::EAuthorizePageState CAuthorizeWebviewBase::state() const
{
	return m_ePageState;
}

QSize CAuthorizeWebviewBase::sizeHint() const
{
	return QSize(800, 600);
}

void CAuthorizeWebviewBase::onPageLoadProcess(int process)
{
	m_nCurrentProcess = process;
}

void CAuthorizeWebviewBase::resizeEvent(QResizeEvent *event)
{
	QDialog::resizeEvent(event);

	const int ICONOFFSET = 30;
	const int TEXTOFFSET = 60;
	const int ICONLENGTH = width() * 1.0 / 5.0;
	const QPoint centerPoint = rect().center();
	m_centerIconRect = QRect(centerPoint.x() - ICONLENGTH / 2, centerPoint.y() - ICONLENGTH / 2 - ICONOFFSET, ICONLENGTH, ICONLENGTH);

	m_textPos = QPoint(centerPoint.x(), m_centerIconRect.y() + m_centerIconRect.height() + TEXTOFFSET);
}

void CAuthorizeWebviewBase::onPageLoadFinished(ShareLibrary::EPageLoadResult eResult)
{
	m_eLastResult = eResult;
	m_bIsFirstLoad = false;
	if (ShareLibrary::Result_Success == eResult)
	{
		setState(ShareLibrary::ShowWebView);
	}
	else
	{
		setState(ShareLibrary::ShowFailed);
	}
}

void CAuthorizeWebviewBase::loadUrl(const QString& strURL)
{
	if (m_pWebview)
	{
		m_pWebview->loadUrl(strURL);
	}

}

void CAuthorizeWebviewBase::startToAuthorize()
{
	clearToken();
	m_bIsFirstLoad = true;
	m_nCurrentProcess = 0;
}

QString CAuthorizeWebviewBase::urlString() const
{
	if (m_pWebview)
	{
		return m_pWebview->urlString();
	}

	return "";
}

QString CAuthorizeWebviewBase::pagePlainText() const
{
	if (m_pWebview)
	{
		return m_pWebview->pagePlainText();
	}

	return "";
}

void CAuthorizeWebviewBase::setResultCode(ShareLibrary::EPageLoadResult eLastResult)
{
	m_eLastResult = eLastResult;

	update();
}

void CAuthorizeWebviewBase::clearCookie()
{
	if (m_pWebview)
	{
		m_pWebview->clearCookie();
	}
}

void CAuthorizeWebviewBase::onDelayClose()
{
	close();

	reset();
}

void CAuthorizeWebviewBase::startDelayClose()
{
	QTimer::singleShot(2000, this, SLOT(onDelayClose()));
}

void CAuthorizeWebviewBase::setStatePixmap(ShareLibrary::EAuthorizePageState state, const QPixmap& pixmap)
{
	m_stateIconImage[state] = pixmap;
}

void CAuthorizeWebviewBase::setBackgroundColor(const QColor& color)
{
	m_backgroundColor = color;

	repaint();
}
