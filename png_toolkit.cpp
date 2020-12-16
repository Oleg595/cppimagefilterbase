#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <array>
#include <vector>
#include "stb_image_write.h"
#include "png_toolkit.h"

png_toolkit::png_toolkit()
{
}

png_toolkit::~png_toolkit()
{
    stbi_image_free(imgData.pixels);
}

bool png_toolkit::load( const std::string &pictureName )
{
    imgData.pixels = stbi_load(pictureName.c_str(), &imgData.w, &imgData.h, &imgData.compPerPixel, 0);
    return imgData.pixels != nullptr;
}

bool png_toolkit::save( const std::string &pictureName )
{
    return stbi_write_png(pictureName.c_str(),
                   imgData.w, imgData.h,
                   imgData.compPerPixel,
                          imgData.pixels, 0) != 0;
}


image_data png_toolkit::getPixelData( void ) const
{
    return imgData;
}

void png_toolkit::Black_White() {
	stbi_uc x;
	for (int i = 0; i < imgData.w; i++) {
		for (int j = 0; j < imgData.h; j++) {
			x = (3 * imgData.pixels[i * imgData.h + imgData.compPerPixel * j] + 6 * imgData.pixels[i * imgData.h + imgData.compPerPixel * j + 1] + 1 * imgData.pixels[i * imgData.h + imgData.compPerPixel * j + 2]) / 10;
			imgData.pixels[i * imgData.h + imgData.compPerPixel * j] = x;
			imgData.pixels[i * imgData.h + imgData.compPerPixel * j + 1] = x;
			imgData.pixels[i * imgData.h + imgData.compPerPixel * j + 2] = x;
		}
	}
}

void png_toolkit::Red() {
	for (int i = 0; i < imgData.w; i++) {
		for (int j = 0; j < imgData.h; j++) {
			imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j] = 255;
			imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 1] = 0;
			imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 2] = 0;
		}
	}
}

int Max(int i) {
	if (i - 2 >= 0) {
		return i - 2;
	}
	else {
		if (i - 1 >= 0) {
			return i - 1;
		}
		else {
			return i;
		}
	}
}

stbi_uc png_toolkit::Matrix5(int i, int j) {
	int x = 0;
	int n = 0;
	std::vector<stbi_uc> arr;
	for (int i1 = Max(i); i1 < i + 3 && i1 < imgData.w; i1++) {
		for (int j1 = Max(j); j1 < j + 3 && j1 < imgData.h; j1++) {
			n++;
			arr.push_back((int)imgData.pixels[imgData.compPerPixel * i1 * imgData.h + imgData.compPerPixel * j1 + 1]);
		}
	}
	std::sort(arr.begin(), arr.end());
	if (n % 2 == 0) {
		return (stbi_uc)arr[n / 2 - 1];
	}
	else {
		return (stbi_uc)arr[n / 2];
	}
}

void png_toolkit::Threshold() {
	Black_White();
	stbi_uc x;
	for (int i = 0; i < imgData.w; i++) {
		for (int j = 0; j < imgData.h; j++) {
			x = Matrix5(i, j);
			if (imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j] < x) {
				imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j] = 0;
			}
		}
	}
	for (int i = 0; i < imgData.w; i++) {
		for (int j = 0; j < imgData.h; j++) {
			if (imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j] == 0) {
				imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 1] = 0;
				imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 2] = 0;
			}
		}
	}
}

stbi_uc png_toolkit::Matrix3_Edge(int i, int j) {
	int x = 0;
	int n = 0;
	for (int i1 = (i > 0) ? (i - 1) : 0; i1 < i + 2 && i1 < imgData.w; i1++) {
		for (int j1 = (j > 0) ? (j - 1) : 0; j1 < j + 2 && j1 < imgData.h; j1++) {
			n++;
			x += (int)imgData.pixels[imgData.compPerPixel * i1 * imgData.h + imgData.compPerPixel * j1 + 2];
		}
	}
	x = (int)imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 2] * 10 - x;
	x = x / (10 - n);
	if (x < 0) {
		return (stbi_uc)0;
	}
	if (x > 255) {
		return (stbi_uc)255;
	}
	return (stbi_uc)x;
}

void png_toolkit::Edge() {
	Black_White();
	stbi_uc x;
	for (int i = 0; i < imgData.w; i++) {
		for (int j = 0; j < imgData.h; j++) {
			x = Matrix3_Edge(i, j);
			imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j] = x;
			//imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 1] = x;
			//imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 2] = x;
		}
	}
	for (int i = 0; i < imgData.w; i++) {
		for (int j = 0; j < imgData.h; j++) {
			//x = Matrix3_Edge(i, j);
			//imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j] = x;
			imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 1] = imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j];
			imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 2] = imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j];
		}
	}
}

stbi_uc png_toolkit::Matrix3_Blur(int i, int j, int s) {
	int x = 0;
	int n = 0;
	for (int i1 = (i > 0) ? i - 1 : 0; i1 < i + 2 && i1 < imgData.w; i1++) {
		for (int j1 = (j > 0) ? j - 1 : 0; j1 < j + 2 && j1 < imgData.h; j1++) {
			n++;
			x += imgData.pixels[imgData.compPerPixel * i1 * imgData.h + imgData.compPerPixel * j1 + s];
		}
	}
	x /= n;
	if (x < 0) {
		return 0;
	}
	if (x > 255) {
		return 255;
	}
	return x;
}

void png_toolkit::Blur() {
	stbi_uc* pix = new stbi_uc[imgData.w * imgData.h * imgData.compPerPixel];
	for (int i = 0; i < imgData.w; i++) {
		for (int j = 0; j < imgData.h; j++) {
			pix[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j] = Matrix3_Blur(i, j, 0);
			pix[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 1] = Matrix3_Blur(i, j, 1);
			pix[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 2] = Matrix3_Blur(i, j, 2);
		}
	}
	for (int i = 0; i < imgData.w; i++) {
		for (int j = 0; j < imgData.h; j++) {
			imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j] = pix[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j];
			imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 1] = pix[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 1];
			imgData.pixels[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 2] = pix[imgData.compPerPixel * i * imgData.h + imgData.compPerPixel * j + 2];
		}
	}
}