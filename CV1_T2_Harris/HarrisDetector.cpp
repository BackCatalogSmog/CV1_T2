#include "HarrisDetector.hpp"
#include <iostream>


HarrisDetector::HarrisDetector()
{
}

HarrisDetector::HarrisDetector(char* fn)
{
	imageColour = cv::imread(fn, 1);
	image = cv::imread(fn,0);
	processed = image.clone();
	//imshow("image", image);
	//cv::waitKey();
	//applySobel(5,5);
	applyHarrisCornerDetection(1);
	cv::imshow("image", imageColour);
	cv::imwrite("testResult.jpg",imageColour);
	cv::waitKey(0);
}


HarrisDetector::~HarrisDetector()
{
}

//apply gaussian blur to variable image
/*void HarrisDetector::blurGaussian()
{
	for (int xi = 0; xi < image.cols; xi++) {
		for (int yi = 0; yi < image.rows; yi++) {
			//get neighbourhood
			uchar neigh[3][3];
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++) {
					if (isOutOfBounds(xi + i, yi + j)) neigh[i + 1][j + 1] = 0;
					else neigh[i + 1][j + 1] = image.at<uchar>(yi + j, xi + i);
				}
			}
			
		}
	}
}*/


void HarrisDetector::applySobel(int x, int y)
{
	for (int xi = 0; xi < image.cols;xi++) {
		for (int yi = 0; yi < image.rows; yi++) {

			std::vector<double> sobelVec = sobel(xi, yi);
			processed.at<uchar>(yi, xi) = static_cast<uchar>(round(sqrt(pow(sobelVec[0], 2) + pow(sobelVec[1], 2))));
			
		}
	}
}

std::vector<double> HarrisDetector::sobel(int x, int y) {
	uchar neigh[3][3];
	std::vector<double> ret;

	//get relevant pixels from image
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (isOutOfBounds(x + i, y + j)) neigh[i + 1][j + 1] = 0;
			else neigh[i + 1][j + 1] = image.at<uchar>(y + j, x + i);
		}
	}

	//calculate sobel derivatives
	//todo: seperate, calculation on paper somehow yielded negated result
	double gx = abs(-neigh[0][0] + neigh[2][0]
		- neigh[0][1] * 2 + neigh[2][1] * 2
		- neigh[0][2] + neigh[2][2])/8;
	double gy = abs(neigh[0][0] + neigh[1][0] * 2 + neigh[2][0]
		- neigh[0][2] - neigh[1][2] * 2 - neigh[2][2])/8;
	ret.push_back(gx);
	ret.push_back(gy);
	return ret;
}

bool HarrisDetector::isOutOfBounds(int x, int y)
{
	//return wether selected pixel lies beyond image limits
	if (x > image.cols - 1 || x<0 || y>image.rows - 1 || y < 0) return true;
	else return false;
}


void HarrisDetector::applyHarrisCornerDetection(int window)
{
	double highest = 0;
	std::vector<std::vector<double>> sobels;
	std::vector<double> scores;
	sobels.reserve(image.cols*image.rows);
	scores.reserve(image.cols*image.rows);
	//calculate sobels
	for (int xi = 0; xi < image.cols; xi++) {
		for (int yi = 0; yi < image.rows; yi++) {
			if (xi == 2 && yi == 100)
				std::cout << "2 100 at position" << sobels.size() << std::endl;
			sobels.push_back(sobel(xi, yi));
		}
	}
	


	//calculate score
	
	for (int xi = 0; xi < image.cols; xi++) {
		for (int yi = 0; yi < image.rows; yi++) {

			double A = 0;
			double B = 0;
			double C = 0;

			for (int i = -window; i < window+1; i++) {
				for (int j = -window; j < window+1; j++) {
					//
					
					if (!isOutOfBounds(xi + i, yi + j)) {
						//std::cout << "Accessing " << (xi*image.cols) + yi << std::endl;
						int access = (xi + i)*image.rows + (yi + j);
						if (xi == 2 && yi == 100 && i == 0 && j == 0)std::cout << "Trying to access element 2 100 at position " << access << std::endl;
						
						//calculate values necessary to determine determinate and trace
						A += pow((sobels[access])[0], 2);
						C += pow((sobels[access])[1], 2);
						B += (sobels[access])[0] * (sobels[access])[1];
						//B += (sobels[(xi*image.cols)  + yi])[0] * (sobels[(xi*image.cols)  + yi])[1];
					}
				}
			}
			

			//calculate harris score "cornerness" in lieu of eigenvalues
			double score = A*C - pow(B, 2) - 0.04*pow(A + C, 2);
			if (score > highest) {
				highest = score;
				std::cout << "new Highest: " << highest << std::endl;
			}


			//colour edges
			if (score > pow(10, 5)) imageColour.at<cv::Vec3b>(yi, xi) = cv::Vec3b(0, 0, 255);
			scores.push_back(score);

			//TODO: supress non local maxima
			



		}
	}
}
