#include "ImageOpenEXR.hpp"
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

void ImageOpenEXR::ToneMap()
{
    imageToSave = new OpenEXR_pixel[W * H];

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

bool ImageOpenEXR::Save(std::string filename)
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
        // Create an OpenCV Mat object to store the image data
        cv::Mat image(H, W, CV_32FC3);

        // Loop over each pixel in the image and copy to the OpenCV Mat object
        for (int j = 0; j < H; j++)
        {
            for (int i = 0; i < W; ++i)
            {
                image.at<cv::Vec3f>(j, i)[0] = imagePlane[j * W + i].B; // B
                image.at<cv::Vec3f>(j, i)[1] = imagePlane[j * W + i].G; // G
                image.at<cv::Vec3f>(j, i)[2] = imagePlane[j * W + i].R; // R
            }
        }

        // Save the image as an OpenEXR file
        if (!cv::imwrite(filename + ".exr", image))
        {
            std::cerr << "Error saving image to file: " << filename << ".exr" << std::endl;
            return false;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error saving image to file: " << filename << ".exr" << std::endl;
        return false;
    }

    delete[] imageToSave;

    return true;
}

void ImageOpenEXR::Display(std::string filename)
{
    // Load the image
    cv::Mat image = cv::imread(filename + ".exr", cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);

    // Check if the image is loaded
    if (image.empty())
    {
        std::cerr << "Error loading image: " << filename << ".exr" << std::endl;
        return;
    }

    // Display the image
    cv::namedWindow("OpenEXR Image", cv::WINDOW_NORMAL);
    cv::imshow("OpenEXR Image", image);

    // Wait for a key press indefinitely
    cv::waitKey(0);

    // Close the window
    cv::destroyWindow("OpenEXR Image");
}
