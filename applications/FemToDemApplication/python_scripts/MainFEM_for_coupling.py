from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7

import MainFemDem
import KratosMultiphysics
import KratosMultiphysics.FemToDemApplication   as KratosFemDem
import cleaning_utility
import adaptive_mesh_refinement_utility
import gid_output_utility

# Python script created to modify the existing one due to the coupling of the DEM app

class FEM_for_coupling_Solution(MainFemDem.FEM_Solution):

	def Info(self):
		print("FEM part of the FEMDEM application")


	def Initialize(self):

		#### INITIALIZE ####
		
		# Add variables (always before importing the model part)
		self.solver.AddVariables()

		# For remeshing purposes
		self.main_model_part.AddNodalSolutionStepVariable(KratosFemDem.NODAL_STRESS_VECTOR)
		self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.NODAL_AREA)
		self.main_model_part.AddNodalSolutionStepVariable(KratosFemDem.EQUIVALENT_NODAL_STRESS)
		
		# Read model_part (note: the buffer_size is set here) (restart is read here)
		self.solver.ImportModelPart()

		# Add dofs (always after importing the model part)
		if((self.main_model_part.ProcessInfo).Has(KratosMultiphysics.IS_RESTARTED)):
			if(self.main_model_part.ProcessInfo[KratosMultiphysics.IS_RESTARTED] == False):
				self.solver.AddDofs()
		else:
			self.solver.AddDofs()


		# Add materials (assign material to model_parts if Materials.json exists)
		self.AddMaterials()
		

		# Add processes
		self.model_processes = self.AddProcesses()
		self.model_processes.ExecuteInitialize()


		# Print model_part and properties
		if(self.echo_level > 1):
			print("")
			print(self.main_model_part)
			for properties in self.main_model_part.Properties:
				print(properties)


		#### START SOLUTION ####

		self.computing_model_part = self.solver.GetComputingModelPart()

		## Sets strategies, builders, linear solvers, schemes and solving info, and fills the buffer
		self.solver.Initialize()
		#self.solver.InitializeStrategy()
		self.solver.SetEchoLevel(self.echo_level)

		
		# Initialize GiD  I/O (gid outputs, file_lists)
		self.SetGraphicalOutput()
		
		self.GraphicalOutputExecuteInitialize()


		print(" ")
		print("::[KSM Simulation]:: Analysis -START- ")

		self.model_processes.ExecuteBeforeSolutionLoop()

		self.GraphicalOutputExecuteBeforeSolutionLoop()		

		# Set time settings
		self.step	   = self.main_model_part.ProcessInfo[KratosMultiphysics.STEP]
		self.time	   = self.main_model_part.ProcessInfo[KratosMultiphysics.TIME]

		self.end_time   = self.ProjectParameters["problem_data"]["end_time"].GetDouble()
		self.delta_time = self.ProjectParameters["problem_data"]["time_step"].GetDouble()


		###   ------  Initializing Adaptive Mesh Refinement  ----------####
		self.cleaning_util = cleaning_utility.CleaningUtility(self.problem_path)

		"""self.gid_output_util = gid_output_utility.GidOutputUtility(self.ProjectParameters,
			                                                       self.problem_name,
			                                                       self.start_time,
			                                                       self.end_time,
			                                                       self.delta_time)

		self.constitutive_law_utility = []  # must be changed->provisional TODO
		self.conditions_util = []



		#self.activate_AMR = self.ProjectParameters["AMR_data"]["activate_AMR"].GetBool()
		self.current_id = 1


		# Initialize the AMR_util
		if(self.activate_AMR):
			self.AMR_util = adaptive_mesh_refinement_utility.AdaptiveMeshRefinementUtility(self.ProjectParameters,
				                                                                           self.start_time,
				                                                                           self.solver,
				                                                                           self.constitutive_law_utility,
				                                                                           gid_output_utility,
				                                                                           self.conditions_util,
				                                                                           self.problem_path)
			self.activate_AMR = self.AMR_util.Initialize() # check the amr criteria
			"""