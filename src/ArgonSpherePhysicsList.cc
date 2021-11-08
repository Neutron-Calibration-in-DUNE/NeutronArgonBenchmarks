#include "ArgonSpherePhysicsList.hh"

ArgonSpherePhysicsList::ArgonSpherePhysicsList()
: G4VModularPhysicsList()
{
  new G4UnitDefinition("millielectronVolt", "meV", "Energy", 1.e-3 * eV);
  new G4UnitDefinition("mm2/g", "mm2/g", "Surface/Mass", mm2 / g);
  new G4UnitDefinition("um2/mg", "um2/mg", "Surface/Mass", um * um / mg);
  // RegisterPhysics (new G4EmStandardPhysics());        ///< want the standard EM physics
  // RegisterPhysics (new G4OpticalPhysics());           ///< as well as the optical physics
  // RegisterPhysics(new G4DecayPhysics());              ///< hadronic decay
  // RegisterPhysics(new G4RadioactiveDecayPhysics());   ///< radioactive decay
  RegisterPhysics(new NeutronHPphysics("neutronHP")); ///< high precision neutron physics
}

ArgonSpherePhysicsList::~ArgonSpherePhysicsList()
{
}

void ArgonSpherePhysicsList::ConstructParticle()
{
    G4BosonConstructor pBosonConstructor;
    pBosonConstructor.ConstructParticle();

    G4LeptonConstructor pLeptonConstructor;
    pLeptonConstructor.ConstructParticle();

    G4MesonConstructor pMesonConstructor;
    pMesonConstructor.ConstructParticle();

    G4BaryonConstructor pBaryonConstructor;
    pBaryonConstructor.ConstructParticle();

    G4IonConstructor pIonConstructor;
    pIonConstructor.ConstructParticle();

    G4ShortLivedConstructor pShortLivedConstructor;
    pShortLivedConstructor.ConstructParticle();
}

void ArgonSpherePhysicsList::SetCuts()
{
  SetCutValue(0 * mm, "proton");
}