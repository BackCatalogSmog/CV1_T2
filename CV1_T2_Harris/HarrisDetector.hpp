#pragma once
#include <vector>

#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"

class HarrisDetector
{
public:
	HarrisDetector();
	HarrisDetector(char* fn);
	~HarrisDetector();
	cv::Mat image;
	cv::Mat imageColour;
	cv::Mat processed;
	//void blurGaussian();
	void applySobel(int x, int y); //Debug method to see if sobel works
	std::vector<double> sobel(int x, int y);
	bool isOutOfBounds(int x, int y);
	void applyHarrisCornerDetection(int window);
	double harrisScore(int x, int y);
};

