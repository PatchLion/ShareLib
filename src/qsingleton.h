/*********************************************************************************
  * Copyright(C) 2009-2015   ChengDu Everimaging
  *
  *FileName:           qsingleton.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-7-15
  *
  *Description:			Qt线程安全单例
**********************************************************************************/
#ifndef QSINGLETON_H
#define QSINGLETON_H

#include <QtCore/QtGlobal>
#include <QtCore/QScopedPointer>
#include "call_once.h"

template<class T>
class Singleton
{
public:
	static T& instance()
	{
		qCallOnce(init, flag); 
		return *tptr;
	}

	static void init()
	{
		tptr.reset(new T);
	}

private:
	Singleton(){};
	~Singleton(){};
	Q_DISABLE_COPY(Singleton)

	static QScopedPointer<T> tptr;
	static QBasicAtomicInt flag;
};

template<class T> QScopedPointer<T> Singleton<T>::tptr(0);
template<class T> QBasicAtomicInt Singleton<T>::flag = Q_BASIC_ATOMIC_INITIALIZER(CallOnce::CO_Request);

#endif // QSINGLETON_H
