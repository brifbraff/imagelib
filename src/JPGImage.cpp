/*
 * JPGImage.cpp
 *
 *  Created on: Jul 16, 2016
 *      Author: mk
 */

#include <iostream>
#include <jpeglib.h>
#include <sstream>
#include <cstring>
#include <stdexcept>

#include "../core/core_jpeg.h"
#include "../header/JPGImage.h"

using namespace std;

JPGImage::JPGImage() {
	image_array = NULL;
	name = NULL;
	IMAGE_WIDTH = 0;
	IMAGE_HEIGHT = 0;
}

JPGImage::~JPGImage() {
}

JPGImage* JPGImage::load(const char* filename) {
	JPGImage* jpg_image = new JPGImage();
	jpg_image->name = filename;
	jpg_image->calculateDimensions();
	jpg_image->image_array = read_jpeg(jpg_image->name, &(jpg_image->IMAGE_WIDTH), &(jpg_image->IMAGE_HEIGHT));
	return jpg_image;
}

JPGImage* JPGImage::convert(Image* image) {
	JPGImage* jpg_image = new JPGImage();
	jpg_image->convertImageFromTo(image, jpg_image);
	return jpg_image;
}

void JPGImage::save(const char* filename) {
	if (image_array != NULL) {
		write_jpeg(filename, image_array, IMAGE_WIDTH, IMAGE_HEIGHT, image_quality);
	}
}

void JPGImage::setQuality(int quality) {
	this->image_quality = quality;
}

void JPGImage::calculateDimensions() {
	int x = 0;
	int y = 0;
	FILE *f = fopen(name, "rb");
	if (f == 0)
		throw std::runtime_error("Image dimensions could not be calculated.");
	fseek(f, 0, 2 /*SEEK_END*/);
	long len = ftell(f);
	fseek(f, 0, 0 /*SEEK_SET*/);
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
			fseek(f, pos, 0 /*SEEK_SET*/);
			fread(buf + 2, 1, 12, f);
		}
	}

	fclose(f);

	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF) {
		y = (buf[7] << 8) + buf[8];
		x = (buf[9] << 8) + buf[10];
	}

	IMAGE_WIDTH = x;
	IMAGE_HEIGHT = y;
}

const char* JPGImage::extension() {
	return "jpg";
}
