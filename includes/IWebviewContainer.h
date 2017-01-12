/*********************************************************************************
  * Copyright(C) 2009   ChengDu Everimaging
  *
  *FileName:           IWebviewContainer.h
  *
  *Author:         	   dai xiaobing
  *
  *Date:         	       2015-8-28
  *
  *Description:		网页容器接口
**********************************************************************************/
#ifndef IWebviewContainer_h__
#define IWebviewContainer_h__

#include "ShareMacroDefine.h"


class IWebviewContainer
{
public:
    //加载Url
	virtual void loadUrl(const QString& strURL) = 0;
	virtual QString urlString() const = 0;

	//网页内容(去掉HTML标记)
	virtual QString pagePlainText() const = 0;
    
    //Cookie
	virtual void clearCookie() = 0;

	//页面加载完毕(根据eResult判断成功或失败)
	virtual void onPageLoadFinished(ShareLibrary::EPageLoadResult eResult) = 0;

	//页面加载进度
	virtual void onPageLoadProcess(int process) = 0;
};
#endif // IWebviewContainer_h__
