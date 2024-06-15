#ifndef ImageJPG_hpp
#define ImageJPG_hpp
#include "image.hpp"

class ImageJPG: public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } JPG_pixel;
    JPG_pixel *imageToSave;
    void ToneMap ();
public:
    ImageJPG(const int W, const int H):Image(W, H) {}
    bool Save (std::string filename);
};

#endif /* ImageJPG_hpp */
