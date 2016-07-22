/*
 * Image.cpp
 *
 *  Created on: Jul 17, 2016
 *      Author: mk
 */

#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "../header/Image.h"

int Image::getWidth() const {
	return IMAGE_WIDTH;
}

int Image::getHeight() const {
	return IMAGE_HEIGHT;
}

Color* Image::getColorAt(int x, int y) const {
	x = (x < 0) ? 0 : x;
	y = (y < 0) ? 0 : y;

	if (x >= IMAGE_WIDTH) {
		x = IMAGE_WIDTH - 1;
	}

	if (y >= IMAGE_HEIGHT) {
		y = IMAGE_HEIGHT - 1;
	}

	uchar r = image_array[y][x][0];
	uchar g = image_array[y][x][1];
	uchar b = image_array[y][x][2];

	Color* c = new Color(r, g, b);
	return c;
}

void Image::convertImageFromTo(Image* from, Image* to) {
	to->IMAGE_WIDTH = from->getWidth();
	to->IMAGE_HEIGHT = from->getHeight();

	to->image_array = (UCOLOR**) malloc(sizeof(UCOLOR*) * (to->IMAGE_HEIGHT));
	for (int i = 0; i < to->IMAGE_HEIGHT; i++) {
		to->image_array[i] = (UCOLOR*) malloc(
				sizeof(UCOLOR) * (to->IMAGE_WIDTH));
	}

	for (int i = 0; i < to->IMAGE_HEIGHT; i++) {
		for (int j = 0; j < to->IMAGE_WIDTH; j++) {
			Color* c = from->getColorAt(j, i);
			to->image_array[i][j][0] = c->red;
			to->image_array[i][j][1] = c->green;
			to->image_array[i][j][2] = c->blue;
			delete c;
		}
	}
}

void Image::paintRect(Pixel* start_from, int right, int down,
		const Color* color) {
	int x = start_from->x;
	int y = start_from->y;

	if (down <= 0 || right <= 0 || y < 0 || y > IMAGE_HEIGHT || x < 0 || x > IMAGE_WIDTH) {
		throw std::invalid_argument("Impossible to work on these inputs.");
	}

	if (x + right >= IMAGE_WIDTH) {
		right = IMAGE_WIDTH - x - 1;
	}

	if (y + down >= IMAGE_HEIGHT) {
		down = IMAGE_HEIGHT - y - 1;
	}

	for (int i = y; i < y + down; i++) {
		for (int j = x; j < x + right; j++) {
			image_array[i][j][0] = color->red;
			image_array[i][j][1] = color->green;
			image_array[i][j][2] = color->blue;
		}
	}
}

/*
 * This can work much faster if we keep a start and end point
 * and shift them instead of changing cells.
 */
void Image::crop(Pixel* start_from, int right, int down) {
	int x = start_from->x;
	int y = start_from->y;

	if (y < 0 || right < 0 || x < 0 || down < 0) {
		throw std::invalid_argument("Impossible to work on these inputs.");
	}

	if (y + down > IMAGE_HEIGHT) {
		down = IMAGE_HEIGHT - y;
	}
	if (x + right > IMAGE_WIDTH) {
		right = IMAGE_WIDTH - x;
	}

	UCOLOR** cropped_image = (UCOLOR**) malloc(sizeof(UCOLOR*) * (down));
	for (int i = 0; i < down; i++) {
		cropped_image[i] = (UCOLOR*) malloc(sizeof(UCOLOR) * (right));
	}

	for (int h = 0; h < down; h++) {
		for (int w = 0; w < right; w++) {
			cropped_image[h][w][0] = image_array[h + y][w + x][0];
			cropped_image[h][w][1] = image_array[h + y][w + x][1];
			cropped_image[h][w][2] = image_array[h + y][w + x][2];
		}
	}

	size_t size_of_array = sizeof(image_array);
	memset(image_array, 0, size_of_array);
	image_array = cropped_image;

	IMAGE_HEIGHT = down;
	IMAGE_WIDTH = right;
}

void Image::rotate90() {
	int new_height = IMAGE_WIDTH;
	int new_width = IMAGE_HEIGHT;

	UCOLOR** rotated_image = (UCOLOR**) malloc(sizeof(UCOLOR*) * (new_height));
	for (int i = 0; i < new_height; i++) {
		rotated_image[i] = (UCOLOR*) malloc(sizeof(UCOLOR) * (new_width));
	}

	for (int i = 0; i < IMAGE_WIDTH; i++) {
		for (int j = 0; j < IMAGE_HEIGHT; j++) {
			rotated_image[i][j][0] = image_array[IMAGE_HEIGHT - 1 - j][i][0];
			rotated_image[i][j][1] = image_array[IMAGE_HEIGHT - 1 - j][i][1];
			rotated_image[i][j][2] = image_array[IMAGE_HEIGHT - 1 - j][i][2];
		}
	}

	size_t size_of_array = sizeof(image_array);
	memset(image_array, 0, size_of_array);
	image_array = rotated_image;

	IMAGE_HEIGHT = new_height;
	IMAGE_WIDTH = new_width;
}

void Image::resize(int new_width, int new_height) {
	UCOLOR **resized_image = (UCOLOR**) malloc(sizeof(UCOLOR*) * (new_height));
	for (int i = 0; i < new_height; i++) {
		resized_image[i] = (UCOLOR*) malloc(sizeof(UCOLOR) * (new_width));
	}

	for (int i = 0; i < new_height; i++) {
		for (int j = 0; j < new_width; j++) {
			resized_image[i][j][0] = image_array[(int) (i * IMAGE_HEIGHT
					/ new_height)][(int) (j * IMAGE_WIDTH / new_width)][0];
			resized_image[i][j][1] = image_array[(int) (i * IMAGE_HEIGHT
					/ new_height)][(int) (j * IMAGE_WIDTH / new_width)][1];
			resized_image[i][j][2] = image_array[(int) (i * IMAGE_HEIGHT
					/ new_height)][(int) (j * IMAGE_WIDTH / new_width)][2];
		}
	}

	size_t size_of_array = sizeof(image_array);
	memset(image_array, 0, size_of_array);

	image_array = resized_image;
	IMAGE_HEIGHT = new_height;
	IMAGE_WIDTH = new_width;
}

Color* Image::getColorAt(Pixel* p) const {
	uint y = p->y;
	uint x = p->x;
	return getColorAt(x, y);
}
