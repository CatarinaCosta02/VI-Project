#ifndef ImageOpenEXR_hpp
#define ImageOpenEXR_hpp
#include "image.hpp"

class ImageOpenEXR: public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } OpenEXR_pixel;
    OpenEXR_pixel *imageToSave;
    void ToneMap ();
public:
    ImageOpenEXR(const int W, const int H):Image(W, H) {}
    bool Save (std::string filename);
    void Display(std::string filename);
    bool CheckImagePlane();
};

#endif /* ImagePFM_hpp */
