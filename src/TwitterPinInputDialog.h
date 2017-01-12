
#ifndef TwitterPinInputDialog_h__
#define TwitterPinInputDialog_h__

#include "ui_PinInputDialog.h"
#include <QtWidgets/QDialog>
#include "FormlessWidgetMoveBase.h"

class CTwitterPinInputDialog : public QDialog, public Ui_PinInputDialog, public CFormlessWidgetMoveBase
{   
	Q_OBJECT

public: 
	CTwitterPinInputDialog(QWidget *parent = 0);
	~CTwitterPinInputDialog();

	//用户输入的pin码
	QString pin() const;
protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void showEvent(QShowEvent *event);

	void paintEvent(QPaintEvent *event);

	//可以按住拖动窗体的区域
	virtual QPainterPath canDragArea() const;

protected Q_SLOTS:
	void on_pushButtonCloseWin_clicked(bool checked);
	void on_pushButtonCloseMac_clicked(bool checked);
	void on_pushButtonCancel_clicked(bool checked);
	void on_pushButtonOK_clicked(bool checked);
	void on_lineEditPin_textEdited(const QString & text);

private:
	QPixmap m_background;
};
#endif // TwitterPinInputDialog_h__
