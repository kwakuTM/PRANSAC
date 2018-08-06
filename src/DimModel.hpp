#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>
#include <memory>

namespace PRANSAC
{
    typedef double NPdouble;

    // A class point where the point model is constructed from
    class DimSpace
    {
    public:
	    virtual ~DimSpace(void) {}; // To make this polymorphic we add dummy destructor
    };

    template <int planeDim> /* Minimum number of parameters required to define the plane model*/
    class Plane3D
    {
    protected:
    std::array<std::shared_ptr<DimSpace>, planeDim> numPlaneParams;

        virtual NPdouble ComputePointDistance(std::shared_ptr<DimSpace> pt) = 0;

    public:
        virtual void Initialize(const std::vector<std::shared_ptr<DimSpace>> &InputParams) = 0;
        virtual std::pair<NPdouble, std::vector<std::shared_ptr<DimSpace>>> Eval(const std::vector<std::shared_ptr<DimSpace>> &EvaluateParams, NPdouble Threshold) = 0;
        virtual std::array<std::shared_ptr<DimSpace>, planeDim> GetModelParams(void) { return numPlaneParams; };
        virtual ~Plane3D(void){};
    };
} // namespace PRANSAC

