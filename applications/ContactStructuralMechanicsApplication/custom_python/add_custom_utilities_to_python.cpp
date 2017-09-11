// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:		 BSD License
//					 license: StructuralMechanicsApplication/license.txt
//
//  Main authors:    Vicente Mataix
//

// System includes

// External includes
#include <boost/python.hpp>

// Project includes
#include "includes/define.h"
#include "processes/process.h"
#include "custom_python/add_custom_utilities_to_python.h"

#include "spaces/ublas_space.h"
#include "linear_solvers/linear_solver.h"

//Utilities
#include "custom_utilities/bprinter_utility.h"
#include "custom_utilities/tree_contact_search.h"
#include "custom_utilities/exact_mortar_segmentation_utility.h"
#include "custom_utilities/process_factory_utility.h"

namespace Kratos
{
namespace Python
{
void  AddCustomUtilitiesToPython()
{
    using namespace boost::python;
    
    // Adding table from bprinter to python
    class_<BprinterUtility>("BprinterUtility", init<>());
    
    // Tree contact search
    class_<TreeContactSearch>("TreeContactSearch", init<ModelPart&>())
    .def(init<ModelPart&, Parameters>())
    .def("InitializeMortarConditions",&TreeContactSearch::InitializeMortarConditions)
    .def("TotalClearScalarMortarConditions",&TreeContactSearch::TotalClearScalarMortarConditions)
    .def("TotalClearComponentsMortarConditions",&TreeContactSearch::TotalClearComponentsMortarConditions)
    .def("TotalClearALMFrictionlessMortarConditions",&TreeContactSearch::TotalClearALMFrictionlessMortarConditions)
    .def("PartialClearScalarMortarConditions",&TreeContactSearch::PartialClearScalarMortarConditions)
    .def("PartialClearComponentsMortarConditions",&TreeContactSearch::PartialClearComponentsMortarConditions)
    .def("PartialClearALMFrictionlessMortarConditions",&TreeContactSearch::PartialClearALMFrictionlessMortarConditions)
    .def("CreatePointListMortar",&TreeContactSearch::CreatePointListMortar)
    .def("UpdatePointListMortar",&TreeContactSearch::UpdatePointListMortar)
    .def("UpdateMortarConditions",&TreeContactSearch::UpdateMortarConditions)
    .def("CleanMortarConditions",&TreeContactSearch::CleanMortarConditions)
    .def("CheckMortarConditions",&TreeContactSearch::CheckMortarConditions)
    ;
    
    // Exact integration (for testing)
    class_<ExactMortarIntegrationUtility<2,2>>("ExactMortarIntegrationUtility2D2N", init<>())
    .def(init<const unsigned int>())
    .def(init<const unsigned int, const bool>())
    .def("TestGetExactIntegration",&ExactMortarIntegrationUtility<2,2>::TestGetExactIntegration)
    .def("TestGetExactAreaIntegration",&ExactMortarIntegrationUtility<2,2>::TestGetExactAreaIntegration)
    ;
    class_<ExactMortarIntegrationUtility<3,3>>("ExactMortarIntegrationUtility3D3N", init<>())
    .def(init<const unsigned int>())
    .def(init<const unsigned int, const bool>())
    .def("TestGetExactIntegration",&ExactMortarIntegrationUtility<3,3>::TestGetExactIntegration)
    .def("TestGetExactAreaIntegration",&ExactMortarIntegrationUtility<3,3>::TestGetExactAreaIntegration)
    ;
    class_<ExactMortarIntegrationUtility<3,4>>("ExactMortarIntegrationUtility3D4N", init<>())
    .def(init<const unsigned int>())
    .def(init<const unsigned int, const bool>())
    .def("TestGetExactIntegration",&ExactMortarIntegrationUtility<3,4>::TestGetExactIntegration)
    .def("TestGetExactAreaIntegration",&ExactMortarIntegrationUtility<3,4>::TestGetExactAreaIntegration)
    ;
  
    // Process Factory utility
    class_<ProcessFactoryUtility>("ProcessFactoryUtility", init<boost::python::list&>())
    .def(init< >())
    .def("AddProcess",&ProcessFactoryUtility::AddProcess)
    .def("AddProcesses",&ProcessFactoryUtility::AddProcesses)
    .def("ExecuteInitialize",&ProcessFactoryUtility::ExecuteInitialize)
    .def("ExecuteBeforeSolutionLoop",&ProcessFactoryUtility::ExecuteBeforeSolutionLoop)
    .def("ExecuteInitializeSolutionStep",&ProcessFactoryUtility::ExecuteInitializeSolutionStep)
    .def("ExecuteFinalizeSolutionStep",&ProcessFactoryUtility::ExecuteFinalizeSolutionStep)
    .def("ExecuteBeforeOutputStep",&ProcessFactoryUtility::ExecuteBeforeOutputStep)
    .def("ExecuteAfterOutputStep",&ProcessFactoryUtility::ExecuteAfterOutputStep)
    .def("ExecuteFinalize",&ProcessFactoryUtility::ExecuteFinalize)
    .def("Clear",&ProcessFactoryUtility::Clear)
    ;
}

}  // namespace Python.

} // Namespace Kratos

