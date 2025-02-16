#pragma once

#include "ScreenBuffer.h"
#include "Video.h"

#include <Windows.h>
#include <thread>
#include <chrono>

#define BUFFER_COUNT 2
#define DEFAULT_FRAMERATE 1

class ScreenController {

private:
	ScreenBuffer screenBuffers[2];
	uint8_t workingBuffer = 0;
	bool paused = true;
	bool controllable = true;
	bool exit = false;

	void display(ScreenBuffer* buffer);
	void key_management();

public:
	Video video;
	uint8_t framerate = DEFAULT_FRAMERATE;

	ScreenController(Video initialVideo);
	~ScreenController();

	void play();
	void play(uint32_t repeats);
	void loop();
	void pause();
	void rewind();
	void restart();
	void load(Video videoLoaded);

	bool is_paused();

};