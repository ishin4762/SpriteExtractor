/**
 *	@file	MyImage.h	
 *	@brief	Define a class for processing images.
 *	@author	ISHIN / Broken Desk
 *	@date	April 25th, 2016
 */

#ifndef __MYIMAGE_H__
#define __MYIMAGE_H__

#include <iostream>
#include <string>
using namespace std;

class MyImage {
public:
	MyImage();
	MyImage( string inputFilename );
	~MyImage();

	void load( string inputFilename );
	void extract( string outputFilename );

private:
	unsigned char *pixels;
	int width;
	int height;
	int bpp;
};

#endif //__MYIMAGE_H__
