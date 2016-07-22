/*
 * PNGImage.h
 *
 *  Created on: Jul 17, 2016
 *      Author: mk
 */

#ifndef PNGIMAGE_H_
#define PNGIMAGE_H_

#include "Image.h"

class PNGImage: public Image {
public:
	~PNGImage();
	static PNGImage* load(const char* filename);
	static PNGImage* convert(Image* image);
	void save(const char* filename);
	const char* extension();

private:
	PNGImage();

protected:
	void calculateDimensions();

};

#endif /* PNGIMAGE_H_ */
