/*
 * PPMImage.cpp
 *
 *  Created on: Jul 17, 2016
 *      Author: mk
 */

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstring>
#include <stdexcept>

#include "../core/core_ppm.h"
#include "../header/PPMImage.h"

PPMImage::PPMImage() {
	image_array = NULL;
	ppm_array = NULL;
	name = NULL;
	IMAGE_WIDTH = 0;
	IMAGE_HEIGHT = 0;
	mode = Mode::A;
}

PPMImage::~PPMImage() {
}

void PPMImage::clearWhiteSpace(FILE *in) {
	int c = fgetc(in);
	while (c == '#' || isspace(c)) {
		if (c == '#') {
			while (c != '\n') {
				c = fgetc(in);
			}
		} else {
			while (isspace(c)) {
				c = fgetc(in);
			}
		}
	}
	ungetc(c, in);
}

char PPMImage::getMode() {
	return mode;
}

PPMImage* PPMImage::load(const char* filename) {
	PPMImage* ppm_image = new PPMImage();
	ppm_image->name = filename;
	ppm_image->calculateDimensions();
	char m = ppm_image->mode;
	ppm_image->ppm_array = read_ppm(filename, &(m), &(ppm_image->IMAGE_WIDTH),
			&(ppm_image->IMAGE_HEIGHT));
	ppm_image->convertTo2d();
	return ppm_image;
}

PPMImage* PPMImage::convert(Image* image) {
	PPMImage* ppm_image = new PPMImage();
	ppm_image->convertImageFromTo(image, ppm_image);
	ppm_image->ppm_array = (uchar*) malloc(ppm_image->IMAGE_WIDTH*ppm_image->IMAGE_HEIGHT*3*sizeof(uchar));
	return ppm_image;
}

void PPMImage::convertTo2d() {
	image_array = (UCOLOR**) malloc(sizeof(UCOLOR*) * (IMAGE_HEIGHT));
	for (int i = 0; i < IMAGE_HEIGHT; i++) {
		image_array[i] = (UCOLOR*) malloc(sizeof(UCOLOR) * (IMAGE_WIDTH));
	}

	for (int i = 0; i < IMAGE_HEIGHT; i++) {
		for (int j = 0; j < IMAGE_WIDTH; j++) {
			image_array[i][j][0] = ppm_array[3 * (i * IMAGE_WIDTH + j)];
			image_array[i][j][1] = ppm_array[3 * (i * IMAGE_WIDTH + j) + 1];
			image_array[i][j][2] = ppm_array[3 * (i * IMAGE_WIDTH + j) + 2];

		}
	}
}

void PPMImage::convertTo1d() {
	int k = 0;
	for (int i = 0; i < IMAGE_HEIGHT; i++) {
		for (int j = 0; j < IMAGE_WIDTH; j++) {
			k = 3 * (i * IMAGE_WIDTH + j);
			ppm_array[k + 0] = image_array[i][j][0];
			ppm_array[k + 1] = image_array[i][j][1];
			ppm_array[k + 2] = image_array[i][j][2];
		}
	}
}

void PPMImage::setMode(Mode m) {
	this->mode = m;
}

void PPMImage::save(const char* filename) {
	convertTo1d();
	if (ppm_array != NULL) {
		write_ppm(filename, ppm_array, this->mode, IMAGE_WIDTH, IMAGE_HEIGHT);
	}
}

void PPMImage::calculateDimensions() {
	calculateDimensionsDetermineMode();
}

void PPMImage::calculateDimensionsDetermineMode() {
	FILE* fp = fopen(name, "r");
	if (!fp) {
		throw std::runtime_error("Could not read input file");
	}

	char a = fgetc(fp);
	a = fgetc(fp);

	if (a == '3') {
		this->mode = Mode::A;
	} else {
		this->mode = Mode::B;
	}

	clearWhiteSpace(fp);

	fscanf(fp, "%d ", &IMAGE_WIDTH);
	fscanf(fp, "%d ", &IMAGE_HEIGHT);
}

const char* PPMImage::extension() {
	return "ppm";
}
