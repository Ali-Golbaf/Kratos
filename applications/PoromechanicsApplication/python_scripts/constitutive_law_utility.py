from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7
#importing the Kratos Library
from KratosMultiphysics import *
from KratosMultiphysics.PoromechanicsApplication import *
CheckForPreviousImport()


def SetConstitutiveLaw(model_part):
    for prop in model_part.Properties:
        ConstitutiveLawName=prop.GetValue(CONSTITUTIVE_LAW_NAME)
        if(ConstitutiveLawName == "LinearElastic2DPlaneStress"):
            prop.SetValue(CONSTITUTIVE_LAW_POINTER, LinearElastic2DPlaneStressLaw() )
        elif(ConstitutiveLawName == "LinearElastic2DPlaneStrain"):
            prop.SetValue(CONSTITUTIVE_LAW_POINTER, LinearElastic2DPlaneStrainLaw() )
        elif(ConstitutiveLawName == "LinearElastic3D"):
            prop.SetValue(CONSTITUTIVE_LAW_POINTER, LinearElastic3DLaw() )
