#include <iostream>
#include <string>
#include <stdlib.h>
#include "PNGImage.h"
#include "PPMImage.h"
#include "JPGImage.h"

using namespace std;

enum Type {
	UNKNOWN, PNG, JPG, PPM
};

Image* i = nullptr;
Type t = UNKNOWN;

void println(std::string m);
void print(std::string m);
void listOptions();
void selectOperation();
Type getType(std::string path);
void loadImage();
void saveImage();
void getDimensions();
void getColorAt();
void paintRect();
void crop();
void rotate90();
void resize();
void convert();
void printColor(Color* c);
Pixel* getCoordinates();
int getWidth();
int getHeight();
Color* getColorCode();
std::string getDesktopPath();

std::string getDesktopPath() {
	std::string home = getenv("HOME");
	if (!home.empty())
		return (home+"/");
	else
		return "";
}

void println(std::string m) {
	cout << m << endl;
}

void print(std::string m) {
	cout << m;
}

void listOptions() {
	println("----------------------------------");
	println("Available operations:");
	println(
			"\t1- save image         2- get dimensions    3- get color at pixel");
	println("\t4- paint rectangle    5- crop              6- rotate90");
	println("\t7- resize             8- convert           9- exit");
}

Type getType(std::string path) {
	size_t lastindex = path.find_last_of(".");
	string ext = path.substr(lastindex + 1, path.length());
	if (ext == "png")
		return Type::PNG;
	else if (ext == "jpg")
		return Type::JPG;
	else if (ext == "ppm")
		return Type::PPM;
	else {
		print("File type not supported: ");
		print(ext);
		println("");
	}
	return UNKNOWN;
}

void loadImage() {
	print("Enter image path: ");
	std::string desktop = getDesktopPath();
	print(desktop);
	std::string path;
	cin >> path;
	path = desktop+path;
	t = getType(path);
	if (t == UNKNOWN) {
		println("ImageLib stopped.");
		exit(0);
	}

	if (t == PNG) {
		i = PNGImage::load(path.c_str());
	} else if (t == JPG) {
		i = JPGImage::load(path.c_str());
	} else if (t == PPM) {
		i = PPMImage::load(path.c_str());
	}

	println("Image loaded.");
}

void printColor(Color* c) {
	cout << "c-> r:" << (int) c->red << " g:" << (int) c->green << " b:"
			<< (int) c->blue << endl;
}

Image* loadPNG(const char* path) {
	cout << "image loaded: " << path << endl;
	return PNGImage::load(path);
}

Image* loadJPG(const char* path) {
	cout << "image loaded: " << path << endl;
	return JPGImage::load(path);
}

Image* loadPPM(const char* path) {
	cout << "image loaded: " << path << endl;
	return PPMImage::load(path);
}

void printColorAt(Image* i, Pixel* p) {
	Color* c = i->getColorAt(p->x, p->y);
	printColor(c);
	cout << "painted color => " << "c-> r:" << (int) c->red << " g:"
			<< (int) c->green << " b:" << (int) c->blue << endl;
	delete c;
	delete p;
}

Color* getColorCode() {
	uint r, g, b;
	cout << "color red: ";
	cin >> r;
	cout << "color green: ";
	cin >> g;
	cout << "color blue: ";
	cin >> b;
	return new Color(r, g, b);
}

void selectOperation() {
	listOptions();
	int op;
	bool error = false;
	do {
		print("Select operation: ");
		cin >> op;

		switch (op) {
		case 1:
			saveImage();
			break;
		case 2:
			getDimensions();
			break;
		case 3:
			getColorAt();
			break;
		case 4:
			paintRect();
			break;
		case 5:
			crop();
			break;
		case 6:
			rotate90();
			break;
		case 7:
			resize();
			break;
		case 8:
			convert();
			break;
		case 9:
			println("good-bye...");
			exit(0);
			break;
		default:
			error = true;
		}
	} while (error);
}

int getWidth() {
	int w;
	print("Width: ");
	cin >> w;
	return w;
}

int getHeight() {
	int h;
	print("Height: ");
	cin >> h;
	return h;
}

Pixel* getCoordinates() {
	int x, y;
	println("Coordinates:");
	print("x: ");
	cin >> x;
	print("y: ");
	cin >> y;
	Pixel* p = new Pixel(x, y);
	return p;
}

void saveImage() {
	std::string save_to;
	print("Path to save: ");
	cin >> save_to;
	i->save(save_to.c_str());
	print("Image saved @ ");
	print(save_to);
	println("");
	selectOperation();
}

void getDimensions() {
	cout << "w :" << i->getWidth() << endl;
	cout << "h :" << i->getHeight() << endl;
	selectOperation();
}

void getColorAt() {
	Pixel* p = getCoordinates();
	Color* c = i->getColorAt(p->x, p->y);
	printColor(c);
	delete c;
	delete p;
	selectOperation();
}

void paintRect() {
	Pixel* p = getCoordinates();
	int w = getWidth();
	int h = getHeight();
	Color* c = getColorCode();
	i->paintRect(p, w, h, c);
	cout << "a rectangle painted => " << "c-> r:" << (int) c->red << " g:"
			<< (int) c->green << " b:" << (int) c->blue << endl;
	delete c;
	delete p;
	selectOperation();
}

void crop() {
	Pixel* p = getCoordinates();
	int w = getWidth();
	int h = getHeight();
	i->crop(p, w, h);
	println("cropped a part of the image");
	delete p;
	selectOperation();
}

void rotate90() {
	i->rotate90();
	println("image rotated");
	selectOperation();
}

void resize() {
	int x = getWidth();
	int y = getHeight();
	i->resize(x, y);
	cout << "image resized to: " << x << "x" << y << endl;
	selectOperation();
}

void convert() {
	cout << "current format is: " << t << endl;
	cout << "select new format: 1-png, 2-jpg, 3-ppm: ";
	int selection;
	cin >> selection;
	if (selection == 1) {
		i = PNGImage::convert(i);
		t = Type::PNG;
	} else if (selection == 2) {
		i = JPGImage::convert(i);
		t = Type::JPG;
	} else if (selection == 3) {
		i = PPMImage::convert(i);
		t = Type::PPM;
	}
	println("Image format is changed successfully");
	selectOperation();
}

/*
 * Entry Point
 */
int main() {
	println("ImageLib started. Supports png, jpg and ppm formats");
	loadImage();
	selectOperation();
	return 0;
}

