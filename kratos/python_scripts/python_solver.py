from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7

# Importing Kratos
import KratosMultiphysics

class PythonSolver(object):
    """The base class for the Python Solvers in the applications
    Changes to this BaseClass have to be discussed first!
    """
    def __init__(self, model_part, solver_settings):
        """The constructor of the PythonSolver-Object.

        It is intended to be called from the constructor
        of deriving classes:
        super(DerivedSolver, self).__init__(solver_settings)

        Keyword arguments:
        self -- It signifies an instance of a class.
        model_part -- The ModelPart to be used
        solver_settings -- The solver settings used
        """
        if (type(model_part) != KratosMultiphysics.ModelPart):
            raise Exception("Input is expected to be provided as a Kratos Model object")

        if (type(solver_settings) != KratosMultiphysics.Parameters):
            raise Exception("Input is expected to be provided as a Kratos Parameters object")

        self.main_model_part = model_part
        self.solver_settings = solver_settings

    def AddVariables(self):
        pass

    def AddDofs(self):
        pass

    def GetMinimumBufferSize(self):
        pass

    def ReadModelPart(self):
        pass

    def PrepareModelPartForSolver(self):
        pass

    def ExportModelPart(self):
        pass

    def ComputeDeltaTime(self):
        pass

    def Solve(self):
        self.Initialize()
        self.InitializeSolutionStep()
        self.Predict()
        self.SolveSolutionStep()
        self.FinalizeSolutionStep()

    def Initialize(self):
        pass

    def Predict(self):
        pass

    def InitializeSolutionStep(self):
        pass

    def FinalizeSolutionStep(self):
        pass

    def SolveSolutionStep(self):
        pass

    def Check(self):
        pass

    def Clear(self):
        pass

    def GetComputingModelPart(self):
        pass