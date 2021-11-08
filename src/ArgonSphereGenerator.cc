#include "ArgonSphereGenerator.hh"

ArgonSpherePrimaryGenerator::ArgonSpherePrimaryGenerator()
: G4VUserPrimaryGeneratorAction()
, fEnvelopeBox(0)
{
    fParticleGun = new G4ParticleGun(1);
    
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName="neutron";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
    
    G4ThreeVector pos(0.,0.,0.);
    G4ThreeVector mom(0.,0.,1.);
    
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleMomentum(2.5*MeV);
    fParticleGun->SetParticleDefinition(particle);
}

ArgonSpherePrimaryGenerator::~ArgonSpherePrimaryGenerator()
{
    delete fParticleGun;
}

void ArgonSpherePrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4double envSizeXY = 0;
    G4double envSizeZ = 0;

    if (!fEnvelopeBox)
    {
    G4LogicalVolume* envLV
        = G4LogicalVolumeStore::GetInstance()->GetVolume("logicEnv");
    if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
    }

    if ( fEnvelopeBox ) {
    envSizeXY = fEnvelopeBox->GetXHalfLength()*2.;
    envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
    }  
    else  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("B1PrimaryGeneratorAction::GeneratePrimaries()",
        "MyCode0002",JustWarning,msg);
    }

    G4double size = 0.8; 
    //G4double x0 = size * envSizeXY * (G4UniformRand()-0.5);
    //G4double y0 = size * envSizeXY * (G4UniformRand()-0.5);
    //G4double z0 = -0.4 * envSizeZ;
    G4double x0 = 0.0;
    G4double y0 = 0.0;
    G4double z0 = 0.0;

    fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

    fParticleGun->GeneratePrimaryVertex(anEvent);
}