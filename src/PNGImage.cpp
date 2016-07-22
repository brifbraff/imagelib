/*
 * PNGImage.cpp
 *
 *  Created on: Jul 17, 2016
 *      Author: mk
 */

#include <iostream>
#include <png.h>
#include <sstream>
#include <cstring>
#include <stdexcept>

#include "../core/core_png.h"
#include "../header/PNGImage.h"

using namespace std;

PNGImage::PNGImage() {
	image_array = NULL;
	name = NULL;
	IMAGE_WIDTH = 0;
	IMAGE_HEIGHT = 0;
}

PNGImage::~PNGImage() {
}

PNGImage* PNGImage::load(const char* filename) {
	PNGImage* png_image = new PNGImage();
	png_image->name = filename;
	png_image->calculateDimensions();
	png_image->image_array = read_png(png_image->name,
			&(png_image->IMAGE_WIDTH), &(png_image->IMAGE_HEIGHT));
	return png_image;
}

PNGImage* PNGImage::convert(Image* image) {
	PNGImage* png_image = new PNGImage();
	png_image->convertImageFromTo(image, png_image);
	return png_image;
}

void PNGImage::save(const char* filename) {
	if (image_array != NULL) {
		write_png(filename, image_array, IMAGE_WIDTH, IMAGE_HEIGHT);
	}
}

void PNGImage::calculateDimensions() {
	int x = 0;
	int y = 0;
	FILE *f = fopen(name, "rb");
	if (f == 0)
		throw std::runtime_error("Image dimensions could not be calculated.");
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (len < 24) {
		fclose(f);
		throw std::runtime_error("Something bad happened here.");
	}

	unsigned char buf[24];
	fread(buf, 1, 24, f);

	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0
			&& buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I'
			&& buf[9] == 'F') {
		long pos = 2;
		while (buf[2] == 0xFF) {
			if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2
					|| buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA
					|| buf[3] == 0xCB)
				break;
			pos += 2 + (buf[4] << 8) + buf[5];
			if (pos + 12 > len)
				break;
			fseek(f, pos, SEEK_SET);
			fread(buf + 2, 1, 12, f);
		}
	}

	fclose(f);

	if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G'
			&& buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A
			&& buf[7] == 0x0A && buf[12] == 'I' && buf[13] == 'H'
			&& buf[14] == 'D' && buf[15] == 'R') {
		x = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
		y = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);
	}

	IMAGE_WIDTH = x;
	IMAGE_HEIGHT = y;
}

const char* PNGImage::extension() {
	return "png";
}
