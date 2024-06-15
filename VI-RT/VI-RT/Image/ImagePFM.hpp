#ifndef ImageJPG_hpp
#define ImageJPG_hpp
#include "image.hpp"

class ImageJPG: public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } PPM_pixel;
    PPM_pixel *imageToSave;
    void ToneMap ();
public:
    ImagePPM(const int W, const int H):Image(W, H) {}
    bool Save (std::string filename);
};

#endif /* ImageJPG_hpp */
