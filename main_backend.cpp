#include <main_frontend.h>

QTEST_MAIN( TestQString )

void TestQString::toUpper()
{
	QString str = "Hello";
	QCOMPARE( str.toUpper() , QString( "HELLO" ) );
}
