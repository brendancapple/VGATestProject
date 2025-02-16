#include "Video.h"
#include <iostream>

Video::Video()
{
	playHead = 0;
}

Video::Video(std::vector<Frame> frames)
{
	playHead = 0;
	for (int i = 0; i < frames.size(); i++) {
		videoBuffer.push_back(frames[i]);
	}
}

Frame Video::peek()
{
	return videoBuffer[playHead];
}

Frame Video::get(int frame)
{
	if (frame >= videoBuffer.size()) frame = (int) videoBuffer.size() - 1;
	if (frame < 0) frame = 0;
	return videoBuffer[frame];
}

bool Video::is_end()
{
	return playHead >= videoBuffer.size()-1;
}

bool Video::is_beg()
{
	return playHead <= 0;
}

bool Video::advance()
{
	playHead++;
	if (playHead >= videoBuffer.size()) playHead = 0;
	return !(playHead >= videoBuffer.size());
}

bool Video::advance(int amount)
{
	playHead += amount;
	if (playHead >= videoBuffer.size()) playHead = (int) videoBuffer.size() - 1;
	return !(playHead >= videoBuffer.size());
}

bool Video::rewind()
{
	playHead--;
	if (playHead < 0) playHead = (int) videoBuffer.size() - 1;
	return !(playHead < 0);
}

bool Video::rewind(int amount)
{
	playHead -= amount;
	if (playHead < 0) playHead = 0;
	return !(playHead < 0);
}

void Video::restart()
{
	playHead = 0;
}

void Video::push(Frame frame)
{
	videoBuffer.push_back(frame);
}

int Video::size()
{
	int output = 0;
	for (int f = 0; f < videoBuffer.size(); f++) {
		Frame frame = videoBuffer[f];
		output += (int) frame.frameBuffer.size() * BYTES_PER_PIXEL;
		output += BYTES_PER_PIXEL;
	}

	return output;
}

void Video::load(std::string filepath)
{
	// File Handling
	std::fstream file(filepath, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		std::cout << "File DNE" << std::endl;
		return;
	}

	uint32_t frameCount;
	file.read((char*)&frameCount, sizeof(uint32_t));
	// std::cout << frameCount << "\n"

	std::vector<Frame> frames;
	for (int f = 0; f < frameCount; f++) {
		uint32_t pixelCount;
		file.read((char*)&pixelCount, sizeof(uint32_t));
		// std::cout << pixelCount << "\n";

		std::vector<Pixel> pixels;
		for (int p = 0; p < pixelCount; p++) {
			Pixel pixel;
			file.read((char*)&pixel, sizeof(Pixel));
			pixels.push_back(pixel);

			// std::cout << file.tellg() << "   " << file.good() << file.bad() << file.fail() << file.eof() << "\n";
			// std::cout << "    " << p << ": " << pixel.sym << " " << pixel.row << " " << pixel.col << " " << pixel.r << " " << pixel.g << " " << pixel.b << "\n";
		}
		Frame frame(pixels);
		frames.push_back(frame);
	}

	this->videoBuffer = frames;
	this->playHead = 0;

	if (!file.good()) std::cout << "Load Failure" << std::endl;
}

void Video::save(std::string filepath)
{
	std::ofstream file;
	file.open(filepath, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!file.is_open()) return;

	uint32_t videoBufferSize = videoBuffer.size();
	file.write((char*) &videoBufferSize, sizeof(uint32_t));
	for (int f = 0; f < videoBuffer.size(); f++) {
		Frame frame = videoBuffer[f];

		uint32_t frameBufferSize = frame.frameBuffer.size();
		file.write((char*) &frameBufferSize, sizeof(uint32_t));
		for (int p = 0; p < frameBufferSize; p++) {
			Pixel pixel = frame.frameBuffer[p];
			// std::cout << "    " << p << ": " << pixel.sym << " " << pixel.row << " " << pixel.col << " " << pixel.r << " " << pixel.g << " " << pixel.b << "\n";
			
			file.write((char*) &frame.frameBuffer[p], sizeof(Pixel));
		}
	}

	file.close();
	if (!file.good()) std::cout << "Save Failure" << std::endl;
}


