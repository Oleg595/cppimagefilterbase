#ifndef PNG_TOOLKIT_H
#define PNG_TOOLKIT_H

#include <string>
#include <map>
//#include <memory>
#include "stb_image.h"

struct image_data
{
    stbi_uc *pixels;
    int w, h;
    int compPerPixel;
};

class png_toolkit
{
public:
    enum class Error {
        WrongSize,
        WrongFormat,
        Ok
    };

    png_toolkit();
    ~png_toolkit();
    bool load( std::string const &pictureName );
    bool save( std::string const &pictureName );
    image_data getPixelData( void ) const;
    void Black_White();
    void Red();
    void Threshold();
    void Edge();
    void Blur();
    stbi_uc Matrix5(int i, int j);
    stbi_uc Matrix3_Edge(int i, int j);
    stbi_uc Matrix3_Blur(int i, int j, int s);

private:
    image_data imgData;
};

#endif // PNG_TOOLKIT_H
