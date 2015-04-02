#include "log.h"

void slog::out( QString message , bool newline /* = true */ )
{
	std::cout << message.toLatin1().data();

	if( newline )
		std::cout << std::endl;
}

void slog::log( QString message , QString subMessage , bool newline /* = true */ )
{
	std::cout << "Log: " << message.toLatin1().data();

	if( !subMessage.isEmpty() )
		std::cout << " (" << subMessage.toLatin1().data() << ")";

	if( newline )
		std::cout << std::endl;
}

void slog::err( QString message , QString subMessage , bool newline /* = true */ )
{
	std::cout << "Error: " << message.toLatin1().data();

	if( !subMessage.isEmpty() )
		std::cout << " (" << subMessage.toLatin1().data() << ")";

	if( newline )
		std::cout << std::endl;
}
