#ifndef __core_jpeg_h__
#define __core_jpeg_h__

typedef double COLOR[3];
typedef unsigned char uchar;
typedef uchar UCOLOR[3];


void write_jpeg(
		const char* filename, ///< [in] Output file name
		UCOLOR**    image,    ///< [in] Pixel data
		int         width,    ///< [in] Image width
		int         height,   ///< [in] Image height
		int         quality); ///< [in] Image quality [0-100]


UCOLOR** read_jpeg(
		const char* filename, ///< [in] Input file name
		int*        width,    ///< [out] Image width
		int*        height);  ///< [out] Image width


#endif
