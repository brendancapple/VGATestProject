#include "VideoConverter.h"

#include <iostream>

VideoConverter::VideoConverter()
{
    framerate = 1; 
}

VideoConverter::~VideoConverter()
{
    workingVideo.release();
}

bool VideoConverter::load_image(std::string filepath)
{
    cv::Mat image;
    image = cv::imread(filepath, cv::IMREAD_COLOR);
    
    if (image.empty()) {
        std::cerr << "Could not open or find the image" << std::endl;
        return false;
    }

    workingImage = image;
    return true;
}

Video VideoConverter::convert_video(std::vector<std::string> filepaths)
{
    std::vector<Frame> frames;
    for (int f = 0; f < filepaths.size(); f++) {
        load_image(filepaths[f]);
        frames.push_back(to_frame());

        std::cout << filepaths[f] << " -> " << frames.back().frameBuffer.size() << " pixels \n";
    }

    return Video(frames);
}

//bool VideoConverter::load_video(std::string filepath)
//{
//    if (!std::FileExists(filepath.c_str()) {
//        std::cout << "File does not exist" << std::endl;
//        return -1;
//    }
//    std::cout << "File Exists" << std::endl;
//
//    cv::VideoCapture bufferedVideo("test.mp4" /*filepath*/);
//    if (!bufferedVideo.isOpened()) {
//        std::cout << "No Video Stream" << std::endl;
//        return false;
//    }
//
//    workingVideo = bufferedVideo;
//    return true;
//}
//
//Video VideoConverter::convert_video(bool displayed)
//{
//    if (displayed) cv::namedWindow("Video Player");
//    framerate = (int) workingVideo.get(cv::CAP_PROP_FPS);
//    std::vector<Frame> frames;
//
//    while (true) {
//        workingVideo >> workingImage;
//        if (workingImage.empty()) break;
//
//        frames.push_back(to_frame());
//
//        if (displayed) {
//            cv::imshow("Video Player", workingImage);
//
//            char c = (char)cv::waitKey(25);
//            if (c == 27) break;
//        }
//    }
//
//    return Video(frames);
//}

Frame VideoConverter::to_frame(cv::Mat image)
{
    cv::Mat fitImage = rev_fit(image);
    std::vector<Pixel> pixels;

    // std::cout << fitImage.rows << " x " << fitImage.cols << std::endl;
    // display_image(fitImage);
    
    for (int i = 0; i < fitImage.rows; i++) {
        for (int j = 0; j < fitImage.cols; j++) {
            cv::Vec3b colorVec = fitImage.at<cv::Vec3b>(i, j);
            Pixel p = { (char)35, i, j, colorVec[2], colorVec[1], colorVec[0] };
            pixels.push_back(p);

            // std::cout << "i" << i << " x j" << j << " -> " << (int) colorVec[2] << " " << (int) colorVec[1] << " " << (int) colorVec[0] << std::endl;
        }
    }

    Frame output(pixels);
    return output; 
}

void VideoConverter::display_frame(cv::Mat image, uint8_t scaleFactor)
{
    cv::Mat fitImage = fit(image);
    cv::Mat scaledImage;
    cv::resize(fitImage, scaledImage, cv::Size(fitImage.rows * scaleFactor, fitImage.cols * scaleFactor), cv::INTER_NEAREST_EXACT);

    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display window", scaledImage);
    cv::waitKey(0);
}

void VideoConverter::display_image(cv::Mat image)
{
    cv::Mat scaledImage;
    cv::resize(image, scaledImage, cv::Size(image.rows * 1, image.cols * 1), cv::INTER_NEAREST_EXACT); 

    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display window", scaledImage);
    cv::waitKey(0);
}

cv::Mat VideoConverter::fit(cv::Mat image)
{
    float heightScale = SCREEN_HEIGHT / (float) image.rows;
    float widthScale = SCREEN_WIDTH / (float) image.cols;
    float scaleFactor = heightScale < widthScale ? heightScale : widthScale;

    uint32_t heightTarget = image.rows * scaleFactor;
    uint32_t widthTarget = image.cols * scaleFactor;

    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(heightTarget, widthTarget * 2), cv::INTER_LINEAR);

    // std::cout << SCREEN_HEIGHT << " / " << image.rows << " = " << heightScale << std::endl;
    // std::cout << SCREEN_WIDTH << " / " << image.cols << " = " << widthScale << std::endl;
    // std::cout << scaleFactor << " -> " << heightTarget << "x" << widthTarget * 2 << std::endl;
    // std::cout << "final: " << resizedImage.rows << " x " << resizedImage.cols << std::endl;

    return resizedImage;
}

cv::Mat VideoConverter::rev_fit(cv::Mat image)
{
    float heightScale = SCREEN_HEIGHT / (float)image.rows;
    float widthScale = SCREEN_WIDTH / (float)image.cols;
    float scaleFactor = heightScale < widthScale ? heightScale : widthScale;

    uint32_t heightTarget = image.rows * scaleFactor;
    uint32_t widthTarget = image.cols * scaleFactor;

    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(widthTarget * 2, heightTarget), cv::INTER_LINEAR);

    // std::cout << SCREEN_HEIGHT << " / " << image.rows << " = " << heightScale << std::endl;
    // std::cout << SCREEN_WIDTH << " / " << image.cols << " = " << widthScale << std::endl;
    // std::cout << scaleFactor << " -> " << heightTarget << "x" << widthTarget * 2 << std::endl;
    // std::cout << "final: " << resizedImage.rows << " x " << resizedImage.cols << std::endl;

    return resizedImage;
}

cv::Mat VideoConverter::greyscale(cv::Mat image)
{
    cv::Mat outImage(image.rows, image.cols, CV_8UC1);
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
            uint8_t luminance = (pixel[0] + pixel[1] + pixel[2]) / 3;
            outImage.at<uchar>(i, j) = luminance;
        }
    }
    return outImage;
}
