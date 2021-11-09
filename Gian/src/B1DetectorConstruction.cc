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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include <G4GDMLParser.hh>// for saving geometry
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  
  // Envelope parameters
  //
  G4double env_sizeXY = 1000000*cm, env_sizeZ = 1000000*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  //     
  // Sphere with LAr 
  //  
  //G4double density = 1.390*g/cm3;
  //G4double a = 39.95*g/mole;
  //G4Material* sphere_mat = new G4Material("liquidArgon",18.,a,density);


  //     
  // Sphere with LAr 
  // Isotopic composition  
  G4Isotope* I36Ar = new G4Isotope("Ar36", 18., 36, 35.967*g/mole); 
  G4Isotope* I38Ar = new G4Isotope("Ar38", 18., 38, 37.962*g/mole); 
  G4Isotope* I40Ar = new G4Isotope("Ar40", 18., 40, 39.962*g/mole); 

  G4Element* Ar_isotopes = new G4Element("Ar_isotopes","Ar",3/*number of isot*/);
  Ar_isotopes->AddIsotope(I36Ar, 0.337*perCent);
  Ar_isotopes->AddIsotope(I38Ar, 0.063 *perCent);
  Ar_isotopes->AddIsotope(I40Ar, 99.6*perCent);
  
  G4double density = 1.406*g/cm3;
  G4double temperature = 85.5 * kelvin;
  G4double pressure = 0.952 * atmosphere;
  G4int ncomponents;
  G4Material* sphere_mat = new G4Material("Ar_isotopes_mat", density, ncomponents=1, kStateLiquid, temperature, pressure);
  sphere_mat->AddElement(Ar_isotopes, 1);

  //
  // Sphere with H2O
  //
  //G4double z, a, density;
  //G4String name, symbol;
  //G4int ncomponents, natoms;
  //a = 1.01*g/mole;
  //G4Element* elH = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);
  //a = 16.00*g/mole;
  //G4Element* elO = new G4Element(name="Oxygen" ,symbol="O" , z= 8., a);
  //density = 1.000*g/cm3;
  //G4Material* sphere_mat = new G4Material(name="Water",density,ncomponents=2);
  //sphere_mat->AddElement(elH, natoms=2);
  //sphere_mat->AddElement(elO, natoms=1);

  //
  // Pb (lead)
  //
  //G4double z, a, density;
  //density = 11.34*g/cm3;
  //a = 207.20*g/mole;
  //G4Material* sphere_mat = new G4Material("Lead" ,z=82., a, density);



  // Gd-doped water
  //Gd2(SO4)3, Gd2S3O12
  //G4Material* fGd = nist->FindOrBuildMaterial("G4_Gd");
  //G4Material* fWater = nist->FindOrBuildMaterial("G4_WATER");
  //G4Material* sphere_mat = new G4Material("GdWater", 1.0*g/cm3, 2);
  //sphere_mat->AddMaterial (fGd, 0.1*perCent);
  //sphere_mat->AddMaterial (fWater, 99.9*perCent);

  //position
  G4ThreeVector pos1 = G4ThreeVector(0, 0, 0);
       
  // Conical section shape       
  G4double sphere_rmina =  0.*cm, sphere_rmaxa = 50000.*cm;
  G4double sphere_SPhi = 0.*deg, sphere_DPhi = 360.*deg;
  G4double sphere_STheta = 0.*deg, sphere_DTheta = 180.*deg;

  G4Sphere* solidSphere =    
    new G4Sphere("Sphere", 
    sphere_rmina, sphere_rmaxa, sphere_SPhi, sphere_DPhi, 
    sphere_STheta, sphere_DTheta);
                      
  G4LogicalVolume* logicSphere =                         
    new G4LogicalVolume(solidSphere,         //its solid
                        sphere_mat,          //its material
                        "Sphere");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos1,                    //at position
                    logicSphere,             //its logical volume
                    "Sphere",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
  // Set Sphere as scoring volume
  //
  fScoringVolume = logicSphere;

  //saving geometry
  //G4GDMLParser Parser;
  //Parser.Write("geometry.gdml",physWorld);



  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
