/**
 *	@file	MyException.h	
 *	@brief	Define a class for exception.
 *	@author	ISHIN / Broken Desk
 *	@date	April 25th, 2016
 */

#ifndef __MYEXCEPTION_H__
#define __MYEXCEPTION_H__

#include <string>
#include <stdlib.h>
#include <stdarg.h>

#ifdef _WIN32
#define __REP_ESCAPE	(						\
	[]( std::string s ) -> std::string {		\
		auto pos = s.find( "\\" );				\
		while ( pos != std::string::npos ) {	\
			s.replace( pos, 1, "/" );			\
			pos = s.find( "\\", pos + 1 );		\
		}										\
		return s;								\
	} )
#else
#define __REP_ESCAPE(x)	(x)
#endif

#define __FILENAME		( std::string( __FILE__ ).substr( __REP_ESCAPE( std::string( __FILE__ ) ).find_last_of('/') + 1 ).c_str() )
#define ERR_ADD(x,y)	( (x).addErrMsg( "%s\n%s : L.%d\n", (y), __FILENAME, __LINE__ ) )
#define ERR_CREATE(y)	( MyException( "%s\n%s : L.%d\n", (y), __FILENAME, __LINE__ ) )

class MyException {
public:
	MyException() {
	}

	MyException( char *format, ... ) {

		char buf[4096];
		va_list argptr;
		va_start( argptr, format );
		vsprintf_s( buf, sizeof(buf)-1, format, argptr );
		buf[ sizeof(buf)-1 ] = '\0';
		va_end( argptr );

		this->_errmsg = buf;
	}

	~MyException() {
	}

	std::string getErrMsg()	{ 
		return _errmsg; 
	}

	void addErrMsg( char *format, ... ) {

		char buf[4096];
		va_list argptr;
		va_start( argptr, format );
		vsprintf_s( buf, sizeof(buf)-1, format, argptr );
		buf[  sizeof(buf)-1 ] = '\0';
		va_end( argptr );

		this->_errmsg += buf;

	}

	void printErrMsg() { 
		fprintf( stderr, _errmsg.c_str() );
	}


protected:
	std::string	_errmsg;

};

#endif //__MYEXCEPTION_H__
