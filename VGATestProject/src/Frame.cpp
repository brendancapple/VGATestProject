#include "Frame.h"

Frame::Frame(Pixel pixels[], int size)
{
	for (int i = 0; i < size; i++) {
		frameBuffer.push_back(pixels[i]);
	}
}

Frame::Frame(std::vector<Pixel> pixels)
{
	for (int i = 0; i < pixels.size(); i++) {
		frameBuffer.push_back(pixels[i]);
	}
}
