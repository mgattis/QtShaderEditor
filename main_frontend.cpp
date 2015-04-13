#include <main_frontend.h>

QTEST_MAIN( TestFrontend )

void TestFrontend::initTestCase( void )
{
	// Nothing to do
}

bool TestFrontend::testObj( CObj *obj , const QString &filePath , const QString &basePath )
{
	if( obj )
	{
		QStringList paths;

		for( int materialIndex = 0 ; materialIndex < obj->materialArray.size() ; materialIndex++ )
		{
			for( int index = 0 ; index < 4 ; index++ )
			{
				QString materialName;

				switch( index )
				{
					case 0:
						materialName = obj->materialArray[ materialIndex ]->ambient_texname.data();
						break;
					case 1:
						materialName = obj->materialArray[ materialIndex ]->diffuse_texname.data();
						break;
					case 2:
						materialName = obj->materialArray[ materialIndex ]->specular_texname.data();
						break;
					case 3:
						materialName = obj->materialArray[ materialIndex ]->normal_texname.data();
						break;
					default: {}
				}

				if( !materialName.isEmpty() && !paths.contains( materialName ) )
				{
					QString path = basePath + "/" + materialName;
					path = path.replace( '\\' , '/' );
					QImage *image = new QImage( path );

					if( image->isNull() )
					{
						QString message = "Failed to load: %1";
						QWARN( message.arg( path ).toLatin1().data() );
					}

					delete image;

					paths.append( materialName );
				}
			}
		}

		return true;
	}

	return false;
}

void TestFrontend::loadProjectDir( void )
{
	QString basePath = QDir::current().absolutePath() + "/QtSEProjects/testProject";

	QDirIterator it( basePath , QDirIterator::Subdirectories );
	QFileInfo info;
	QString type;
	bool error = false;

	while( it.hasNext() )
	{
		it.next();
		//qDebug() << it.next();

		info = it.fileInfo();

		if( info.isFile() && info.fileName().endsWith( ".json" ) )
		{
			QJsonObject obj = CJsonTemplate::get()->loadUserJson( info.filePath() , type );

			if( obj.isEmpty() )
			{
				QWARN( QString( "Failed to read: \"%1\"" ).arg( info.filePath() ).toLatin1().data() );
				error = true;
			}
		}
	}

	QVERIFY2( !error , "One or more JSON files failed to load" );
}

void TestFrontend::loadObjGrassblock( void )
{
	QString basePath = QDir::current().absolutePath() + "/resources";
	QString filePath = basePath + "/grassblock.obj";

	CObj *obj = CObj::loadFromPath( filePath , basePath );
	testObj( obj , filePath , basePath );
	//QVERIFY( testObj( obj , filePath , basePath ) );
	delete obj;
}

void TestFrontend::loadObjMinecraft( void )
{
	QString basePath = QDir::current().absolutePath() + "/resources";
	QString filePath = basePath + "/minecraft.obj";

	CObj *obj = CObj::loadFromPath( filePath , basePath );
	QVERIFY( testObj( obj , filePath , basePath ) );
	delete obj;
}

void TestFrontend::loadObjSponza( void )
{
	QString basePath = QDir::current().absolutePath() + "/resources/dabrovic-sponza";
	QString filePath = basePath + "/sponza.obj";

	CObj *obj = CObj::loadFromPath( filePath , basePath );
	QVERIFY( testObj( obj , filePath , basePath ) );
	delete obj;
}

void TestFrontend::loadObjHead( void )
{
	QString basePath = QDir::current().absolutePath() + "/resources/head";
	QString filePath = basePath + "/head.OBJ";

	CObj *obj = CObj::loadFromPath( filePath , basePath );
	QVERIFY( testObj( obj , filePath , basePath ) );
	delete obj;
}

void TestFrontend::loadObjLostEmpire( void )
{
	QString basePath = QDir::current().absolutePath() + "/resources/lost-empire";
	QString filePath = basePath + "/lost_empire.obj";

	CObj *obj = CObj::loadFromPath( filePath , basePath );
	QVERIFY( testObj( obj , filePath , basePath ) );
	delete obj;
}

void TestFrontend::loadObjRungholt( void )
{
	QString basePath = QDir::current().absolutePath() + "/resources/rungholt";
	QString filePath = basePath + "/rungholt.obj";

	CObj *obj = CObj::loadFromPath( filePath , basePath );
	QVERIFY( testObj( obj , filePath , basePath ) );
	delete obj;
}

void TestFrontend::loadObjSanMiguel( void )
{
	QString basePath = QDir::current().absolutePath() + "/resources/san-miguel";
	QString filePath = basePath + "/san-miguel.obj";

	CObj *obj = CObj::loadFromPath( filePath , basePath );
	QVERIFY( testObj( obj , filePath , basePath ) );
	delete obj;
}

void TestFrontend::cleanupTestCase( void )
{
	// Nothing to do
}
