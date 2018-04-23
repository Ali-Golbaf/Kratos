//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Pooyan Dadvand
//
#include <iostream>

#include "includes/kernel.h"
#include "includes/kratos_version.h"
#include "input_output/logger.h"

namespace Kratos {
Kernel::Kernel() : mpKratosCoreApplication(Kratos::make_shared<KratosApplication>(
                std::string("KratosMultiphysics"))) {
    KRATOS_INFO("Kratos Core Load") << " |  /           |             " << std::endl;
    KRATOS_INFO("Kratos Core Load") << " ' /   __| _` | __|  _ \\   __|" << std::endl;
    KRATOS_INFO("Kratos Core Load") << " . \\  |   (   | |   (   |\\__ \\ " << std::endl;
    KRATOS_INFO("Kratos Core Load") << "_|\\_\\_|  \\__,_|\\__|\\___/ ____/" << std::endl;
    KRATOS_INFO("Kratos Core Load") << "           Multi-Physics " << KRATOS_VERSION << std::endl;

    if (!IsImported("KratosMultiphysics")) {
        mpKratosCoreApplication->RegisterVariables();
        this->ImportApplication(mpKratosCoreApplication);
    }
}

std::unordered_set<std::string> &Kernel::GetApplicationsList() {
  static std::unordered_set<std::string> application_list;
  return application_list;
}

bool Kernel::IsImported(std::string ApplicationName) const {
    if (GetApplicationsList().find(ApplicationName) !=
        GetApplicationsList().end())
        return true;
    else
        return false;
}

void Kernel::ImportApplication(KratosApplication::Pointer pNewApplication) {
    if (IsImported(pNewApplication->Name()))
        KRATOS_ERROR << "importing more than once the application : "
                     << pNewApplication->Name() << std::endl;

    pNewApplication->Register();
    Kernel::GetApplicationsList().insert(pNewApplication->Name());
}

std::string Kernel::Info() const { return "kernel"; }

void Kernel::PrintInfo(std::ostream& rOStream) const { rOStream << "kernel"; }

/// Print object's data.
void Kernel::PrintData(std::ostream& rOStream) const {
    rOStream << "Variables:" << std::endl;
    KratosComponents<VariableData>().PrintData(rOStream);
    rOStream << std::endl;
    rOStream << "Elements:" << std::endl;
    KratosComponents<Element>().PrintData(rOStream);
    rOStream << std::endl;
    rOStream << "Conditions:" << std::endl;
    KratosComponents<Condition>().PrintData(rOStream);

    rOStream << "Loaded applications:" << std::endl;

    auto& application_list = Kernel::GetApplicationsList();
    rOStream << "number of loaded applications = " << application_list.size()
             << std::endl;
    for (auto it = application_list.begin(); it != application_list.end(); ++it)
        rOStream << "  " << *it << std::endl;
}
}
