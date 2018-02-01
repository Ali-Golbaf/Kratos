# ==============================================================================
#  KratosShapeOptimizationApplication
#
#  License:         BSD License
#                   license: ShapeOptimizationApplication/license.txt
#
#  Main authors:    Baumgaertner Daniel, https://github.com/dbaumgaertner
#
# ==============================================================================

# Making KratosMultiphysics backward compatible with python 2.6 and 2.7
from __future__ import print_function, absolute_import, division 

# importing the Kratos Library
from KratosMultiphysics import *
from KratosMultiphysics.ALEApplication import *
from KratosMultiphysics.ShapeOptimizationApplication import *

# check that KratosMultiphysics was imported in the main script
CheckForPreviousImport()

# Additional imports
import time as timer

from mesh_controller_basic import MeshControlerBasic
from mesh_controller_ale_solver import MeshControllerALESolver

# ==============================================================================
def CreateMeshController( OptimizationModelPart, OptimizationSettings ):
    MeshMotionSettings = OptimizationModelPart["design_variables"]["mesh_motion"]
    if MeshMotionSettings["apply_ale_mesh_solver"].GetBool():
        return MeshControllerALESolver( OptimizationModelPart, MeshMotionSettings)
    else
        return MeshControlerBasic( OptimizationModelPart, MeshMotionSettings )


# # ==============================================================================