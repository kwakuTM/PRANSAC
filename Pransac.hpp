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
    	std::vector<P> m_BestModel;
    };
}