#ifndef CJSONTEMPLATE_H
#define CJSONTEMPLATE_H

#include "qtil.h"
//#include "VJsonForm.h"

#include <iostream>

#include <QByteArray>
#include <QCoreApplication>
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
#include <QVariantList>
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
		valueList = NULL;
	}

	inline QString getValueName( void ) const { return type == structure ? value.toString() : getValueName( type ); }

	QString key;
	Type type;
	QVariant value;
	QVariantList *valueList;

	/*
	union
	{
		QList< void* > voidList;
		QList< int* > intList;
		QList< float* > floatList;
		QList< QString* > stringList;
	};
	*/

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
	QVariantList *valueList; // Do not delete. We do not own this
};

struct CJsonStructure
{
	~CJsonStructure() { UTIL_deleteVectorAsPointerArray( keyvalues ); }

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

	//void validate( QJsonArray &cmp, const QJsonValue::Type type );
	void validate( QJsonArray &arr, const QJsonArray &ref );
	void validate( QJsonObject &obj , const QJsonObject &ref );

public:
	QJsonObject createTree( const QString &name , bool gui ) const;
	void createTree( const QString &name , QTreeWidgetItem *parent , bool guiOnly ) const;
	void createTree( const QString &name , const QJsonObject &obj , QTreeWidgetItem *parent ) const;
	QStringList getKeysForStructure( const QString name );

	QJsonObject loadUserJson( const QString &path , QString &type ) const;

public:
	static CJsonTemplate* get( void )
	{
		static CJsonTemplate *instance = new CJsonTemplate();
		return instance;
	}
};

#endif // CJSONTEMPLATE_H
