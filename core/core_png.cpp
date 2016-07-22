#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <png.h>
#include "core_png.h"


UCOLOR** read_png(
		const char*    filename, ///< [in] Output file name
		int*           width,    ///< [out] Image width
		int*           height)   ///< [out] Image height
{
	// int bit_depth, color_type;
	FILE* fp = fopen(filename, "rb");
	if (!fp)
	{
		return NULL;
	}
	/*
	int number = 8;
	png_byte header[9]; // 8+1
	fread(header, sizeof(png_byte), number, fp);
	bool is_png = !png_sig_cmp(header, 0, number);
	if (!is_png)
	{
		return NULL;
	}
	*/
	png_structp png_ptr = png_create_read_struct(
			PNG_LIBPNG_VER_STRING,
			(png_voidp)NULL, NULL, NULL);
	if (!png_ptr)
	{
		return NULL;
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);

	/*
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return NULL;
	}
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return NULL;
	}
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return NULL;
	}
	*/
	png_init_io(png_ptr, fp);
	/*
	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)width, (png_uint_32*)height,
			&bit_depth, &color_type, NULL, NULL, NULL);
	printf("%d, %d, %d, %d\n", *width, *height, bit_depth, color_type);
	//png_read_image(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	//png_read_end(png_ptr, info_ptr);
	png_byte** row_pointers = (png_byte**)png_malloc(png_ptr, (*height)*png_sizeof(png_byte*));
	for (int i=0; i < *height; i++)
	{
		row_pointers[i] = (png_byte*)png_malloc(png_ptr, (*width)*bit_depth*3);
	}
	*/
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	//*height = info_ptr->height;
	//*width = info_ptr->width;
	//bit_depth = info_ptr->bit_depth;
	//color_type = info_ptr->color_type;
	//printf("%d, %d, %d, %d\n", *width, *height, bit_depth, color_type);
	png_byte** row_pointers = NULL;
	row_pointers = png_get_rows(png_ptr, info_ptr);
	fclose(fp);
	return (UCOLOR**)row_pointers;
}

bool write_png(
    const char* filename, ///< [in] Output file name
    UCOLOR**    data,     ///< [in] Pixel data
    int         width,    ///< [in] Image width
    int         height)   ///< [in] Image height
{
    FILE* fp = fopen(filename, "wb");

    if (!fp)
    {
        return false;
    }

    png_structp png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING,
            (png_voidp)NULL,
            NULL,
            NULL);

    if (!png_ptr)
    {
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);

    //png_set_compression_level(png_ptr, Z_BEST_COMPRESSION); // optional

    png_set_IHDR(png_ptr, info_ptr, width, height, 8,
            PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

    png_byte** row_pointers = (png_byte**) png_malloc(png_ptr, height * sizeof (png_byte *));

    for (int y = 0; y < height; ++y)
    {
        png_byte* row = (png_byte*) png_malloc(png_ptr, sizeof (unsigned char) * width * 3);
        row_pointers[y] = row;

        //unsigned char* dataPtr = data + y*width*3;
        unsigned char* dataPtr = (unsigned char*)data[y];
        memcpy(row, dataPtr, sizeof(unsigned char) * width * 3);
    }

    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    for (int y = 0; y < height; ++y)
    {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);

    fclose(fp);

    return true;
}
