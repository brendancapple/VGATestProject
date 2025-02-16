#include "ScreenController.h"

ScreenController::ScreenController(Video initialVideo)
{
	video = initialVideo;
	for (int s = 0; s < BUFFER_COUNT; s++)
		screenBuffers[s].wipe();

	std::thread keyThread(&ScreenController::key_management, this);
	keyThread.detach();
}

ScreenController::~ScreenController()
{
	exit = true;
	controllable = false;
}

void ScreenController::display(ScreenBuffer* buffer)
{
	// std::cout << (int) workingBuffer << " ";
	buffer->wipe();
	buffer->push(video.peek());

	Pixel pauseGUI = { paused ? (char)254 : '>', 0, 0, paused ? 255 : 0, paused ? 0 : 255, 0 };
	buffer->push(pauseGUI);

	buffer->render();
}

void ScreenController::key_management()
{
	while (controllable) {
		if (GetKeyState(VK_SPACE) & 0x8000) {
			paused = !paused;
		}
		else if (GetKeyState(VK_ESCAPE) & 0x8000) {
			exit = true;
			controllable = false;
		}
	}
}

void ScreenController::play()
{
	int interframePeriod = 1000 / framerate;
	this -> paused = false;

	while (true) {
		std::thread renderThread(&ScreenController::display, this, &screenBuffers[workingBuffer]);
		workingBuffer = (workingBuffer + 1) % BUFFER_COUNT;

		std::this_thread::sleep_for(std::chrono::milliseconds(interframePeriod));
		renderThread.join();
		
		if (paused) {
			std::thread pauseThread(&ScreenController::display, this, &screenBuffers[workingBuffer]);
			workingBuffer = (workingBuffer + 1) % BUFFER_COUNT;
			while (paused && !exit)
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			pauseThread.join();
		}
		if (video.is_end() || exit) break;
		video.advance();
	}

}

void ScreenController::play(uint32_t repeats)
{
	for (uint32_t i = 0; i < repeats; i++) {
		this->restart();
		this->play();

		if (exit) break;
	}
}

void ScreenController::loop() 
{
	while (true) {
		this->restart();
		this->play();

		if (exit) break;
	}
}

void ScreenController::pause()
{
	this->paused = true;
}

void ScreenController::rewind()
{
	int interframePeriod = 1000 / framerate;
	this->paused = false;

	while (!paused) {
		std::thread renderThread(&ScreenController::display, this, &screenBuffers[workingBuffer]); 
		workingBuffer = (workingBuffer + 1) % BUFFER_COUNT; 

		std::this_thread::sleep_for(std::chrono::milliseconds(interframePeriod));
		renderThread.join();

		if (paused) {
			std::thread pauseThread(&ScreenController::display, this, &screenBuffers[workingBuffer]);
			workingBuffer = (workingBuffer + 1) % BUFFER_COUNT;
			while (paused && !exit)
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			pauseThread.join();
		}
		if (video.is_beg() || exit) break;
		video.rewind();
	}
}

void ScreenController::restart()
{
	video.restart();
}

void ScreenController::load(Video videoLoaded)
{
	video = videoLoaded; 
}

bool ScreenController::is_paused()
{
	return this->paused;
}
