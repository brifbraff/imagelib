/*
 * JPGImage.h
 *
 *  Created on: Jul 16, 2016
 *      Author: mk
 */

#ifndef JPGIMAGE_H_
#define JPGIMAGE_H_

#include "Image.h"

class JPGImage: public Image {

public:
	~JPGImage();
	static JPGImage* load(const char* filename);
	static JPGImage* convert(Image* image);
	void save(const char* filename);
	void setQuality(int quality);
	const char* extension();

private:
	JPGImage();
	int image_quality = 100;

protected:
	void calculateDimensions();

};

#endif /* JPGIMAGE_H_ */
