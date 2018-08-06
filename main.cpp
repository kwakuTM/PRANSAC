#include <iostream>
#include <iomanip>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <random>

#include "Pransac.hpp"
#include "PlaneModel.hpp"

int main(int argc, char * argv[])
{
	if (argc != 1 && argc != 2)
	{
		std::cout << "[ USAGE ]: " << argv[0] << "[<nPoints> = 500]" << std::endl;
		return -1;
	}

	std::string path;

	if (argc == 2)
	{
        path = argv[1];
	}

	PRANSAC::RANSAC<PlaneModel, 3, 30> Estimator;

	Estimator.Initialize(0.5, 5000); // Threshold, iterations
	int start = cv::getTickCount();
	Estimator.Estimate(); // points to be checked must be passed into this function
	int end = cv::getTickCount();
	std::cout << "RANSAC took: " << PRANSAC::NPdouble(end - start) / PRANSAC::NPdouble(cv::getTickFrequency()) * 1000.0 << " ms." << std::endl;
	
	return 0;
}