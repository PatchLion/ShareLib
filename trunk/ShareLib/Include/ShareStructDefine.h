#ifndef ShareStructDefine_h__
#define ShareStructDefine_h__

#include <QtCore/QString>
#include "ShareMacroDefine.h"

class IShareParam
{
	virtual bool checkParamValid() const = 0;
};

//Facebook �������
class CFacebookShareParam : public IShareParam
{
public:
	QString description;			//����
	QString albumID;				//���ID
	QString filePath;					//�ļ�·��

public:
	virtual bool checkParamValid() const
	{
		return !filePath.isEmpty();
	}
};

//Flickr�������
class CFlickrShareParam : public IShareParam
{
public:
	QString	title;							//����
	QString	description;			//����
	QString	albumID;				//���ID
	QString	filePath;					//�ļ�·��
	int			privacyType;			//Ȩ������

public:
	virtual bool checkParamValid() const
	{
		return !filePath.isEmpty();
	}
};

//Twitter�������
class CTwitterShareParam : public IShareParam
{
public:
	QString	description;			//����
	QString	filePath;					//�ļ�·��

public:
	virtual bool checkParamValid() const
	{
		return !filePath.isEmpty();
	}
};

#endif // ShareStructDefine_h__
