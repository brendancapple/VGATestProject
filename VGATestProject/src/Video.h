#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include "Frame.h"

class Video {

private:
	int playHead;
	std::vector<Frame> videoBuffer;

	int size();

public: 
	Video();
	Video(std::vector<Frame> frames);
	~Video() {};

	Frame peek();
	Frame get(int frame);
	bool is_end();
	bool is_beg();
	bool advance();
	bool advance(int amount);
	bool rewind();
	bool rewind(int amount);
	void restart();

	void push(Frame frame);

	void load(std::string filepath);
	void save(std::string filepath);
};