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
    	std::vector<P> bestModel; // the best model
    	std::vector<std::vector<std::shared_ptr<DimSpace>>> bestInliers;
    	std::vector<std::shared_ptr<DimSpace>> sampledPoints;

    	int maxIterations; // Number of iterations before terminations
    	NPfloat threshold; // the threshold buffer
    	NPfloat bestModelScore;
    	int bestModelIdx; 

    	std::vector<std::mt19937>RandEngines;  // Mersenne twister high quality RNG that support *OpenMP* multi-threading

    public:
        RANSAC(void)
	    {
             int num_threads = std::max(1, omp_get_max_threads());
             std::cout << "[INFO]: Maximum number of usuable threads: " << num_threads << std::endl;
             for(int i = 0; i < num_threads; ++i)
             {
                std::random_device seedDevice;
                RandEngines.push_back(std::mt19937(seedDevice()));
             }

             Reset();
     	};

        virtual ~RANSAC(void){}; 

        void Initialize(NPdouble threshold, int maxIterations = 1000)
        {
            threshold = threshold;
            maxIterations = maxIterations;
        };

        void Reset(void) 
        {
            //Clear sampled data for next iteration
            allData.clear();
            allSampledModels.clear();

            bestModelIdx = -1;
            bestModelScore = 0;
        };

        const std::vector<std::shared_ptr<DimSpace>>& GetRemainingPoints(void) { return allData; };
        const std::vector<std::vector<std::shared_ptr<DimSpace>>>& GetBestInliers(void) { return bestInliers; };
        std::shared_ptr<T> GetBestModel(void){ return bestModel; };
        
        bool Estimate(const std::vector<std::shared_ptr<DimSpace>>& Data)
        {
            if (Data.size() <= planeParams)
            {
                std::cerr << "[ WARN ]: RANSAC - Input data is too small, RANSAC idle"
                return false;
            }

            allData = Data;

            std::vector<NPdouble> InlierFractionAccum(m_MaxIterations);
            std::vector<std::vector<std::shared_ptr<DimSpace>>> InliersAccum(m_MaxIterations);
            std::vector<std::shared_ptr<DimSpace>> allInlierPoints;
            allSampledModels.resize(m_MaxIterations);

            int numTreads =std::max(1, omp_get_max_threads());
            omp_set_dynamic(0);
            omp_set_num_threads(numTreads);

            //#pragma omp parallel for
            for (int = 0; i < m_MaxIterations; i++)
            {
                if (bestInliers.siz() < num_planes_expected && allData.size() > 3)
                {
                    //Select planeParams random samples
                    std::vector<std::shared_ptr<DimSpace>> sample(planeParams);
                    std::vector<std::shared_ptr<DimSpace>> remainingSamples = allData;

                    std::shuffle(remainingSamples.begin(), remainingSamples.end(), RandEngines[omp_get_thread_num()]); //shuffle to avoid picking same sample
                    std::copy(remainingSamples.begin(), remainingSamples.begin() + planeParams, sample.begin()); //pick first plane values from sample

                    remainingSamples.erase(remainingSamples.begin(), remainingSamples.begin + planeParams);

                    std::shared_ptr<P> sampleModel = std::make_shared<P>(sample);

                    //sample evaluation

                    std::pair<NPdouble, std::vector<std::shared_ptr<DimSpace>>> inlierPair = sampleModel->eval(threshold, remainingSamples);
                    InliersAccum[i] = eval.first;
                    InlierFractionAccum = eval.second;
                }
            }
        };
    };
}