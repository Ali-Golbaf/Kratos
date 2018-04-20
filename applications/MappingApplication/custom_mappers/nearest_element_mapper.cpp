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

// System includes

// External includes

// Project includes
#include "nearest_element_mapper.h"
#include "custom_utilities/mapper_typedefs.h"

namespace Kratos
{
/***********************************************************************************/
/* PUBLIC Methods */
/***********************************************************************************/


/***********************************************************************************/
/* PROTECTED Methods */
/***********************************************************************************/


/***********************************************************************************/
/* PRIVATE Methods */
/***********************************************************************************/


// /// input stream function
// inline std::istream & operator >> (std::istream& rIStream, NearestElementMapper& rThis);

// /// output stream function
// inline std::ostream & operator << (std::ostream& rOStream, const NearestElementMapper& rThis) {
//   rThis.PrintInfo(rOStream);
//   rOStream << " : " << std::endl;
//   rThis.PrintData(rOStream);
//   return rOStream;
// }

///////////////////////////////////////////////////////////////////////////////////////////////////
// Class template instantiation
template class NearestElementMapper< MapperDefinitions::UblasSparseSpaceType, MapperDefinitions::DenseSpaceType >;
#ifdef KRATOS_USING_MPI // mpi-parallel compilation
template class NearestElementMapper< MapperDefinitions::TrilinosSparseSpaceType, MapperDefinitions::DenseSpaceType >;
#endif

}  // namespace Kratos.