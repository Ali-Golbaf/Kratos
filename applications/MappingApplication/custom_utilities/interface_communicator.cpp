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
#include "interface_communicator"


namespace Kratos
{
    InterfaceCommunicator::InterfaceCommunicator(ModelPart& rModelPartOrigin
                                                 Modelpart::Pointer pModelPartDestination)
                                                 : mrModelPartOrigin(rModelPartOrigin),
                                                   mpModelPartDestination(mpModelPartDestination)
    {

    }



}  // namespace Kratos.


