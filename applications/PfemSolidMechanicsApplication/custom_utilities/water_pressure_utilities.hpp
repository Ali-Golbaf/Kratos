//
//   Project Name:        KratosSolidMechanicsApplication $
//   Last modified by:    $Author:              LMonforte $
//   Date:                $Date:                July 2015 $
//   Revision:            $Revision:                  0.1 $
//
//

#if !defined(KRATOS_WATER_PRESSURE_UTILITIES)
#define KRATOS_WATER_PRESSURE_UTILITIES


// System includes

// External includes
//#include "boost/smart_ptr.hpp"

// Project includes
#include "includes/define.h"
#include "includes/variables.h"
#include "utilities/math_utils.h"

#include "includes/properties.h"

#include "custom_elements/large_displacement_element.hpp" //needed??


namespace Kratos
{


   class WaterPressureUtilities
   {

      public:
         typedef Matrix MatrixType;

         typedef Vector VectorType;

         typedef unsigned int IndexType;

         typedef unsigned int SizeType;

         typedef Properties PropertiesType; 

         typedef Node < 3 > NodeType; 
         typedef Geometry <NodeType>   GeometryType; 

         WaterPressureUtilities();

         virtual ~WaterPressureUtilities() {};


         VectorType& CalculateAndAddHydroProblem( VectorType & rRightHandSide, const VectorType & rBaseClassRHS, const VectorType & rVolumeForce, const int number_of_variables, GeometryType & rGeometry, const Properties & rProperties, const MatrixType & rB, const MatrixType & rDN_DX, const VectorType & rN, const double & rDetF0, const double & rTimeStep, const Matrix & rTotalF, const double & rIntegrationWeight, const double rCurrentRadius = 0.0);

         VectorType & CalculateAndAddStabilization( Vector & rRightHandSide, const int number_of_variables, GeometryType & rGeometry, const PropertiesType & rProperties, const Matrix & rDN_DX, const double & rConstrainedModulus, const double & rDetF0, const double & rTimeStep, const double & rIntegrationWeight);

         MatrixType& CalculateAndAddHydroProblemLHS( MatrixType & rLeftHandSide, const MatrixType & rBaseClassLHS, const VectorType & rVolumeForce, const int number_of_variables, GeometryType & rGeometry, const Properties & rProperties, const MatrixType & rB, const MatrixType & rDN_DX, const VectorType & rN, const double & rDetF0, const double & rTimeStep, const Matrix & rTotalF, const double & rIntegrationWeight, const double rCurrentRadius = 0.0);

         MatrixType & CalculateAndAddStabilizationLHS( Matrix & rLeftHandSide, const int number_of_variables, GeometryType & rGeometry, const PropertiesType & rProperties, const Matrix & rDN_DX, const double & rConstrainedModulus, const double & rDetF0, const double & rTimeStep, const double & rIntegrationWeight);

         void GetPermeabilityTensor( const PropertiesType & rProperties, const Matrix & rTotalF, Matrix & rK,  const double & rInitial_porosity, const unsigned int & rDimension, const double & rVolume );

      protected:


         // Get Properties 
         void GetConstants( double& rScalingConstant, double & rWaterBulk, double & rDeltaTime, double & rPermeability, const PropertiesType& rProperties);

         void GetScalingConstant( double& rScalingConstant, const PropertiesType& pProperties);

         void GetPermeabilityTensor( const PropertiesType & rProperties, const Matrix & rTotalF, Matrix & rK , const double & rInitial_porosity, const double & rVolume);


         virtual void GetVoigtSize( const unsigned int dimension, unsigned int & voigtsize, unsigned int & principal_dimension); 

         double & ComputeStabilizationFactor ( double & rAlphaStabilization, const PropertiesType & rProperties, const Matrix & rDN_DX, const double & rTimeStep, const double & rConstrainedModulus);

         virtual double CalculateVolumeChange( GeometryType & rGeometry, const Vector & rN, const Matrix & rTotalF);
         
         // CALCULATE RHS 
         VectorType& CalculateWaterPressureForces( VectorType& rRightHandSide , GeometryType & rGeometry,  const PropertiesType & rProperties, const MatrixType & rDN_DX, const Vector & rN, const double & rDetF0, const Matrix & rTotalF, const double & rDeltaTime, const double & rIntegrationWeight);

         virtual VectorType& CalculateAndAddWaterPressureForcesDisplacement( VectorType& rRightHandSide , GeometryType & rGeometry,  const PropertiesType & rProperties, const MatrixType & rDN_DX, const Vector & rN, const double & rDetF0, const Matrix & rTotalF, const double & rDeltaTime, const double & rIntegrationWeight, const double & rCurrentRadius);


         VectorType& CalculateWaterInternalForcesContribution( VectorType& rRightHandSideVector, GeometryType& rGeometry, const PropertiesType & rProperties, const Matrix & rB, const Vector & rN, const double & rIntegrationWeight);

         VectorType & CalculateVolumeForcesContribution( VectorType & rRightHandSideVector, GeometryType& rGeometry, const PropertiesType & rProperties, const Vector & rVolumeForce, const Vector & rN, const double & rDetF0, const double & rIntegrationWeight);

         VectorType & CalculateStabilization( VectorType & rLocalRHS, GeometryType & rGeometry,  const PropertiesType & rProperties, const Matrix & rDN_DX, const double & rTimeStep, const double & rConstrainedModulus, const double & rIntegrationWeight);

         // RESHAPCE RHS
         VectorType& AddReshapeBaseClassRHS( VectorType & rRightHandSideVector, const VectorType& rBaseClassRHS, const unsigned int number_of_variables, const unsigned int number_of_nodes);

         VectorType& AddReshapeWaterPressureForces( VectorType & rRightHandSide, const VectorType& rPartialRHS, const unsigned int number_of_variables, const unsigned int number_of_points);

         VectorType& AddReshapeWaterInternalForcesContribution( VectorType & rRightHandSideVector, const VectorType& rPartialRHS, const unsigned int number_of_variables, const unsigned int number_of_nodes, const unsigned int dimension);


         // CALCULATE LHS
         MatrixType & ComputeWaterPressureKuug( MatrixType & LocalLHS, GeometryType & rGeometry, const Matrix & rB, const Vector & rN, const double & rIntegrationWeight);

         virtual MatrixType & ComputeWaterPressureKUwP( MatrixType & LocalLHS, GeometryType & rGeometry, const Matrix & rDN_DX, const VectorType & rN, const double & rIntegrationWeight, const double & rCurrentRadius);

         MatrixType & ComputeWaterPressureKwPwP( MatrixType & rLocalLHS, GeometryType & rGeometry, const PropertiesType & rProperties, const Matrix & rDN_DX, const Matrix & rTotalF, const Vector & rN, const double & rTimeStep, const double & rIntegrationWeight);

         virtual MatrixType & ComputeSolidSkeletonDeformationMatrix(MatrixType & rLocalLHS, GeometryType & rGeometry, const PropertiesType & rProperties, const Matrix & rDN_DX, const Vector & rN, const double & rIntegrationWeight, const double & rCurrentRadius);

         MatrixType & ComputeDarcyFlowGeometricTerms(MatrixType & rLocalLHS, GeometryType & rGeometry, const PropertiesType & rProperties, const Matrix & rB,  const Matrix & rDN_DX, const Matrix & rTotalF, const Vector & rN, const double & rDeltaTime, const double & rIntegrationWeight);

         virtual MatrixType & ComputeDensityChangeTerm( MatrixType & rLocalLHS, GeometryType & rGeometry, const PropertiesType & rProperties, const Vector & rVolumeForce,  const Matrix & rDN_DX, const Vector & rN, const double & rDetF0, const double & rIntegrationWeight, const double & rCurrentRadius);

         MatrixType & CalculateStabilizationLHS( MatrixType & rLocalLHS, GeometryType & rGeometry,  const PropertiesType & rProperties, const Matrix & rDN_DX, const double & rTimeStep, const double & rConstrainedModulus, const double & rIntegrationWeight);


         // RESHAPE LHS
         MatrixType & AddReshapeBaseClassLHS( MatrixType & rLeftHandSide, const MatrixType & rBaseClassLHS, const unsigned int dimension, const unsigned int number_of_variables, const unsigned int number_of_nodes);

         virtual MatrixType & AddReshapeSolidSkeletonDeformationMatrix( MatrixType & rLeftHandSide, const MatrixType & rBaseClassLHS, const unsigned int dimension, const unsigned int number_of_variables, const unsigned int number_of_nodes);

         MatrixType & AddReshapeKUU( MatrixType & rLeftHandSide, const MatrixType & rBaseClassLHS, const unsigned int dimension, const unsigned int number_of_variables, const unsigned int number_of_nodes);

         MatrixType & AddReshapeKUwP( MatrixType & rLeftHandSide, const MatrixType & rBaseClassLHS, const unsigned int dimension, const unsigned int number_of_variables, const unsigned int number_of_nodes);

         MatrixType & AddReshapeKwPwP( MatrixType & rLeftHandSide, const MatrixType & rBaseClassLHS, const unsigned int dimension, const unsigned int number_of_variables, const unsigned int number_of_nodes);

         MatrixType & AddReshapeKwPU( MatrixType & rLeftHandSide, const MatrixType & rBaseClassLHS, const unsigned int dimension, const unsigned int number_of_variables, const unsigned int number_of_nodes);


         // VARIABLES
         // TO CHOOSE THE STABILIZATION 
         bool mPPP;
         
   }; // end Class WaterPressureUtilities

} // end namespace kratos

#endif // KRATOS_WATER_PRESSURE_UTILITIES
