/**
 *	@file	MyImage.cpp	
 *	@brief	Processing image.
 *	@author	ISHIN / Broken Desk
 *	@date	April 25th, 2016
 */

#include "MyImage.h"
#include "MyException.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stblib/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stblib/stb_image_write.h"

#include <vector>

struct RECT {
	int id;
	int left;
	int right;
	int top;
	int bottom;
public:
	RECT() {
		id = 0;
		left = right = top = bottom = 0;
	}
	RECT( int id, int x, int y ) {
		this->id = id;
		left = right = x;
		top = bottom = y;
	}
};

/**
 *	@brief	Contructor
 */
MyImage::MyImage()
{
	pixels = NULL;
}

/**
 *	@brief	Contructor, load an image.
 */
MyImage::MyImage( string inputFilename )
{
	pixels = NULL;
	
	try {
		load( inputFilename );
	}
	catch ( MyException &e ) {
		ERR_ADD( e, "Error occurred at constructor." );
		throw e;
	}
	catch ( ... ) {
		throw ERR_CREATE( "Unknown error occurred at constructor." );
	}
}

/**
*	@brief	Destructor
*/
MyImage::~MyImage()
{
	if ( pixels ) {
		stbi_image_free( pixels );
	}
}

/**
 *	@brief	Load image.
 *	@param ( inputFilename ) filename of an image file.
 */
void MyImage::load( string inputFilename )
{
	// Get RGBA type pixels from image file.
	pixels = stbi_load( inputFilename.c_str(), &width, &height, &bpp, 4 );
	if ( pixels == NULL ) {
		throw ERR_CREATE( "Can't load image file." );
	}

	// Show image information.
	cout << "Load Image : " << endl;
	cout << "  Filename = " << inputFilename << endl;
	cout << "  Width = " << width << endl;
	cout << "  Height = " << height << endl;
	cout << "  Bpp = " << bpp << endl;

}

static void paintAndSearch(
	unsigned char *pixels, unsigned int *flags, int &nFlag, int x, int y, int &w, int &h, RECT &rect ) {

	pixels[ ( x + y*w ) * 4 + 3 ] = 0;
	flags[ x + y*w ] = nFlag;

	if ( x < rect.left ) {
		rect.left = x;
	}
	if ( x > rect.right ) {
		rect.right = x;
	}
	if ( y < rect.top ) {
		rect.top = y;
	}
	if ( y > rect.bottom ) {
		rect.bottom = y;
	}

	// Up
	if ( y > 0 ) {
		if ( pixels[ ( x + (y-1)*w ) * 4 + 3 ] != 0 ) {
			paintAndSearch( pixels, flags, nFlag, x, y-1, w, h, rect );
		}
	}
	// Down
	if ( y < h-1 ) {
		if ( pixels[ ( x + (y+1)*w ) * 4 + 3 ] != 0 ) {
			paintAndSearch( pixels, flags, nFlag, x, y+1, w, h, rect );
		}
	}
	// Left
	if ( x > 0 ) {
		if ( pixels[ ( (x-1) + y*w ) * 4 + 3 ] != 0 ) {
			paintAndSearch( pixels, flags, nFlag, x-1, y, w, h, rect );
		}
	}
	// Right
	if ( x < w-1 ) {
		if ( pixels[ ( (x+1) + y*w ) * 4 + 3 ] != 0 ) {
			paintAndSearch( pixels, flags, nFlag, x+1, y, w, h, rect );
		}
	}

}

/**
 *	@brief	Extract sprite area.
 *	@param ( outputFilename ) filename of a result file.
 */
void MyImage::extract( string outputFilename )
{
	if ( width <= 0 || height <= 0 ) {
		throw ERR_CREATE( "Image width and height must be over zero." );
	}

	std::vector<RECT> vRect;

	// Step 1. Segmentation
	unsigned int *flags = new unsigned int[ width * height ];
	int nFlag = 0;

	for ( int j=0; j<height; j++ ) {
		unsigned int *wpos = flags + j * width;
		unsigned char *rpos = pixels + j * width * 4;

		for ( int i=0; i<width; i++, wpos++, wpos++, rpos+=4 ) {

			// Seed point
			if ( rpos[3] != 0 ) {
				// search 4 direction
				RECT rect( ++nFlag, i, j );
				paintAndSearch( pixels, flags, nFlag, i, j, width, height, rect );
				vRect.push_back( rect );
			}
		}
	}
	delete[] flags;

	cout << "Sprites:" << endl;
	// Step 2. Show Rect Info
	for each ( RECT rect in vRect ) {
		cout << "{ \"id\": " << rect.id << ", ";
		cout << "\"x\": " << rect.left << ", ";
		cout << "\"y\": " << rect.top << ", ";
		cout << "\"width\": " << rect.right - rect.left << ", ";
		cout << "\"height\": " << rect.bottom - rect.top << " }," << endl;
	}

	// Step 3. Result image
	unsigned char *outpixels = new unsigned char[ width * height * 4 ];
	memset( outpixels, 0, width * height * 4 );
	for each ( RECT rect in vRect ) {
		for ( int j=rect.top; j<=rect.bottom; j++ ) {
			for ( int i=rect.left; i<=rect.right; i += rect.right - rect.left ) {
				outpixels[ (i + j*width )*4 + 0 ] = 255;
				outpixels[ (i + j*width )*4 + 1 ] = 255;
				outpixels[ (i + j*width )*4 + 3 ] = 255;
			}
		}

		for ( int j=rect.top; j<=rect.bottom; j += rect.bottom - rect.top ) {
			for ( int i=rect.left; i<=rect.right; i++ ) {
				outpixels[ (i + j*width )*4 + 0 ] = 255;
				outpixels[ (i + j*width )*4 + 1 ] = 255;
				outpixels[ (i + j*width )*4 + 3 ] = 255;
			}
		}

	}
	int ret = stbi_write_png( outputFilename.c_str(), width, height, 4, outpixels, width*4 );
	delete[] outpixels;
	vRect.clear();

	if ( !ret ) {
		throw ERR_CREATE( "Cannot save image." );
		
	}
}
