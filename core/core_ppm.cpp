#include <cstdio>
#include <cstdlib>
#include "core_ppm.h"

/**
***************************************************************************************
* writePPM
*
* @brief
*	  Saves the given image in PPM file format. For details of this format see,
*	 http://netpbm.sourceforge.net/doc/ppm.html. The pixel data is assumed
*	 to be a contiguous array of pixels. The first pixel corresponds to the top-left
*	 corner of the image. Each pixel must have three color components stored in
*	 RGB order.
*
* @return
*	  true if successful, false otherwise
**************************************************************************************
*/
bool write_ppm(
	const char*	filename,	///< [in] Output file name
	uchar*		data,		///< [in] Pixel data
	char		mode,		///< [in] PPM image mode: 'A' for ASCII, 'B' for Binary
	int			width,		///< [in] Image width
	int			height)		///< [in] Image height
{
	// Check the sanity of the inputs
	if (!data || width <= 0 || height <= 0 || data == NULL) {
		return false;
	}

	// Open the output file
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		return false;
	}

	// Write the PPM header
	if (mode == 'A') {
		fprintf(fp, "P3\n");		  // magic number
	} else if (mode == 'B') {
		fprintf(fp, "P6\n");		  // magic number
	} else {
		return false;
	}
	fprintf(fp, "%d %d\n", width, height);   // image width and height
	fprintf(fp, "255\n");         // maximum color value

	// Write the scanline data from top to bottom
	uchar* ptr = data;
	if (mode == 'A') {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				uchar* rgb = &ptr[y*width*3+x*3];
				fprintf(fp, "%u %u %u\n", rgb[0], rgb[1], rgb[2]);
				//fprintf(stdout, "%u %u %u\n", rgb[0], rgb[1], rgb[2]);
				//int a;
				//scanf("%d", &a);
			}
		}
	} else {
		size_t scanlineWidth = width * 3;
		for (int y = 0; y < height; ++y) {
			fwrite(ptr, scanlineWidth, 1, fp);
			ptr += scanlineWidth;
		}
	}

	// Close the file stream
	fclose(fp);

	return true;
}


/**
***************************************************************************************
* readPPM
*
* @brief
*	  Reads a given PPM image For details of this format see, 
*	 http://netpbm.sourceforge.net/doc/ppm.html. The pixel data is assumed
*	 to be a contiguous array of pixels. The first pixel corresponds to the top-left
*	 corner of the image. Each pixel must have three color components stored in
*	 RGB order.
*
* @return
*	  the pixel data as a contiguous array of pixels
**************************************************************************************
*/

#include <iostream>
uchar* read_ppm(
	const char*	filename,	///< [in] Output file name
	char*		mode,		///< [out] PPM image mode: 'A' for ASCII, 'B' for Binary
	int*		width,		///< [out] Image width
	int*		height)		///< [out] Image height
{
	// Open the input file
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		return NULL;
	}
	uchar* data;
	
	// Read the PPM header
	int maxValue;
	char magicNumber[4];
	fscanf(fp, "%s", magicNumber);
	if (magicNumber[1] == '3') { // P3
		*mode = 'A';
	} else {
		*mode = 'B';
	}
	fscanf(fp, "%d %d %d", width, height, &maxValue);

	data = (uchar*) malloc((*width)*(*height)*3*sizeof(uchar));
	if (*mode == 'B') {
		// read the trailing newline character
		char eol;
		fscanf(fp, "%c", &eol);
	}
	// Write the scanline data from top to bottom
	uchar* ptr = data;
	if (*mode == 'A') {
		for (int y = 0; y < *height; ++y) {
			for (int x = 0; x < *width; ++x) {
				uchar* rgb = &ptr[y*(*width)*3+x*3];
				fscanf(fp, "%hhu %hhu %hhu", &rgb[0], &rgb[1], &rgb[2]);
			}
		}
	} else if (*mode == 'B') {
		size_t scanlineWidth = (*width) * 3;
		for (int y = 0; y < *height; ++y) {
			fread(ptr, scanlineWidth, 1, fp);
			ptr += scanlineWidth;
		}
	} 

	// Close the file stream
	fclose(fp);
	return data;
}

