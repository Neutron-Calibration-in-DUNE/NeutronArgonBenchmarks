/**
 * @file    ArgonSphereDetectorConstruction.hh
 * @brief   The class which handles the construction of the argon sphere volume
 * @ingroup ArgonSphereDetectorConstruction
 * @author  Nicholas Carrara (nmcarrara@ucdavis.edu),
**/
#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "globals.hh"

class ArgonSphereDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    ArgonSphereDetectorConstruction();
    ~ArgonSphereDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();

    G4LogicalVolume* getLogicalWorldVolume()    { return logicWorld; }
    G4LogicalVolume* getLogicalEnvVolume()      { return logicEnv; }
    G4LogicalVolume* getLogicalSphereVolume()   { return logicSphere; }

    G4VPhysicalVolume* getPhysicalWorldVolume()     { return physWorld; }
    G4VPhysicalVolume* getPhysicalEnvVolume()       { return physEnv; }
    G4VPhysicalVolume* getPhysicalSphereVolume()    { return physSphere; }

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

private:
    // parameters for the geometry of the sphere
    G4double fRadius; 
    G4double xWorld, yWorld, zWorld;
    G4double xEnv, yEnv, zEnv;
    // G4 volumes for the sphere
    G4Box               *solidWorld, *solidEnv;
    G4Sphere            *solidSphere;                 ///< sphere
    G4LogicalVolume     *logicWorld, *logicEnv, *logicSphere;   ///< logical volume for the sphere
    G4VPhysicalVolume   *physWorld,  *physEnv, *physSphere;    ///< physical volume for the sphere

    G4double fTemperature;
    G4double fPressure;

    G4double fAr36Density, fAr38Density, fAr40Density;
    G4double fAr36MassMol, fAr38MassMol, fAr40MassMol;
    G4double fAr36Ratio, fAr38Ratio, fAr40Ratio;

    G4double fAverageMassMol;
    G4double fAverageDensity;
    G4double fNaturalArDensity;

    G4Isotope *fIAr36, *fIAr38, *fIAr40;
    G4Element *fArIsotopes;
    G4Material *fWorldMat, *fEnvMat, *fLAr;

    void DefineMaterials();
    virtual void ConstructSDandField();

    G4GenericMessenger *fMessenger;

protected:
    G4LogicalVolume*  fScoringVolume;
};