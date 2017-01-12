//#include "stdafx.h"
#include "FormlessWidgetMoveBase.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QApplication>
CFormlessWidgetMoveBase::CFormlessWidgetMoveBase()
	: m_bIsMove(false)
	, m_bCanMoveWithMouse(false)
	, m_bIsUpdate(true)
{

}

void CFormlessWidgetMoveBase::onMousePressed(QWidget* widget, QMouseEvent* event)
{
	if (canDragArea().contains(event->pos()) 
		&& widget
		&& checkMousePressedType(event)
		&& canMoveWithMouse())
	{
		m_oriPos = widget->pos();
		m_point = event->globalPos();
		setMoveState(true);
	}
}

void CFormlessWidgetMoveBase::onMouseMoving(QWidget* widget, QMouseEvent* event)
{
	if (isMoving() && widget)
	{
		QRect activeRect;
		activeRect.setTopLeft(QPoint(0, 0));

		if (widget->parentWidget() && !widget->windowFlags().testFlag(Qt::Dialog))
		{
			activeRect.setSize(widget->parentWidget()->size());
		}
		else
		{
			QDesktopWidget* desktopWidget = QApplication::desktop();//获取可用桌面大小
			if (desktopWidget)
			{
				activeRect.setSize(desktopWidget->availableGeometry().size());
			}
		}

		QPoint newPos = m_oriPos + (event->globalPos() - m_point);

		const int WOFFSET = 20;
		const int HOFFSET = 20;

		if (newPos.x() < activeRect.x() - widget->width() + WOFFSET)
		{
			newPos.setX(activeRect.x() - widget->width() + WOFFSET);
		}
		if (newPos.y() < activeRect.y())
		{
			newPos.setY(activeRect.y());
		}
		if (newPos.x() > (activeRect.x() + activeRect.width() - WOFFSET))
		{
			newPos.setX(activeRect.x() + activeRect.width() - WOFFSET);
		}
		if (newPos.y() > (activeRect.y() + activeRect.height() - HOFFSET))
		{
			newPos.setY(activeRect.y() + activeRect.height() - HOFFSET);
		}
		widget->move(newPos);
        if(m_bIsUpdate)
        {
            widget->update();
        }
	}
}

void CFormlessWidgetMoveBase::onMouseRelease(QWidget* widget, QMouseEvent* event)
{
	setMoveState(false);
}

bool CFormlessWidgetMoveBase::checkMousePressedType(QMouseEvent* event)
{
	return (event->button() == Qt::LeftButton
		&& event->modifiers() == Qt::NoModifier);
}

