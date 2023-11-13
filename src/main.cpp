#include <iostream>

#include "ANSI.h"
#include "Image.hpp"
#include "ffmpeg.hpp"


using namespace std;


// --------------------------------- [ Main Function ] -------------------------------------- //


int main(int argc, char const* const* argv){
	Image img = Image(360,360);
	
	// Red image
	for (int i = 0 ; i < (img.w*img.h) ; i++){
		img[i].r = 255;
	}
	
	int err = ffEncode(img, "bin/img.png", true);
	if (err != 0){
		cerr << ANSI_RED "Failed to encode image using ffmpeg." ANSI_RESET "\n";
		exit(err);
	}
	
	return 0;
}


// ------------------------------------------------------------------------------------------ //
