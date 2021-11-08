/**
 * @file    ArgonSphereGenerator.hh
 * @brief   
 * @ingroup ArgonSphereGenerator
 * @author  Nicholas Carrara (nmcarrara@ucdavis.edu),
**/

#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4IonTable.hh"
#include "G4ChargedGeantino.hh"
#include "globals.hh"

class ArgonSpherePrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    ArgonSpherePrimaryGenerator();
    ~ArgonSpherePrimaryGenerator();
    
    virtual void GeneratePrimaries(G4Event*);

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
    
private:
    G4ParticleGun *fParticleGun;
    G4Box* fEnvelopeBox;
};