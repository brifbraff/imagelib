#ifndef __core_ppm_h__
#define __core_ppm_h__

typedef unsigned char uchar;

/**
***************************************************************************************
* writePPM
*
* @brief
*	  Saves the given image in binary PPM file format. For details of this format
*     see, http://netpbm.sourceforge.net/doc/ppm.html. The pixel data is assumed
*     to be a contiguous array of pixels. The first pixel corresponds to the top-left
*     corner of the image. Each pixel must have three color components stored in
*     RGB order.
*
* @return
*	  true if successful, false otherwise
**************************************************************************************
*/
bool write_ppm(
    const char*    filename, ///< [in] Output file name
    uchar*         udata,    ///< [in] Pixel data
	char           mode,     ///< [in] PPM image mode: 'A' for ASCII, 'B' for Binary
    int            width,    ///< [in] Image width
    int            height);  ///< [in] Image height


/**
***************************************************************************************
* readPPM
*
* @brief
*     Reads a given PPM image For details of this format see, 
*    http://netpbm.sourceforge.net/doc/ppm.html. The pixel data is assumed
*    to be a contiguous array of pixels. The first pixel corresponds to the top-left
*    corner of the image. Each pixel must have three color components stored in
*    RGB order.
*
* @return
*     the pixel data as a contiguous array of pixels
**************************************************************************************
*/
uchar* read_ppm(
    const char*     filename, ///< [in] Output file name
	char*           mode,     ///< [out] PPM image mode: 'A' for ASCII, 'B' for Binary
    int*            width,    ///< [out] Image width
    int*            height);  ///< [out] Image height

#endif // __core_ppm_h__
