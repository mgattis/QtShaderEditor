#ifndef CJSONTEMPLATE_H
#define CJSONTEMPLATE_H

#include "VJsonForm.h"

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
#include <QTreeWidgetItem>
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
		guiInsert = indexable = projectInsert = false;
	}

	inline QString getValueName( void ) const { return type == structure ? value.toString() : getValueName( type ); }

	QString key;
	Type type;
	QVariant value;
	bool guiInsert , indexable , projectInsert;

	static QString getValueName( Type type )
	{
		switch( type )
		{
			case structure:
				return "structure";
			case boolean:
				return "bool";
			case integer:
				return "int";
			case floating:
				return "float";
			case string:
				return "string";
			default: {}
		}

		return "unknown";
	}
};

class VJsonFormItem : public QTreeWidgetItem
{
public:
	VJsonFormItem( QTreeWidgetItem *parent , CJsonKeyvalueData::Type type , bool isArray , const QVariant &defaultValue );

protected:
	void toArray( QJsonArray &array ) const;
	void toStructure( QJsonObject &obj ) const;

public:
	void toObject( QJsonObject &obj ) const;

public:
	CJsonKeyvalueData::Type type;
	bool isArray;
	QVariant defaultValue , lastValue;
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

protected:
	QMap< QString , CJsonStructure* > structMap;

protected:
	void parseDefaults( const QString &path /* = QString() */ );

	void validate( QJsonArray &arr, const QJsonArray &ref );
	void validate( QJsonObject &obj , const QJsonObject &ref );

public:
	QJsonObject createTree( const QString &name , bool gui ) const;
	void createTree( const QString &name , QTreeWidgetItem *parent ) const;
	void createTree( const QString &name , const QJsonObject &obj , QTreeWidgetItem *parent ) const;

	QJsonObject loadUserJson( const QString &path ) const;

public:
	static CJsonTemplate* get( void )
	{
		static CJsonTemplate *instance = new CJsonTemplate();
		return instance;
	}
};

#endif // CJSONTEMPLATE_H
