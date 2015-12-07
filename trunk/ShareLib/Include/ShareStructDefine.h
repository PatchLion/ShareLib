#ifndef ShareStructDefine_h__
#define ShareStructDefine_h__

#include <QtCore/QString>
#include "ShareMacroDefine.h"

class IShareParam
{
	virtual bool checkParamValid() const = 0;
};

//Facebook 分享参数
class CFacebookShareParam : public IShareParam
{
public:
	QString description;			//描述
	QString albumID;				//相册ID
	QString filePath;					//文件路径

public:
	virtual bool checkParamValid() const
	{
		return !filePath.isEmpty();
	}
};

//Flickr分享参数
class CFlickrShareParam : public IShareParam
{
public:
	QString	title;							//标题
	QString	description;			//描述
	QString	albumID;				//相册ID
	QString	filePath;					//文件路径
	int			privacyType;			//权限类型

public:
	virtual bool checkParamValid() const
	{
		return !filePath.isEmpty();
	}
};

//Twitter分享参数
class CTwitterShareParam : public IShareParam
{
public:
	QString	description;			//描述
	QString	filePath;					//文件路径

public:
	virtual bool checkParamValid() const
	{
		return !filePath.isEmpty();
	}
};

#endif // ShareStructDefine_h__
