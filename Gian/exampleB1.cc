//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"

//#include "G4RunManagerFactory.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"

#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include "Analysis.hh"
#include "NeutronHPphysics.hh"


// Addtions for HP neutron 
#include "G4ParticleHPManager.hh"
#include "G4Types.hh"
#include "PhysicsList.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  Analysis que_cosas;


  //#ifdef G4MULTITHREADED
  //G4MTRunManager* runManager = new G4MTRunManager;
  //#else
  G4RunManager* runManager = new G4RunManager;
  //#endif


  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  runManager->SetUserInitialization(new B1DetectorConstruction());


  //HP neutrons physics list
  PhysicsList* phys = new PhysicsList;
  runManager->SetUserInitialization(phys);
    
  // User action initialization
  //runManager->SetUserInitialization(new B1ActionInitialization());
  runManager->SetUserInitialization(new AnalysisActionInit);
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();


  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  UImanager->ApplyCommand("/run/verbose 1");
  UImanager->ApplyCommand("/event/verbose 1");
  UImanager->ApplyCommand("/tracking/verbose 1");

  // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    std::cout<< " entering batch mode" << std::endl;
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    std::cout<< " entering interactive mode" << std::endl;
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }


  // Replaced HP environmental variables with C++ calls
  G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( true );
  G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );
  G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( true );
  G4ParticleHPManager::GetInstance()->SetNeglectDoppler( true );
  G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( true );
  G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( true );
  G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( true );
  //G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( true );
  //G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );
  //G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( true );
  //G4ParticleHPManager::GetInstance()->SetNeglectDoppler( true );
  //G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( true );
  //G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( true );
  //G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( true );

 
  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
