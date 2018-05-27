#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>
#include <memory>

namespace PRANSAC
{
    typedef float NPfloat;

    // A class point where the point model is constructed from
    class DimSpace
    {
    public:
	    virtual ~DimSpace(void) {}; // To make this polymorphic we add dummy destructor
    };

    template <int planeParams> /* Minimum number of parameters required to define the plane model*/
    class AbstractPlane
    {
    protected:
	std::array<std::shared_ptr<DimSpace>, planeParams> numPlaneParams; 

        virtual NPfloat ComputePointDistance(std::shared_ptr<DimSpace> pt) = 0;

    public:
        virtual void Initialize(const std::vector<std::shared_ptr<DimSpace>> &InputParams) = 0;
        virtual std::pair<NPfloat, std::vector<std::shared_ptr<DimSpace>>> Evaluate(const std::vector<std::shared_ptr<DimSpace>> &EvaluateParams, NPfloat Threshold) = 0;

        virtual std::array<std::shared_ptr<DimSpace>, planeParams> GetModelParams(void) { return numPlaneParams; };
        virtual ~AbstractPlane(void){};
    };
} // namespace PRANSAC

