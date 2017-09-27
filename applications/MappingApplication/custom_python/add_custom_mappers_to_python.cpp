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
#include <boost/python.hpp>

// Project includes
#include "custom_python/add_custom_mappers_to_python.h"
#include "custom_utilities/mapper_factory_new.h"
#include "custom_strategies/builders/ublas_mapping_matrix_builder.h"


namespace Kratos
{

namespace Python
{

// Wrapper functions for taking a default argument for the flags // TODO inline? Jordi
inline void UpdateInterface(Mapper& dummy)
{
    Kratos::Flags dummy_flags = Kratos::Flags();
    double dummy_search_radius = -1.0f;
    dummy.UpdateInterface(dummy_flags, dummy_search_radius);
}

inline void UpdateInterface(Mapper& dummy, Kratos::Flags options)
{
    double dummy_search_radius = -1.0f;
    dummy.UpdateInterface(options, dummy_search_radius);
}

inline void UpdateInterface(Mapper& dummy, double search_radius)
{
    Kratos::Flags dummy_flags = Kratos::Flags();
    dummy.UpdateInterface(dummy_flags, search_radius);
}

inline void Map(Mapper& dummy,
         const Variable<double>& origin_variable,
         const Variable<double>& destination_variable)
{
    Kratos::Flags dummy_flags = Kratos::Flags();
    dummy.Map(origin_variable, destination_variable, dummy_flags);
}

inline void Map(Mapper& dummy,
         const Variable< array_1d<double, 3> >& origin_variable,
         const Variable< array_1d<double, 3> >& destination_variable)
{
    Kratos::Flags dummy_flags = Kratos::Flags();
    dummy.Map(origin_variable, destination_variable, dummy_flags);
}

inline void InverseMap(Mapper& dummy,
                const Variable<double>& origin_variable,
                const Variable<double>& destination_variable)
{
    Kratos::Flags dummy_flags = Kratos::Flags();
    dummy.InverseMap(origin_variable, destination_variable, dummy_flags);
}

inline void InverseMap(Mapper& dummy,
                const Variable< array_1d<double, 3> >& origin_variable,
                const Variable< array_1d<double, 3> >& destination_variable)
{
    Kratos::Flags dummy_flags = Kratos::Flags();
    dummy.InverseMap(origin_variable, destination_variable, dummy_flags);
}

void  AddCustomMappersToPython()
{
    using namespace boost::python;

    typedef UblasSpace<double, CompressedMatrix, Vector> SerialSparseSpaceType;
    typedef UblasSpace<double, Matrix, Vector> LocalSpaceType;
    typedef LinearSolver<SerialSparseSpaceType, LocalSpaceType> SerialLinearSolverType; // for Mortar
    typedef UblasMappingMatrixBuilder<SerialSparseSpaceType, LocalSpaceType> SerialMappingMatrixBuilderType;

    void (*pUpdateInterface)(Mapper &)
        = &UpdateInterface;

    void (*pUpdateInterfaceOptions)(Mapper &, Kratos::Flags)
        = &UpdateInterface;

    void (*pUpdateInterfaceSearchRadius)(Mapper &, double)
        = &UpdateInterface;

    void (*pMapScalar)(Mapper &,
                       const Variable<double> &,
                       const Variable<double> &)
        = &Map;

    void (*pMapVector)(Mapper &,
                       const Variable< array_1d<double, 3> > &,
                       const Variable< array_1d<double, 3> > &)
        = &Map;

    void (*pInverseMapScalar)(Mapper &,
                              const Variable<double> &,
                              const Variable<double> &)
        = &InverseMap;

    void (*pInverseMapVector)(Mapper &,
                              const Variable< array_1d<double, 3> > &,
                              const Variable< array_1d<double, 3> > &)
        = &InverseMap;


    void (Mapper::*pUpdateInterfaceFull)(Kratos::Flags, double)
        = &Mapper::UpdateInterface;

    void (Mapper::*pMapScalarOptions)(const Variable<double> &,
            const Variable<double> &,
            Kratos::Flags)
        = &Mapper::Map;

    void (Mapper::*pMapVectorOptions)(const Variable< array_1d<double, 3> > &,
            const Variable< array_1d<double, 3> > &,
            Kratos::Flags)
        = &Mapper::Map;

    void (Mapper::*pInverseMapScalarOptions)(const Variable<double> &,
            const Variable<double> &,
            Kratos::Flags)
        = &Mapper::InverseMap;

    void (Mapper::*pInverseMapVectorOptions)(const Variable< array_1d<double, 3> > &,
            const Variable< array_1d<double, 3> > &,
            Kratos::Flags)
        = &Mapper::InverseMap;
    
    // Exposing the base class of the Mappers to Python, but without constructor
    class_< Mapper, Mapper::Pointer, boost::noncopyable > mapper 
        = class_< Mapper, Mapper::Pointer, boost::noncopyable >("Mapper", no_init)
            .def("UpdateInterface",  pUpdateInterface)
            .def("UpdateInterface",  pUpdateInterfaceOptions)
            .def("UpdateInterface",  pUpdateInterfaceSearchRadius)
            .def("Map",              pMapScalar)
            .def("Map",              pMapVector)
            .def("InverseMap",       pInverseMapScalar)
            .def("InverseMap",       pInverseMapVector)

            .def("UpdateInterface",  pUpdateInterfaceFull)
            .def("Map",              pMapScalarOptions)
            .def("Map",              pMapVectorOptions)
            .def("InverseMap",       pInverseMapScalarOptions)
            .def("InverseMap",       pInverseMapVectorOptions)
            ;
    
    // Adding the flags that can be used while mapping
    mapper.attr("SWAP_SIGN") = MapperFlags::SWAP_SIGN;
    mapper.attr("ADD_VALUES") = MapperFlags::ADD_VALUES;
    mapper.attr("CONSERVATIVE") = MapperFlags::CONSERVATIVE;
    mapper.attr("REMESHED") = MapperFlags::REMESHED;

    // Jordi is it possible to expose the mappers without a constructor and use them only through the factory?
    // This would circumvent problems with the wrong space being selected

    // Exposing the Mappers
    // Matrix-free Mappers
    class_< NearestNeighborMapper, bases<Mapper>, boost::noncopyable>
    ("NearestNeighborMapper", init<ModelPart&, ModelPart&, Parameters>());
    class_< NearestElementMapper, bases<Mapper>, boost::noncopyable>
    ("NearestElementMapper", init<ModelPart&, ModelPart&, Parameters>());
    // Matrix-based Mappers
    class_<NearestNeighborMapperMatrix<SerialMappingMatrixBuilderType, SerialLinearSolverType>,
            bases<Mapper>, boost::noncopyable>("NearestNeighborMapperMatrix", init<ModelPart &, ModelPart &, Parameters>());
    class_<NearestElementMapperMatrix<SerialMappingMatrixBuilderType, SerialLinearSolverType>,
            bases<Mapper>, boost::noncopyable>("NearestElementMapperMatrix", init<ModelPart &, ModelPart &, Parameters>());
    class_<MortarMapper<SerialMappingMatrixBuilderType, SerialLinearSolverType>,
            bases<Mapper>, boost::noncopyable>("MortarMapper", init<ModelPart &, ModelPart &, Parameters>());

    // Exposing the MapperFactory
    class_< MapperFactoryNew, boost::noncopyable>("MapperFactoryNew", no_init)
    .def("CreateMapper", &MapperFactoryNew::CreateMapper)
    .staticmethod("CreateMapper")
    ;
}

}  // namespace Python.

} // Namespace Kratos
