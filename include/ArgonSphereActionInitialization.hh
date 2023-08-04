/**
 * @file    ArgonSphereActionInitialization.hh
 * @brief   The ArgonSphereActionInitialization function for the neutron simulation
 * @ingroup ArgonSphereActionInitialization
 * @author  Gian Caceres [gcaceres@ucdavis.edu], Nicholas Carrara [nmcarrara@ucdavis.edu],
**/
#pragma once

#include "G4VUserActionInitialization.hh"

#include "ArgonSphereGenerator.hh"
#include "ArgonSphereRunAction.hh"
#include "ArgonSphereEventAction.hh"
#include "ArgonSphereSteppingAction.hh"

class ArgonSphereActionInitialization : public G4VUserActionInitialization
{
public:
    ArgonSphereActionInitialization();
    ~ArgonSphereActionInitialization();
    
    virtual void BuildForMaster() const;
    virtual void Build() const;
};