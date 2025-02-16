#pragma once

#include <vector>
#include "Pixel.h"

class Frame {

public:
	std::vector<Pixel> frameBuffer;

	Frame(Pixel pixels[], int size);
	Frame(std::vector<Pixel> pixels);
	~Frame() {};

};