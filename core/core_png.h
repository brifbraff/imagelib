#ifndef __core_png_h__
#define __core_png_h__

typedef unsigned char uchar;
typedef uchar UCOLOR[3];


bool write_png(
    const char* filename, ///< [in] Output file name
    UCOLOR**    data,     ///< [in] Pixel data
    int         width,    ///< [in] Image width
    int         height);  ///< [in] Image height


UCOLOR** read_png(
    const char* filename, ///< [in] Input file name
    int*        width,    ///< [out] Image width
    int*        height);  ///< [out] Image height


#endif // __core_png_h__
