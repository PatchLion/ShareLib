#include "TwitterPinInputDialog.h"
#include <QtGui/QPainter>
CTwitterPinInputDialog::CTwitterPinInputDialog( QWidget *parent/*=0*/ )
	: QDialog(parent)
{
	setupUi(this);

	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	pushButtonOK->setEnabled(false);
	

	lineEditPin->setPlaceholderText(tr("Please enter the PIN code returned by Twitter"));

	setCanMoveWithMouse(true);
	m_background.load(":/images/background.png");
#ifdef Q_OS_WIN
	pushButtonCloseMac->setVisible(false);
#endif

#ifdef Q_OS_MAC
	pushButtonCloseWin->setVisible(false);
#endif
}
 

CTwitterPinInputDialog::~CTwitterPinInputDialog()
{
}

void CTwitterPinInputDialog::on_pushButtonCloseWin_clicked(bool checked)
{
	reject();
}

void CTwitterPinInputDialog::on_pushButtonCloseMac_clicked(bool checked)
{
	reject();
}

void CTwitterPinInputDialog::on_pushButtonCancel_clicked(bool checked)
{
	reject();
}

void CTwitterPinInputDialog::on_pushButtonOK_clicked(bool checked)
{
	if (!lineEditPin->text().isEmpty())
	{
		accept();
	}
}

void CTwitterPinInputDialog::on_lineEditPin_textEdited(const QString & text)
{
	pushButtonOK->setEnabled(!text.isEmpty());
}

QString CTwitterPinInputDialog::pin() const
{
	return lineEditPin->text();
}

void CTwitterPinInputDialog::mousePressEvent(QMouseEvent* event)
{
	CFormlessWidgetMoveBase::onMousePressed(this, event);
	QDialog::mousePressEvent(event);
}

void CTwitterPinInputDialog::mouseMoveEvent(QMouseEvent* event)
{
	CFormlessWidgetMoveBase::onMouseMoving(this, event);
	QDialog::mouseMoveEvent(event);
}

void CTwitterPinInputDialog::mouseReleaseEvent(QMouseEvent* event)
{
	CFormlessWidgetMoveBase::onMouseRelease(this, event);
	QDialog::mouseReleaseEvent(event);
}

QPainterPath CTwitterPinInputDialog::canDragArea() const
{
	QPainterPath path;
	path.addRect(0, 0, width(), 30);

	return path;
}

void CTwitterPinInputDialog::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), m_background);
}

void CTwitterPinInputDialog::showEvent(QShowEvent *event)
{
	lineEditPin->setSelection(0, 0);
}

