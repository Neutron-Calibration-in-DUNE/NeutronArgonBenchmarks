/**
 * @file    ArgonSphereRunAction.hh
 * @brief   
 * @ingroup ArgonSphereRunAction
 * @author  Nicholas Carrara (nmcarrara@ucdavis.edu),
**/

#pragma once

#include "G4UserRunAction.hh"
#include "G4Run.hh"

#include "g4root.hh"

class ArgonSphereRunAction : public G4UserRunAction
{
public:
    ArgonSphereRunAction();
    ~ArgonSphereRunAction();
    
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
};