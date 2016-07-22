/*
 * Image.h
 *
 *  Created on: Jul 16, 2016
 *      Author: mk
 */

#ifndef IMAGE_H_
#define IMAGE_H_

typedef unsigned char uchar;
typedef uchar UCOLOR[3];

struct Pixel {
	Pixel(int x, int y) : x(x), y(y) {}
	~Pixel() {};
	int x;
	int y;
};

struct Color {
	Color(uchar r, uchar g, uchar b) : red(r), green(g), blue(b) {}
	~Color() {};
	uchar red;
	uchar green;
	uchar blue;
};

class Image {
public:
	virtual ~Image() {};

	virtual void save(const char* filename)=0;
	int getWidth() const;
	int getHeight() const;
	Color* getColorAt(int x, int y) const;
	Color* getColorAt(Pixel* p) const;
	void paintRect(Pixel* start_from, int right, int down, const Color* color);
	void crop(Pixel* start_from, int right, int down);
	void rotate90();
	void resize(int new_width, int new_height);
	virtual const char* extension() = 0;

protected:
	UCOLOR** image_array;
	const char* name;
	int IMAGE_WIDTH;
	int IMAGE_HEIGHT;

	virtual void calculateDimensions() = 0;
	void convertImageFromTo(Image* from, Image* to);
};

#endif /* IMAGE_H_ */
