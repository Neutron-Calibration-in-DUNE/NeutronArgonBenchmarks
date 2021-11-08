/**
 * @file    ArgonSpherePhysicsList.hh
 * @brief   Physics List for the LAr Neutron simulation
 * @ingroup ArgonSpherePhysicsList
 * @author  Nicholas Carrara (nmcarrara@ucdavis.edu),
**/
#pragma once

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "ArgonSphereNeutronHPphysics.hh"


class ArgonSpherePhysicsList : public G4VModularPhysicsList
{
public:
    ArgonSpherePhysicsList();
    ~ArgonSpherePhysicsList();

    virtual void ConstructParticle();
    virtual void SetCuts();
};