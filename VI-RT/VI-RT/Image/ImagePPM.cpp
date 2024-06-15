//
//  ImagePPM.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//
// Implements the ToneMap function to convert and clamp pixel values before saving.
// Implements the Save function for saving the PPM image.

#include "ImagePPM.hpp"
#include <iostream>
#include <fstream>
#include <opencv2/highgui.hpp>

void ImagePPM::ToneMap () {
    imageToSave = new PPM_pixel[W*H];
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            imageToSave[j*W+i].val[0] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].R) * 255);
            imageToSave[j*W+i].val[1] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].G) * 255);
            imageToSave[j*W+i].val[2] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].B) * 255);
        }
    }
    // Print a message to indicate that ToneMap is complete
    std::cout << "ToneMapping completed!" << std::endl;
    
}

bool ImagePPM::Save(std::string filename) {
    // convert from float to {0,1,..., 255}
    // Converta para o formato JPEG aqui
        // Converta para o formato PPM aqui
    ToneMap();

    // Open the file for writing
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return false; // Signal failure
    }

    try {
        // Write the PPM header
        file << "P6\n" << W << " " << H << "\n255\n";

        // Write the image data
        unsigned char r, g, b;
        for (int i = 0; i < W * H; ++i) {
            r = static_cast<unsigned char>(std::min(1.f, imagePlane[i].R) * 255);
            g = static_cast<unsigned char>(std::min(1.f, imagePlane[i].G) * 255);
            b = static_cast<unsigned char>(std::min(1.f, imagePlane[i].B) * 255);
            file << r << g << b;
        }
    } catch (const char *err) {
        std::cerr << err << std::endl;
        file.close(); // Close the file in case of an exception
        return false; // Signal failure
    }

    // Close the file
    file.close();

    // Optionally, delete the imageToSave array if it's dynamically allocated
    delete[] imageToSave;

    return true; // Signal success
}


