#include "ScreenBuffer.h"

#include<iostream>
#include<vector>

ScreenBuffer::ScreenBuffer() 
{
	head = (int*) malloc(sizeof(int) * SCREEN_HEIGHT * SCREEN_WIDTH * 2);
	// this->clear();
}

ScreenBuffer::~ScreenBuffer()
{
	this->clear();
	free(head);
}

void ScreenBuffer::render() 
{
	std::cout << "\033[2J \033[3J" << std::endl;
	int row = 0;
	int col = 0;

	for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
		if (row >= SCREEN_HEIGHT) break;
		if (col >= SCREEN_WIDTH) {
			row++;
			col = 0;
			std::cout << std::endl;
		}

		if (*(head + i * 2) == (int)'\n') continue;

		col++;
		int colorInt = *(head + i * 2 + 1);
		std::string colorStr = "\033[38;2;" + std::to_string((colorInt >> 16) & 255) + ";"
			+ std::to_string((colorInt >> 8) & 255) + ";"
			+ std::to_string(colorInt & 255) + "m";
		std::cout << colorStr << (char)*(head + i * 2);
	}
}

void ScreenBuffer::wipe()
{
	for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
		*(head + i * 2 + 1) = (255 << 16) + (255 << 8) + 255;
		*(head + i * 2) = (int)' ';
	}
}

void ScreenBuffer::clear()
{
	for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH * 2; i++) {
		*(head + i) = 0;
	}
}


void ScreenBuffer::push(Pixel pixel)
{
	int color = (pixel.r << 16) + (pixel.g << 8) + pixel.b; // r, g, b
	int offset = (pixel.row * SCREEN_WIDTH + pixel.col) * 2;

	*(head + offset + 1) = color;
	*(head + offset) = (int)pixel.sym;
}

void ScreenBuffer::push(Pixel pixels[], int pixelCount)
{
	for (int i = 0; i < pixelCount; i++) {
		this->push(pixels[i]);
	}
}

void ScreenBuffer::push(std::vector<Pixel> pixels)
{
	for (int i = 0; i < pixels.size(); i++) {
		this->push(pixels[i]);
	}
}

void ScreenBuffer::push(Frame frame)
{
	this->push(frame.frameBuffer);
}

void ScreenBuffer::push_string(std::string string)
{
	int offset = 0;
	for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
		int color = (155 << 16) + (0 << 8) + 255; // r, g, b

		if (string[i + offset] == '\n') {
			offset++;
			i--;
			continue;
		}

		if (string.length() <= i) break;
		*(head + i * 2 + 1) = color;
		*(head + i * 2) = (int) string[i + offset];
	}
}

Frame ScreenBuffer::to_frame(std::string string)
{
	std::vector<Pixel> pixels;
	int offset = 0;
	for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
		if (string.length() <= i) break;
		if (string[i + offset] == ' ') continue;

		if (string[i + offset] == '\n') {
			offset++;
			i--;
			continue;
		}

		int row = (i + offset) / SCREEN_WIDTH;
		int col = (i + offset) % SCREEN_WIDTH;
		
		Pixel p = { string[i + offset], row, col, 255, 255, 255 };
		pixels.push_back(p);
	}

	Frame output(pixels);
	return output;
}
