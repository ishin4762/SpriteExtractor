/**
 *	@file	main.cpp	
 *	@brief	Controlling central process of this application.
 *	@author	ISHIN / Broken Desk
 *	@date	April 25th, 2016
 */

#include <iostream>
using namespace std;

#include "MyException.h"
#include "MyImage.h"

/**
 *	@brief	The entry point of this application.
 *	@param ( argc )	# of arguments.
 *	@param ( argv )	array of arguments.  
 *	@return	Exit code for OS.
 */
int main( int argc, char* argv[] )
{
	int exitCode = 0;
	MyImage *image = NULL;

	try {
		if ( argc < 2 ) {
			cout << "Usage: $ SpriteExtractor <inputFilename> [<outputFilename>]" << endl;
			cout << "  inputFilename: image file ( bmp, png, jpg )" << endl;
			cout << "  outputFilename: result file ( png )" << endl;
		}
		else {
			string inputFilename = argv[1];
			string outputFilename = inputFilename.substr( 0, inputFilename.find_last_of('.') ) + "_out.png";
			if ( argc >= 3 ) {
				outputFilename = argv[2];
			}

			image = new MyImage( inputFilename );
			image->extract( outputFilename );
		}

	}
	catch ( MyException &e ) {
		e.printErrMsg();
		exitCode = 1;
	}
	catch ( ... ) {
		MyException e;
		ERR_ADD( e, "Unknown Error is Occurred!" );
		e.printErrMsg();
		exitCode = 2;
	}

	if ( image != NULL ) {
		delete image;
	}

	return exitCode;
}
