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
   // P - class Plane3D
	template <class P, int planeDim, int numPlanesExpected>
    class RANSAC
    {
    private:
    	std::vector<std::shared_ptr<DimSpace>> allData; //All the data
    	std::vector<std::shared_ptr<P>> allSampledModels; //vector of sampled models
    	std::vector<P> bestModel; // the best model
    	std::vector<std::vector<std::shared_ptr<DimSpace>>> bestInliers;
    	std::vector<std::shared_ptr<DimSpace>> sampledPoints;

    	int maxIterations; // Number of iterations before terminations
    	NPdouble threshold; // the threshold buffer
    	NPdouble bestModelScore;
    	int bestModelIdx;
        int pointsInPlane; 

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
        std::shared_ptr<P> GetBestModel(void){ return bestModel; };

        bool Estimate(const std::vector<std::shared_ptr<DimSpace>>& Data)
        {
            if (Data.size() <= planeDim)
            {
                std::cerr << "[ WARN ]: RANSAC - Input data is too small, RANSAC idle" << std::endl;
                return false;
            }

            allData = Data;

            std::vector<NPdouble> InlierFractionAccum(maxIterations);
            std::vector<std::vector<std::shared_ptr<DimSpace>>> InliersAccum(maxIterations);
            std::vector<std::shared_ptr<DimSpace>> allInlierPoints;
            allSampledModels.resize(maxIterations);

            int numTreads =std::max(1, omp_get_max_threads());
            omp_set_dynamic(0);
            omp_set_num_threads(numTreads);

            //#pragma omp parallel for
            for (int i = 0; i < maxIterations; i++)
            {
                if (bestInliers.size() < numPlanesExpected && allData.size() > 3)
                {
                    //Select planeParams random samples
                    std::vector<std::shared_ptr<DimSpace>> sample(planeDim);
                    std::vector<std::shared_ptr<DimSpace>> remainingSamples = allData;

                    std::shuffle(remainingSamples.begin(), remainingSamples.end(), RandEngines[omp_get_thread_num()]); //shuffle to avoid picking same sample
                    std::copy(remainingSamples.begin(), remainingSamples.begin() + planeDim, sample.begin()); //pick first plane values from sample

                    remainingSamples.erase(remainingSamples.begin(), remainingSamples.begin() + planeDim);

                    std::shared_ptr<P> sampleModel = std::make_shared<P>(sample);

                    //sample evaluation

                    std::pair<NPdouble, std::vector<std::shared_ptr<DimSpace>>> inlierPair = sampleModel->Eval(remainingSamples, threshold);
                    InlierFractionAccum[i] = inlierPair.first;
					InliersAccum[i] = inlierPair.second;
                    // Push back into history.
                    allSampledModels[i] = sampleModel;

                    if (InlierFractionAccum[i] > bestModelScore && InliersAccum[i].size() > pointsInPlane){
                        bestModelScore = InlierFractionAccum[i];
                        bestModelIdx = allSampledModels.size() - 1;
                        bestInliers.push_back(InliersAccum[i]);
                        for (auto j: InliersAccum[i])
                            {
                                allInlierPoints.push_back(j);
                            }
                            for(int i = 0; i < allInlierPoints.size(); ++i)
                            {
                                allData.erase(std::remove(allData.begin(), allData.end(), allInlierPoints[i]), allData.end());
                            }
                    }
                }
            }
        };
    };
}