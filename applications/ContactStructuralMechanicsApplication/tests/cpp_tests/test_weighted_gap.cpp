//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:   BSD License
//      Kratos default license: kratos/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//

// System includes

// External includes

// Project includes
#include "testing/testing.h"
#include "includes/kratos_flags.h"
#include "includes/mapping_variables.h"
#include "contact_structural_mechanics_application.h"
#include "contact_structural_mechanics_application_variables.h"
#include "includes/gid_io.h"
#include "utilities/variable_utils.h"
#include "processes/simple_mortar_mapper_process.h"

namespace Kratos 
{
    namespace Testing 
    {
        typedef Node<3>                                                    NodeType;

        void GiDIOGapDebug(ModelPart& ThisModelPart)
        {
            GidIO<> gid_io("TEST_WEIGHTED_GAP", GiD_PostBinary, SingleFile, WriteUndeformed,  WriteConditionsOnly);
            const int nl_iter = ThisModelPart.GetProcessInfo()[NL_ITERATION_NUMBER];
            const double label = static_cast<double>(nl_iter);

            gid_io.InitializeMesh(label);
            gid_io.WriteMesh(ThisModelPart.GetMesh());
            gid_io.FinalizeMesh();
            gid_io.InitializeResults(label, ThisModelPart.GetMesh());
            gid_io.WriteNodalFlags(ACTIVE, "ACTIVE", ThisModelPart.Nodes(), label);
            gid_io.WriteNodalFlags(SLAVE, "SLAVE", ThisModelPart.Nodes(), label);
            gid_io.WriteNodalFlags(ISOLATED, "ISOLATED", ThisModelPart.Nodes(), label);
            gid_io.WriteNodalResults(WEIGHTED_GAP, ThisModelPart.Nodes(), label, 0);
            gid_io.WriteNodalResultsNonHistorical(NORMAL_GAP, ThisModelPart.Nodes(), label);
            gid_io.WriteNodalResultsNonHistorical(NODAL_AREA, ThisModelPart.Nodes(), label);
            gid_io.WriteNodalResultsNonHistorical(AUXILIAR_COORDINATES, ThisModelPart.Nodes(), label);
            gid_io.WriteNodalResults(NORMAL, ThisModelPart.Nodes(), label, 0);
        }
        
        /**
         * This method can be used to create a plane/cylinder condition set
         */
        void CreatePlaneCilynderProblem(
            ModelPart& ThisModelPart,
            const std::size_t NumberOfDivisions,
            const double Lenght,
            const double Radius,
            const double Angle,
            const double Slope = 0.0
            )
        {
            Properties::Pointer p_cond_prop = ThisModelPart.pGetProperties(0);
            
            double x, y;
            
            // Creating the base geometry
            std::size_t id_node = 0;
            const double dx = Lenght/static_cast<double>(NumberOfDivisions);
            for (std::size_t i = 0; i < NumberOfDivisions + 1; ++i){
                x = dx * i;
                y = Slope * dx * i;
                id_node++;
                NodeType::Pointer p_node_1 = ThisModelPart.CreateNewNode(id_node, x , y , 0.0);
                p_node_1->Set(SLAVE, true);
                p_node_1->Set(MASTER, false);
                p_node_1->Set(ACTIVE, true);
                id_node++;
                NodeType::Pointer p_node_2 = ThisModelPart.CreateNewNode(id_node, x , y , 1.0);
                p_node_2->Set(SLAVE, true);
                p_node_2->Set(MASTER, false);
                p_node_2->Set(ACTIVE, true);
            }
            
            std::size_t id_cond = 0;
            std::vector<Condition::Pointer> slave_conds;
            for (std::size_t i = 0; i < NumberOfDivisions; i++)
            {
                id_cond++;
                std::vector<NodeType::Pointer> condition_nodes (4);
                condition_nodes[0] = ThisModelPart.pGetNode((2 * i)+1);
                condition_nodes[1] = ThisModelPart.pGetNode((2 * i)+2);
                condition_nodes[2] = ThisModelPart.pGetNode((2 * i)+4);
                condition_nodes[3] = ThisModelPart.pGetNode((2 * i)+3);
                Quadrilateral3D4 <NodeType> quad( condition_nodes);
                
                Condition::Pointer pcond = ThisModelPart.CreateNewCondition("Condition3D4N", id_cond, quad, p_cond_prop);
                pcond->Set(SLAVE, true);
                pcond->Set(MASTER, false);
                slave_conds.push_back(pcond);
            }
            
            // Creating the base circle
            x = 0.0;
            std::size_t count = 0;
            const double dtheta = Angle/static_cast<double>(NumberOfDivisions);
            while (x < Lenght && count * dtheta < Globals::Pi/2.0){
                x = Radius * std::sin(count * dtheta);
                y = Radius * (1.0 - std::cos(count * dtheta));
                id_node++;
                NodeType::Pointer p_node_1 = ThisModelPart.CreateNewNode(id_node, x, y , 0.0);
                p_node_1->Set(SLAVE, false);
                p_node_1->Set(MASTER, true);
                p_node_1->Set(ACTIVE, false);
                id_node++;
                NodeType::Pointer p_node_2 = ThisModelPart.CreateNewNode(id_node, x, y , 1.0);
                p_node_2->Set(SLAVE, false);
                p_node_2->Set(MASTER, true);
                p_node_2->Set(ACTIVE, false);
                count++;
            }
            
            // Adding map
            IndexSet this_set;
            std::vector<Condition::Pointer> master_conds;
            for (std::size_t i = 0; i < count - 1; i++) {
                id_cond++;
                this_set.AddId(id_cond);
                std::vector<NodeType::Pointer> condition_nodes (4);
                condition_nodes[3] = ThisModelPart.pGetNode((2 * (i + NumberOfDivisions + 1)+1));
                condition_nodes[2] = ThisModelPart.pGetNode((2 * (i + NumberOfDivisions + 1)+2));
                condition_nodes[1] = ThisModelPart.pGetNode((2 * (i + NumberOfDivisions + 1)+4));
                condition_nodes[0] = ThisModelPart.pGetNode((2 * (i + NumberOfDivisions + 1)+3));
                Quadrilateral3D4 <NodeType> quad( condition_nodes);
                
                Condition::Pointer pcond = ThisModelPart.CreateNewCondition("Condition3D4N", id_cond, quad, p_cond_prop);
                pcond->Set(SLAVE, false);
                pcond->Set(MASTER, true);
                master_conds.push_back(pcond);
            }
            
            // We compute the normals
            MortarUtilities::ComputeNodesMeanNormalModelPart(ThisModelPart);
            
            // We compute the normal gap to compare with the weighted gap
            // We add the index SetScalarVar
            for(auto& icond : ThisModelPart.Conditions()) {
                if (icond.Is(SLAVE))
                    icond.SetValue(INDEX_SET, Kratos::make_shared<IndexSet>(this_set));
            }
            
            // We set the auxiliar Coordinates
            for(auto& inode : ThisModelPart.Nodes()) {
                if (inode.Is(MASTER))
                    inode.SetValue(AUXILIAR_COORDINATES, inode.Coordinates());
                else
                    inode.SetValue(AUXILIAR_COORDINATES, ZeroVector(3));
            }
            
            // We set the mapper parameters
            Parameters mapping_parameters = Parameters(R"({"inverted_master_slave_pairing": false, "distance_threshold" : 1.0e24})" );
            mapping_parameters["distance_threshold"].SetDouble(ThisModelPart.GetProcessInfo()[DISTANCE_THRESHOLD]);
            typedef SimpleMortarMapperProcess<3, 4, Variable<array_1d<double, 3>>, NonHistorical> MapperType;
            MapperType mapper = MapperType(ThisModelPart, AUXILIAR_COORDINATES, mapping_parameters);
            mapper.Execute();
            
            // We compute now the normal gap and set the nodes under certain threshold as active
            for(auto& inode : ThisModelPart.Nodes()) {      
                if (inode.Is(SLAVE) == true) {
                    // We compute the gap
                    const array_1d<double, 3>& normal = inode.FastGetSolutionStepValue(NORMAL);
                    const array_1d<double, 3>& auxiliar_coordinates = inode.GetValue(AUXILIAR_COORDINATES);
                    const array_1d<double, 3>& components_gap = ( inode.Coordinates() - auxiliar_coordinates);
                    const double gap = inner_prod(components_gap, - normal);
                    inode.SetValue(NORMAL_GAP, gap);
                }
                else
                    inode.SetValue(NORMAL_GAP, 0.0);
            }
            
            // We set the database
            ModelPart& computing_rcontact_model_part = ThisModelPart.GetSubModelPart("ComputingContact"); 
            for (auto& slave_cond : slave_conds) {
                for (auto& master_cond : master_conds) {
                    id_cond++;
                    Condition::Pointer p_auxiliar_condition = computing_rcontact_model_part.CreateNewCondition("ALMFrictionlessMortarContactCondition3D4N", id_cond, slave_cond->GetGeometry(), p_cond_prop);
                    // We set the geometrical values
                    p_auxiliar_condition->SetValue(PAIRED_GEOMETRY, master_cond->pGetGeometry());
                    p_auxiliar_condition->SetValue(NORMAL, slave_cond->GetValue(NORMAL));
                    p_auxiliar_condition->SetValue(PAIRED_NORMAL, master_cond->GetValue(NORMAL));
                    // We activate the condition and initialize it
                    p_auxiliar_condition->Set(ACTIVE, true);
                    p_auxiliar_condition->Initialize();
                }
            }
        }
        
        /** 
        * Checks the correct work of the weighted gap computation
        * Test 1
        */

        KRATOS_TEST_CASE_IN_SUITE(TestWeightedGap1, ContactStructuralApplicationFastSuite)
        {
            ModelPart this_model_part("Contact");
            this_model_part.CreateSubModelPart("ComputingContact");
            this_model_part.SetBufferSize(2);
            
            this_model_part.AddNodalSolutionStepVariable(WEIGHTED_GAP);
            this_model_part.AddNodalSolutionStepVariable(NORMAL);
            
            auto& process_info = this_model_part.GetProcessInfo();
            process_info[STEP] = 1;
            process_info[NL_ITERATION_NUMBER] = 1;
            process_info[DISTANCE_THRESHOLD] = 1.0;
            
            // First we create the nodes
            const std::size_t number_of_divisions = 8;
            const double lenght = 4.0;
            const double radius = 6.0;
            const double angle = Globals::Pi/6;
            const double slope = 0.0;
            
            // We create our problem
            CreatePlaneCilynderProblem(this_model_part, number_of_divisions, lenght, radius, angle, slope);
            
            // We compute the explicit contribution
            VariableUtils().SetScalarVar<Variable<double>>(WEIGHTED_GAP, 0.0, this_model_part.Nodes());
            for (auto& id_cond : this_model_part.GetSubModelPart("ComputingContact").Conditions())
                id_cond.AddExplicitContribution(process_info);
                
            // DEBUG         
//             GiDIOGapDebug(this_model_part);
            
            const double tolerance = 1.0e-4;
            for (auto& inode : this_model_part.Nodes()) {
                if (inode.Is(SLAVE)) {
                    if (std::abs(inode.FastGetSolutionStepValue(WEIGHTED_GAP)) > 0.0) {
                        const double normal_gap = inode.GetValue(NORMAL_GAP);
                        const double weighted_gap_corrected = inode.FastGetSolutionStepValue(WEIGHTED_GAP)/inode.GetValue(NODAL_AREA);
                        KRATOS_CHECK_LESS_EQUAL(std::abs(weighted_gap_corrected - normal_gap)/std::abs(normal_gap), tolerance);
                    }
                }
            }
        }
        
    } // namespace Testing
}  // namespace Kratos.