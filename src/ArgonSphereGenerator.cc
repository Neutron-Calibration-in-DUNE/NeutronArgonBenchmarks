#include "ArgonSphereGenerator.hh"

ArgonSpherePrimaryGenerator::ArgonSpherePrimaryGenerator()
{
    // add the messenger for the particle gun
    fMessenger = new G4GenericMessenger(this, "/neutron/", "Neutron Source");

    // initial values of parameters
    fX = 0.0; fY = 0.0; fZ = 0.0;

    fMessenger->DeclareProperty("x", fX, "x start position");
    fMessenger->DeclareProperty("y", fY, "y start position");
    fMessenger->DeclareProperty("z", fZ, "z start position");

    fPosition.setX(fX); fPosition.setY(fY); fPosition.setZ(fZ);

    // define the particle gun
    fParticleGun = new G4ParticleGun(1);
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName="neutron";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
    
    G4ThreeVector mom(0.,0.,1.);
    
    fParticleGun->SetParticlePosition(fPosition);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(2.5*MeV);
    fParticleGun->SetParticleDefinition(particle);
}

ArgonSpherePrimaryGenerator::~ArgonSpherePrimaryGenerator()
{
    delete fParticleGun;
    delete fMessenger;
}

void ArgonSpherePrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // G4double envSizeXY = 0;
    // G4double envSizeZ = 0;

    // if (!fEnvelopeBox)
    // {
    // G4LogicalVolume* envLV
    //     = G4LogicalVolumeStore::GetInstance()->GetVolume("logicEnv");
    // if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
    // }

    // if ( fEnvelopeBox ) {
    // envSizeXY = fEnvelopeBox->GetXHalfLength()*2.;
    // envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
    // }  
    // else  {
    // G4ExceptionDescription msg;
    // msg << "Envelope volume of box shape not found.\n"; 
    // msg << "Perhaps you have changed geometry.\n";
    // msg << "The gun will be place at the center.";
    // G4Exception("B1PrimaryGeneratorAction::GeneratePrimaries()",
    //     "MyCode0002",JustWarning,msg);
    // }

    // G4double size = 0.8; 
    // //G4double x0 = size * envSizeXY * (G4UniformRand()-0.5);
    // //G4double y0 = size * envSizeXY * (G4UniformRand()-0.5);
    // //G4double z0 = -0.4 * envSizeZ;
    // G4double x0 = 0.0;
    // G4double y0 = 0.0;
    // G4double z0 = 0.0;

    // fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
    //fPosition.setX(fX); fPosition.setY(fY); fPosition.setZ(fZ);
    //std::cout << "here, " << fPosition.x() << std::endl;
    //fParticleGun->SetParticlePosition(fPosition);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}