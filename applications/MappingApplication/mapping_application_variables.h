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


#if !defined(KRATOS_MAPPING_APPLICATION_VARIABLES_H_INCLUDED )
#define  KRATOS_MAPPING_APPLICATION_VARIABLES_H_INCLUDED

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "includes/variables.h"
#include "includes/kratos_application.h"

namespace Kratos
{
    KRATOS_DEFINE_APPLICATION_VARIABLE( MAPPING_APPLICATION, int,               MAPPING_MATRIX_EQUATION_ID )
    KRATOS_DEFINE_APPLICATION_VARIABLE( MAPPING_APPLICATION, vector<int>,       MAPPING_MATRIX_NEIGHBOR_EQUATION_IDS)
    KRATOS_DEFINE_APPLICATION_VARIABLE( MAPPING_APPLICATION, vector<double>,    MAPPING_MATRIX_NEIGHBOR_WEIGHTS)

    // variables for debugging
    KRATOS_DEFINE_APPLICATION_VARIABLE( MAPPING_APPLICATION, double, NEIGHBOR_RANK )
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( MAPPING_APPLICATION, NEIGHBOR_COORDINATES )
}

#endif	/* KRATOS_MAPPING_APPLICATION_VARIABLES_H_INCLUDED */