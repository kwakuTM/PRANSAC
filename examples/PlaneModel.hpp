#pragma once

#include "DimModel.hpp"

typedef std::array<PRANSAC::NPdouble, 3> PointArray3P;

class Point3D 
     : public PRANSAC::DimSpace
{
public:
	Point3D(PRANSAC::NPdouble x, PRANSAC::NPdouble y, PRANSAC::NPdouble z)
	{
		m_Point3D[0] = x;
		m_Point3D[1] = y;
		m_Point3D[2] = z;
	}
	PointArray3P m_Point3D; 
};

class PlaneModel 
	: public PRANSAC::Plane3D<3>
{
  protected:
  	// Parametric form
    PRANSAC::NPdouble m_a, m_b, m_c; 
    PRANSAC::NPdouble m_DistDenominator; 

    // Another parametrization y = mx + d
    PRANSAC::NPdouble m_m; 
    PRANSAC::NPdouble m_d; 

    virtual PRANSAC::NPdouble ComputePointDistance(std::shared_ptr<PRANSAC::DimSpace> Param) override
    {
      auto ExtPoint3D = std::dynamic_pointer_cast<Point3D>(Param);
      if(ExtPoint3D == nullptr)
          throw std::runtime_error("PlaneModel::ComputePointDistance() - Passed parameter are not of type Point3D.");

      auto planePoint = std::dynamic_pointer_cast<Point3D>(numPlaneParams[0]);
      PRANSAC::NPdouble Numer = fabs(m_a * (ExtPoint3D->m_Point3D[0] - planePoint->m_Point3D[0]) + m_b * (ExtPoint3D->m_Point3D[1] - planePoint->m_Point3D[1]) + m_c * (ExtPoint3D->m_Point3D[2] - planePoint->m_Point3D[2]) + m_d);
      PRANSAC::NPdouble Dist = Numer / m_DistDenominator;

      return Dist;
    };

    virtual std::shared_ptr<Point3D> unitVectorPQR(std::shared_ptr<Point3D> Point1, std::shared_ptr<Point3D> Point2)
    {
      PRANSAC::NPdouble x = Point2->m_Point3D[0] - Point1->m_Point3D[0];
      PRANSAC::NPdouble y = Point2->m_Point3D[1] - Point1->m_Point3D[1];
      PRANSAC::NPdouble z = Point2->m_Point3D[2] - Point1->m_Point3D[2];

      std::shared_ptr<Point3D> new_Point3D =  std::make_shared<Point3D>(x, y, z);
      return new_Point3D;
    }

    virtual std::shared_ptr<Point3D> crossProduct(std::shared_ptr<Point3D> vect_A, std::shared_ptr<Point3D> vect_B)
    {
      PRANSAC::NPdouble x, y, z;
      std::shared_ptr<Point3D> vect_cross =  std::make_shared<Point3D>(x, y, z);
      vect_cross->m_Point3D[0] = vect_A->m_Point3D[1] * vect_B->m_Point3D[2] - vect_A->m_Point3D[2] * vect_B->m_Point3D[1];
      vect_cross->m_Point3D[1] = vect_A->m_Point3D[0] * vect_B->m_Point3D[2] - vect_A->m_Point3D[2] * vect_B->m_Point3D[0];
      vect_cross->m_Point3D[2] = vect_A->m_Point3D[0] * vect_B->m_Point3D[1] - vect_A->m_Point3D[1] * vect_B->m_Point3D[0];
      return vect_cross;
    }

  public:
    PlaneModel(const std::vector<std::shared_ptr<PRANSAC::DimSpace>> &InputParams)
    {
      Initialize(InputParams);
    };

    virtual void Initialize(const std::vector<std::shared_ptr<PRANSAC::DimSpace>> &InputParams) override
    {
      if(InputParams.size() != 3)
          throw std::runtime_error("PlaneModel - Number of input parameters does not match minimum number required for this model.");

  // Check for AbstractParamter types
      auto Point1 = std::dynamic_pointer_cast<Point3D>(InputParams[0]);
      auto Point2 = std::dynamic_pointer_cast<Point3D>(InputParams[1]);
      auto Point3 = std::dynamic_pointer_cast<Point3D>(InputParams[2]);
      if(Point1 == nullptr || Point2 == nullptr || Point3 == nullptr)
          throw std::runtime_error("PlaneModel - InputParams type mismatch. It is not a Point3D.");

      std::copy(InputParams.begin(), InputParams.end(), numPlaneParams.begin());

      std::shared_ptr<Point3D> vectorPQ = unitVectorPQR(Point1, Point2);
      std::shared_ptr<Point3D> vectorPR = unitVectorPQR(Point1, Point3);
      std::shared_ptr<Point3D> vectorCross = crossProduct(vectorPQ, vectorPR);

      m_a = vectorCross->m_Point3D[0];
      m_b = vectorCross->m_Point3D[1];
      m_c = vectorCross->m_Point3D[2];
      m_d = 0;


      m_DistDenominator = sqrt(m_a * m_a + m_b * m_b + m_c * m_c); // Cache square root for efficiency
    };

    virtual std::pair<PRANSAC::NPdouble, std::vector<std::shared_ptr<PRANSAC::DimSpace>>> Eval(const std::vector<std::shared_ptr<PRANSAC::DimSpace>> &EvaluateParams, PRANSAC::NPdouble Threshold)
    {
      std::vector<std::shared_ptr<PRANSAC::DimSpace>> Inliers;
      int nTotalParams = EvaluateParams.size();
      int nInliers = 0;

      auto planePoint1 = std::dynamic_pointer_cast<Point3D>(numPlaneParams[0]);
      auto planePoint2 = std::dynamic_pointer_cast<Point3D>(numPlaneParams[1]);
      auto planePoint3 = std::dynamic_pointer_cast<Point3D>(numPlaneParams[2]);

      PRANSAC::NPdouble centerX = (planePoint1->m_Point3D[0] + planePoint2->m_Point3D[0] + planePoint3->m_Point3D[0]) / 3;
      PRANSAC::NPdouble centerY = (planePoint1->m_Point3D[1] + planePoint2->m_Point3D[1] + planePoint3->m_Point3D[1]) / 3;
      PRANSAC::NPdouble centerZ = (planePoint1->m_Point3D[2] + planePoint2->m_Point3D[2] + planePoint3->m_Point3D[2]) / 3;
      
      PRANSAC::NPdouble distPoint1 = sqrt(pow(planePoint1->m_Point3D[0] - centerX, 2) + pow(planePoint1->m_Point3D[1] - centerY, 2) + pow(planePoint1->m_Point3D[2] - centerZ, 2));
      PRANSAC::NPdouble distPoint2 = sqrt(pow(planePoint2->m_Point3D[0] - centerX, 2) + pow(planePoint2->m_Point3D[1] - centerY, 2) + pow(planePoint2->m_Point3D[2] - centerZ, 2));
      PRANSAC::NPdouble distPoint3 = sqrt(pow(planePoint3->m_Point3D[0] - centerX, 2) + pow(planePoint3->m_Point3D[1] - centerY, 2) + pow(planePoint3->m_Point3D[2] - centerZ, 2));

      if((distPoint1 && distPoint2 && distPoint3 < 1)){

        for(auto& Param : EvaluateParams)
        {
          if (ComputePointDistance(Param) < Threshold)
          { 
            Inliers.push_back(Param);
            nInliers++;
          }
        }
      }

      PRANSAC::NPdouble InlierFraction = (PRANSAC::NPdouble(nInliers) / PRANSAC::NPdouble(nTotalParams)) * 100; // This is the inlier fraction
      return std::make_pair(InlierFraction, Inliers);
    };

};