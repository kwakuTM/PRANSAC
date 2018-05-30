#pragma once

#include <iostream>
#include <random>
#include <cmath>
#include <string>
#include <omp.h>
#include <algorithm>
#include <vector>
#include <memory>

#include "DimModel.hpp"


namespace PRANSAC
{ 
   // T - AbstractPlane
	template <class P, int planeParams>
    class Ransac
    {
    private:
    	std::vector<std::shared_ptr<P>> allData; //All the data
    	std::vector<std::shared_ptr<P>> allSampledModels; //vector of sampled models
    	std::vector<P> m_BestModel; // the best model
    	std::vector<std::vector<std::shared_ptr<DimSpace>>> m_BestInliers;
    	std::vector<std::shared_ptr<DimSpace>> sampledPoints;

    	int maxIterations; // Number of iterations before terminations
    	NPfloat threshold; // the threshold buffer
    	NPfloat bestModelScore;
    	int bestModelIdx; 

    	std::vector<std::mt19937>m_RandEnginers;  // Mersenne twister high quality RNG that support *OpenMP* multi-threading
    }; 
}