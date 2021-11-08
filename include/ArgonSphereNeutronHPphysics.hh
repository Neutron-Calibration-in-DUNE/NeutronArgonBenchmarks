/**
 * @file    ArgonSphereNeutronHPphysics.hh
 * @brief   
 * @ingroup ArgonSphereNeutronHPphysics
 * @author  Nicholas Carrara (nmcarrara@ucdavis.edu),
**/
#pragma once

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"
#include "G4HadronElasticProcess.hh"
#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPThermalScatteringData.hh"
#include "G4ParticleHPElastic.hh"
#include "G4ParticleHPThermalScattering.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4ParticleHPCaptureData.hh"
#include "G4ParticleHPCapture.hh"
#include "G4HadronFissionProcess.hh"
#include "G4ParticleHPFissionData.hh"
#include "G4ParticleHPFission.hh"
#include "G4SystemOfUnits.hh"

// local includes
#include "ArgonSphereNeutronHPMessenger.hh"

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class NeutronHPMessenger;


class NeutronHPphysics : public G4VPhysicsConstructor
{
  public:
    NeutronHPphysics(const G4String& name="neutron");
   ~NeutronHPphysics();

  public:
    virtual void ConstructParticle() { };
    virtual void ConstructProcess();
    
  public:
    void SetThermalPhysics(G4bool flag) {fThermal = flag;};  
    
  private:
    G4bool  fThermal;
    NeutronHPMessenger* fNeutronMessenger;  
};
