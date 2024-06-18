#include "ImageJPG.hpp"
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

void ImageJPG::ToneMap()
{
    imageToSave = new JPG_pixel[W * H];

    // Loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0; j < H; j++)
    {
        for (int i = 0; i < W; ++i)
        {
            imageToSave[j * W + i].val[0] = (unsigned char)(std::min(1.f, imagePlane[j * W + i].R) * 255);
            imageToSave[j * W + i].val[1] = (unsigned char)(std::min(1.f, imagePlane[j * W + i].G) * 255);
            imageToSave[j * W + i].val[2] = (unsigned char)(std::min(1.f, imagePlane[j * W + i].B) * 255);
        }
    }

    std::cout << "ToneMapping completed!" << std::endl;
}

bool ImageJPG::Save(std::string filename)
{

    ToneMap();

    // Open the file for writing
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return false; // Signal failure
    }

    try
    {
        // Create an OpenCV Mat object to store the image data
        // CV_32FC2
        cv::Mat image(H, W, CV_8UC3);

        // Loop over each pixel in the image, clamp and convert to byte format
        // Since openCV uses BGR format, we need to swap the R and B channels to save it in RGB format
        for (int j = 0; j < H; j++)
        {
            for (int i = 0; i < W; ++i)
            {
                image.at<cv::Vec3b>(j, i)[0] = imageToSave[j * W + i].val[2]; // B
                image.at<cv::Vec3b>(j, i)[1] = imageToSave[j * W + i].val[1]; // G
                image.at<cv::Vec3b>(j, i)[2] = imageToSave[j * W + i].val[0]; // R
            }
        }
        // Save the image as a JPEG file
        if (!cv::imwrite(filename + ".jpg", image))
        {
            std::cerr << "Error saving image to file: " << filename << ".jpg" << std::endl;
            return false;
        }
    }
    catch (const cv::Exception &e)
    {
        std::cerr << "OpenCV error: " << e.what() << std::endl;
        return false;
    }

    delete[] imageToSave;

    return true;
}
