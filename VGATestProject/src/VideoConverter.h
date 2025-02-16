#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"

#include <vector>
#include <string>

#include "Pixel.h"
#include "Frame.h"
#include "Video.h"
#include "ScreenBuffer.h"

class VideoConverter {

private:
	cv::VideoCapture workingVideo;
	cv::Mat workingImage;

public:
	uint32_t framerate;

	VideoConverter();
	~VideoConverter();

	bool load_image(std::string filepath);
	// bool load_video(std::string filepath);
	// Video convert_video(bool displayed);
	Video convert_video(std::vector<std::string> filepaths);

	Frame to_frame() { return to_frame(workingImage); }
	void display_frame(uint8_t scaleFactor) { display_frame(workingImage, scaleFactor); }

private:
	Frame to_frame(cv::Mat image);
	void display_frame(cv::Mat image, uint8_t scaleFactor);
	void display_image(cv::Mat image);

	cv::Mat fit(cv::Mat image);
	cv::Mat rev_fit(cv::Mat image);
	cv::Mat greyscale(cv::Mat image);

};