#include "ImagePFM.hpp"
#include <iostream>
#include <fstream>
#include <iomanip> // std::setprecision


bool ImagePFM::Save(std::string filename) {
    imageToSave = new PFM_pixel[W*H];
    // Open the file for writing
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return false; // Signal failure
    }

    try {
        // Write the PFM header
        file << "PF\n";
        file << W << " " << H << "\n";
        file << "-1.0\n"; // Indicates little-endian float format

        // Write the image data
        
        for (int j = H - 1; j >= 0; --j) {  // Iterate from last row to first row
            for (int i = 0; i < W; ++i) {
                RGB& color = imagePlane[j * W + i];
                file.write(reinterpret_cast<char*>(&color.R), sizeof(float));
                file.write(reinterpret_cast<char*>(&color.G), sizeof(float));
                file.write(reinterpret_cast<char*>(&color.B), sizeof(float));
    }
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

