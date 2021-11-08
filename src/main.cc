/**
 * @file    main.cc
 * @brief   The main function for the neutron simulation
 * @ingroup main
 * @author  Gian Caceres [gcaceres@ucdavis.edu], Nicholas Carrara [nmcarrara@ucdavis.edu],
**/
#include <iostream>
#include <vector>
#include <string>

// Geant4 includes 
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4PhysListFactory.hh"
// Neutron HP
#include "G4ParticleHPManager.hh"
#include "G4Types.hh"

#include "ArgonSphereDetectorConstruction.hh"
#include "ArgonSpherePhysicsList.hh"
#include "ArgonSphereActionInitialization.hh"

int main(int argc, char** argv)
{
    // create a user session
    G4UIExecutive* ui = 0;

    // #ifdef G4MULTITHREADED
    //   G4MTRunManager* runManager = new G4MTRunManager;
    // #else
    //   G4RunManager* runManager = new G4RunManager;
    // #endif
    G4RunManager* runManager = new G4RunManager;
    // apply the detector, physics list and initialization
    runManager->SetUserInitialization(new ArgonSphereDetectorConstruction());
    runManager->SetUserInitialization(new ArgonSpherePhysicsList());
    runManager->SetUserInitialization(new ArgonSphereActionInitialization());
    
    // Replaced HP environmental variables with C++ calls                                                                                     
    G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( true );
    G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );
    G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( true );
    G4ParticleHPManager::GetInstance()->SetNeglectDoppler( true );
    G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( true );
    G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( true );
    G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( true );

    runManager->Initialize();

    // print out available physics lists
    G4PhysListFactory *physListFactory = new G4PhysListFactory();
    const std::vector<G4String> physicsLists = physListFactory->AvailablePhysLists();
    std::cout << "Enabled Physics Lists:" << std::endl;
    for(size_t i = 0; i < physicsLists.size(); i++)
    {
        std::cout << "\t[" << i << "]: " << physicsLists[i] << std::endl;
    }
    // print out all processes for neutrons
    G4ParticleDefinition* neutron = G4Neutron::Neutron();
    G4ProcessManager* pManager = neutron->GetProcessManager();
    G4ProcessVector* processes = pManager->GetProcessList();
    std::cout << "Enabled Neutron HP Physics Processes:" << std::endl;
    for(size_t i = 0; i < processes->size(); i++)
    {
        std::cout << "\t[" << i << "]: " << (*processes)[i]->GetProcessName() << std::endl;
    }
    // print out physical volume properties
    

    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if(ui)
    {
        UImanager->ApplyCommand("/control/execute vis.mac");
        UImanager->ApplyCommand("/run/verbose 1");
        UImanager->ApplyCommand("/event/verbose 1");
        UImanager->ApplyCommand("/tracking/verbose 1");
        ui->SessionStart();
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    return 0;
}