#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

#include "IFileClass.h"
#include "CPath.h"

#include <QObject>
#include <QString>

class IJSONResource : public IFileClass< IJSONResource >
{
	//
};

class CTexture
{
	//
};

class CModel
{
	//
};

class CShader
{
	//
};

class CFramebuffer
{
	//
};

class CStage
{
	//
};

class CResourceManager
{
private:
	CResourceManager();
	~CResourceManager();

protected:
	void loadResource( QString path );
	//CStage* getStage( QString name );

protected:
	// TODO: Internal arrays

public:
	static CResourceManager* getInstance( void )
	{
		static CResourceManager* instance = new CResourceManager();
		return instance;
	}
};

#endif // CRESOURCEMANAGER_H
