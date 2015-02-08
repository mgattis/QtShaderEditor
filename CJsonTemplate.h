#ifndef CJSONTEMPLATE_H
#define CJSONTEMPLATE_H

#include <iostream>

#include <QByteArray>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVector>

struct CJsonKeyvalueData
{
	enum Type
	{
		structure = 0,
		boolean,
		integer,
		floating,
		string
	};

	CJsonKeyvalueData( void )
	{
		autoInsert = guiInsert = indexable = false;
	}

	QString key;
	Type type;
	QVariant value;
	bool autoInsert , guiInsert , indexable;
};

struct CJsonStructure
{
	QVector< CJsonKeyvalueData* > keyvalues;
};

class CJsonTemplate
{
private:
	CJsonTemplate( const QString &path = QString() );
	~CJsonTemplate();

public:
	QMap< QString , CJsonStructure* > structMap;

protected:
	void parseDefaults( const QString &path /* = QString() */ );
	void createTree( const QString &name );

public:
	static CJsonTemplate* get( void )
	{
		static CJsonTemplate *instance = new CJsonTemplate();
		return instance;
	}
};

#endif // CJSONTEMPLATE_H