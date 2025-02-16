#pragma once

#include<string>
#include "Pixel.h"
#include "Frame.h"

#define SCREEN_HEIGHT 18
#define SCREEN_WIDTH 100

class ScreenBuffer {

private:
	int* head;

public:
	ScreenBuffer();
	~ScreenBuffer();

	void render();
	void wipe();
	void clear();

	void push(Pixel);
	void push(Pixel[], int pixelCount);
	void push(std::vector<Pixel>);
	void push(Frame);
	void push_string(std::string);
	
	static Frame to_frame(std::string);
};