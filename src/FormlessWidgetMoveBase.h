/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           FormlessWidgetMoveBase.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-12-30
  *
  *Description:		没有边框的窗体移动基类
**********************************************************************************/
#ifndef FormlessWidgetMoveBase_h__
#define FormlessWidgetMoveBase_h__

#include <QtCore/QPoint>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainterPath>

class CFormlessWidgetMoveBase
{
public:
	CFormlessWidgetMoveBase();

protected:
	void onMousePressed(QWidget* widget, QMouseEvent* event);
	void onMouseMoving(QWidget* widget, QMouseEvent* event);
	void onMouseRelease(QWidget* widget, QMouseEvent* event);

	void setCanMoveWithMouse(bool can) { m_bCanMoveWithMouse = can; }
	bool canMoveWithMouse() const
	{ 
		return m_bCanMoveWithMouse; 
	}
    
    
	void setMoveState(bool moving) { m_bIsMove = moving; }
	bool isMoving() const { return m_bIsMove; }
    void setIsUpdate(bool isUpdata){m_bIsUpdate=isUpdata;}

	//检测鼠标点击类型
	virtual bool checkMousePressedType(QMouseEvent* event);

	//可以按住拖动窗体的区域
	virtual QPainterPath canDragArea() const = 0;

private:
	//移动窗体用参数
	QPoint																		m_oriPos;
	QPoint																		m_point;
	bool																			m_bIsMove;
	bool																			m_bCanMoveWithMouse;
    bool  m_bIsUpdate;
};

#endif // FormlessWidgetMoveBase_h__
