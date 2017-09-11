//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Philipp Bucher, Jordi Cotela
//
// See Master-Thesis P.Bucher
// "Development and Implementation of a Parallel
//  Framework for Non-Matching Grid Mapping"



#if !defined(KRATOS_MAPPING_APPLICATION_H_INCLUDED )
#define  KRATOS_MAPPING_APPLICATION_H_INCLUDED


// System includes
#include <string>
#include <iostream>


// External includes


// Project includes
#include "includes/define.h"
#include "includes/kratos_application.h"
#include "includes/variables.h"

#include "custom_conditions/nearest_neighbor_mapper_condition.h"
#include "custom_conditions/nearest_element_mapper_condition.h"
#include "custom_conditions/mortar_mapper_condition.h"


namespace Kratos
{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/// Short class definition.
/** Detail class definition.
*/
class KratosMappingApplication : public KratosApplication
{
public:
    ///@name Type Definitions
    ///@{


    /// Pointer definition of KratosMappingApplication
    KRATOS_CLASS_POINTER_DEFINITION(KratosMappingApplication);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    KratosMappingApplication();

    /// Destructor.
    virtual ~KratosMappingApplication() {}


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{

    virtual void Register();



    ///@}
    ///@name Access
    ///@{


    ///@}
    ///@name Inquiry
    ///@{


    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    virtual std::string Info() const
    {
        return "KratosMappingApplication";
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << Info();
        PrintData(rOStream);
    }

    ///// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const
    {
        KRATOS_WATCH("in my application");
        KRATOS_WATCH(KratosComponents<VariableData>::GetComponents().size() );

        rOStream << "Variables:" << std::endl;
        KratosComponents<VariableData>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Elements:" << std::endl;
        KratosComponents<Element>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Conditions:" << std::endl;
        KratosComponents<Condition>().PrintData(rOStream);
    }


    ///@}
    ///@name Friends
    ///@{


    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{


    ///@}
    ///@name Protected member Variables
    ///@{


    ///@}
    ///@name Protected Operators
    ///@{


    ///@}
    ///@name Protected Operations
    ///@{


    ///@}
    ///@name Protected  Access
    ///@{


    ///@}
    ///@name Protected Inquiry
    ///@{


    ///@}
    ///@name Protected LifeCycle
    ///@{


    ///@}

private:
    ///@name Static Member Variables
    ///@{

    // static const ApplicationCondition  msApplicationCondition;

    ///@}
    ///@name Member Variables
    ///@{

    const NearestNeighborMapperCondition   mNearestNeighborMapperCondition2D1N;
    const NearestNeighborMapperCondition   mNearestNeighborMapperCondition3D1N;

    const NearestElementMapperCondition   mNearestElementMapperCondition2D2NLine; // 2D-Line
    const NearestElementMapperCondition   mNearestElementMapperCondition3D2NLine; // 3D-Line
    const NearestElementMapperCondition   mNearestElementMapperCondition2D3NTri;  // 2D-Tri
    const NearestElementMapperCondition   mNearestElementMapperCondition3D3NTri;  // 3D-Tri
    const NearestElementMapperCondition   mNearestElementMapperCondition2D4NQuad; // 2D-Quad
    const NearestElementMapperCondition   mNearestElementMapperCondition3D4NQuad; // 3D-Quad
    const NearestElementMapperCondition   mNearestElementMapperCondition3D4N;     // Tetra
    const NearestElementMapperCondition   mNearestElementMapperCondition3D8N;     // Hexa

    const MortarMapperCondition   mMortarMapperCondition2D2NLine; // 2D-Line
    const MortarMapperCondition   mMortarMapperCondition3D2NLine; // 3D-Line
    const MortarMapperCondition   mMortarMapperCondition2D3NTri;  // 2D-Tri
    const MortarMapperCondition   mMortarMapperCondition3D3NTri;  // 3D-Tri
    const MortarMapperCondition   mMortarMapperCondition2D4NQuad; // 2D-Quad
    const MortarMapperCondition   mMortarMapperCondition3D4NQuad; // 3D-Quad
    const MortarMapperCondition   mMortarMapperCondition3D4N;     // Tetra
    const MortarMapperCondition   mMortarMapperCondition3D8N;     // Hexa

    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{


    ///@}
    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{

    /// Assignment operator.
    KratosMappingApplication& operator=(KratosMappingApplication const& rOther);

    /// Copy constructor.
    KratosMappingApplication(KratosMappingApplication const& rOther);


    ///@}

}; // Class KratosMappingApplication

///@}


///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

///@}


}  // namespace Kratos.

#endif // KRATOS_MAPPING_APPLICATION_H_INCLUDED  defined