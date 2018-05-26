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
    class AbstractPoint
    {
    protected:
        bool picked= false;

    public:
        virtual bool pointIsPicked(void){ picked= !picked; };
	    virtual ~AbstractPoint(void) {}; // To make this polymorphic we add dummy destructor
    };

    template <int paramsNum> /* Minimum number of parameters required to define this model*/
    class AbstractPlane
    {
    protected:
	std::array<std::shared_ptr<AbstractPoint>, paramsNum> numModelParams; 

        virtual NPfloat ComputePointDistance(std::shared_ptr<AbstractPoint> Param) = 0;

    public:
        virtual void Initialize(const std::vector<std::shared_ptr<AbstractPoint>> &InputParams) = 0;
        virtual std::pair<NPfloat, std::vector<std::shared_ptr<AbstractPoint>>> Evaluate(const std::vector<std::shared_ptr<AbstractPoint>> &EvaluateParams, NPfloat Threshold) = 0;

        virtual std::array<std::shared_ptr<AbstractPoint>, paramsNum> GetModelParams(void) { return numModelParams; };
        virtual ~AbstractPlane(void){};
    };
} // namespace PRANSAC

