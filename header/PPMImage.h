/*
 * PPMImage.h
 *
 *  Created on: Jul 17, 2016
 *      Author: mk
 */

#ifndef PPMIMAGE_H_
#define PPMIMAGE_H_

#include "Image.h"

enum Mode {
	A = 'A', B = 'B'
};

class PPMImage: public Image {
public:
	~PPMImage();
	static PPMImage* load(const char* filename);
	static PPMImage* convert(Image* image);
	void save(const char* filename);
	void setMode(Mode mode);
	const char* extension();

private:
	uchar* ppm_array;
	Mode mode;

	PPMImage();
	void clearWhiteSpace(FILE *in);
	char getMode();
	void calculateDimensionsDetermineMode();
	void convertTo2d();
	void convertTo1d();


protected:
	void calculateDimensions();

};

#endif /* PPMIMAGE_H_ */
