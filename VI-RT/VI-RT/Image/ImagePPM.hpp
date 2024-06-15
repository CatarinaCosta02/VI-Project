//
//  ImagePPM.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//
// Inherits from Image, extending its functionality for PPM image specific operations.
// Defines the PPM_pixel structure to represent a pixel in PPM format.
// Includes the ToneMap function for converting and clamping pixel values.
// Declares the Save function specific to PPM format.

#ifndef ImagePPM_hpp
#define ImagePPM_hpp
#include "image.hpp"
//
//  ImagePPM.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//
// Inherits from Image, extending its functionality for PPM image specific operations.
// Defines the PPM_pixel structure to represent a pixel in PPM format.
// Includes the ToneMap function for converting and clamping pixel values.
// Declares the Save function specific to PPM format.

#ifndef ImagePPM_hpp
#define ImagePPM_hpp
#include "image.hpp"

class ImagePPM: public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } PPM_pixel;
    PPM_pixel *imageToSave;
    void ToneMap ();
public:
    ImagePPM(const int W, const int H):Image(W, H) {}
    bool Save (std::string filename);
};

#endif /* ImagePPM_hpp */

class ImagePPM: public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } PPM_pixel;
    PPM_pixel *imageToSave;
    void ToneMap ();
public:
    ImagePPM(const int W, const int H):Image(W, H) {}
    bool Save (std::string filename);
};

#endif /* ImagePPM_hpp */
