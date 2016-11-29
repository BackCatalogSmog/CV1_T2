#include <iostream>
#include "HarrisDetector.hpp"
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"


#include "HarrisDetector.hpp"


int main(int argc, char* argv[])
{
	char* file;
	//std::cout << argv[0];
	if (argc < 2) {
		file = "test.png";
	}
	else {
		file = argv[1];
	}
	cv::namedWindow("image", 0);
	HarrisDetector harris(file);

	//read image, grayscale

	//calculate
	double k = 0.04; //Weight suggested by Harris




	return 0;
}
