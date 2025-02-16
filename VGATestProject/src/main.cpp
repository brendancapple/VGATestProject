#include<cstdlib>
#include<iostream>
#include<string>
#include<thread>

#include "ScreenBuffer.h"
#include "ScreenController.h"
#include "VideoConverter.h"
#include "Pixel.h"
#include "Frame.h"
#include "Video.h"


int main(int argc, char* argv[])
{
	// Argument Reading
	std::string filepath;
	if (argc > 1)
		filepath = argv[1];
	else {
		filepath = "vid/nyan.cvid";
	}

	Video loadTest; 
	std::cout << "Loading: " << filepath << std::endl; 
	loadTest.load(filepath); 

	/*std::vector<std::string> framepaths = {
		"vid/nyanCat/frame1.png",
		"vid/nyanCat/frame2.png",
		"vid/nyanCat/frame3.png",
		"vid/nyanCat/frame4.png",
		"vid/nyanCat/frame5.png",
		"vid/nyanCat/frame6.png"
	};

	VideoConverter videoConverter; 
	loadTest = videoConverter.convert_video(framepaths); 
	loadTest.save("vid/nyan.cvid");*/
	

	ScreenController mediaPlayer(loadTest);
	mediaPlayer.framerate = 2;
	mediaPlayer.loop();
	// mediaPlayer.play(15);
	// mediaPlayer.play();
	// mediaPlayer.rewind(); 

	return 0;
}