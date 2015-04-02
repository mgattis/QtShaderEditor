#ifndef LOG_H
#define LOG_H

#include <iostream>

#include <QString>

namespace slog
{
	void out( QString message , bool newline = true );
	void log( QString message , QString subMessage , bool newline = true );
	void err( QString message , QString subMessage , bool newline = true );
}

#endif // LOG_H
