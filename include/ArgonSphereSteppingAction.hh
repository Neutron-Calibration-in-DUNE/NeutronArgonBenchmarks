/**
 * @file    ArgonSphereSteppingAction.hh
 * @brief   
 * @ingroup ArgonSphereSteppingAction
 * @author  Gian Caceres [gcaceres@ucdavis.edu], Nicholas Carrara [nmcarrara@ucdavis.edu],
**/
#pragma once

#include "G4UserSteppingAction.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"

#include "ArgonSphereDetectorConstruction.hh"
#include "ArgonSphereEventAction.hh"

class ArgonSphereSteppingAction : public G4UserSteppingAction
{
public:
    ArgonSphereSteppingAction(ArgonSphereEventAction* eventAction);
    ~ArgonSphereSteppingAction();
    
    virtual void UserSteppingAction(const G4Step*);
    
private:
    ArgonSphereEventAction *fEventAction;
};