/**
 * @file    ArgonSphereEventAction.hh
 * @brief   The class which handles the construction of the argon sphere volume
 * @ingroup ArgonSphereEventAction
 * @author  Nicholas Carrara (nmcarrara@ucdavis.edu),
**/
#pragma once

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "G4AnalysisManager.hh"

#include "ArgonSphereRunAction.hh"

class ArgonSphereEventAction : public G4UserEventAction
{
public:
    ArgonSphereEventAction(ArgonSphereRunAction*);
    ~ArgonSphereEventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
        
private:

};
