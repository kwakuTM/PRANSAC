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

    template <int planeParams> /* Minimum number of parameters required to define the plane model*/
    class Plane
    {
    protected: 

    public:
        
    };
} // namespace PRANSAC

