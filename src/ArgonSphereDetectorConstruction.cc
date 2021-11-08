#include "ArgonSphereDetectorConstruction.hh"

ArgonSphereDetectorConstruction::ArgonSphereDetectorConstruction()
: G4VUserDetectorConstruction()
{
    // initialize sphere geometry parameters
    xWorld  = 50001;
    yWorld  = 50001;
    zWorld  = 50001;
    xEnv    = xWorld;
    yEnv    = yWorld;
    zEnv    = zWorld;
    fRadius = 50000;
    // initialize argon parameters
    fTemperature = 85.8 * kelvin;
    fPressure    = 0.952 * atmosphere;

    fAr36MassMol = 35.967*g/mole;
    fAr38MassMol = 37.962*g/mole;
    fAr40MassMol = 39.962*g/mole;

    fAr36Ratio = 0.334;
    fAr38Ratio = 0.063;
    fAr40Ratio = 99.603;

    fAverageDensity = 1.406*g/cm3;
    fNaturalArDensity = 1.3973*g/cm3;

    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
    //  parameters
    fMessenger->DeclareProperty("x_world", xWorld, "half distance along world x direction");
    fMessenger->DeclareProperty("y_world", yWorld, "half distance along world y direction");
    fMessenger->DeclareProperty("z_world", zWorld, "half distance along world z direction");
    fMessenger->DeclareProperty("radius", fRadius, "radius of the sphere in cm");

    DefineMaterials();
}

ArgonSphereDetectorConstruction::~ArgonSphereDetectorConstruction()
{}

void ArgonSphereDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();

    fIAr36 = new G4Isotope(
        "Ar36",        // name
        18.,            // atomic number
        36,             // atomic mass
        fAr36MassMol);  // molar mass
    
    fIAr38 = new G4Isotope(
        "Ar38",        // name
        18.,            // atomic number
        38,             // atomic mass
        fAr38MassMol);  // molar mass

    fIAr40 = new G4Isotope(
        "Ar40",        // name
        18.,            // atomic number
        40,             // atomic mass
        fAr40MassMol);  // molar mass

    // add the isotopes to the definition of Ar
    fArIsotopes = new G4Element("ArIsotopes", "Ar", 3);
    fArIsotopes->AddIsotope(fIAr36, fAr36Ratio*perCent);
    fArIsotopes->AddIsotope(fIAr38, fAr38Ratio*perCent);
    fArIsotopes->AddIsotope(fIAr40, fAr40Ratio*perCent);

    // need now the definition of LAr with the composition
    fLAr = new G4Material(
        "LAr",          // name
        fAverageDensity,// density
        1,              // # of components
        kStateLiquid,   // state
        fTemperature,   // temperature
        fPressure);     // pressure
    
    fLAr->AddElement(fArIsotopes, 1);

    fWorldMat = nist->FindOrBuildMaterial("G4_AIR");
    fEnvMat   = nist->FindOrBuildMaterial("G4_AIR");
}

G4VPhysicalVolume *ArgonSphereDetectorConstruction::Construct()
{
    // create the world volume
    solidWorld  = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    logicWorld  = new G4LogicalVolume(solidWorld, fWorldMat, "logicWorld");
    physWorld   = new G4PVPlacement(
        0, 
        G4ThreeVector(0., 0., 0.),
        logicWorld,
        "physWorld",
        0, 
        false,
        0);
    // create the envelope volume
    solidEnv    = new G4Box("envWorld", xEnv, yEnv, zEnv);
    logicEnv    = new G4LogicalVolume(solidEnv, fEnvMat, "logicEnv");
    physEnv     = new G4PVPlacement(
        0,
        G4ThreeVector(0., 0., 0.),
        logicEnv,
        "physEnv",
        logicWorld,
        false,
        0,
        true);
    // create the argon sphere volume
    solidSphere = new G4Sphere(
        "ArgonSphere", 
        0, 
        fRadius, 
        0, 
        2*CLHEP::pi, 
        0, 
        2*CLHEP::pi);
    
    logicSphere = new G4LogicalVolume(solidSphere, fLAr, "logicSphere");
    physSphere  = new G4PVPlacement(
        0, 
        G4ThreeVector(0., 0., 0.), 
        logicSphere, 
        "physSphere", 
        logicEnv, 
        false, 
        0, 
        true);
    
    fScoringVolume = logicSphere;

    G4Material* material = physSphere->GetLogicalVolume()->GetMaterial();
    std::cout << "LAr material properties:" << std::endl;
    std::cout << "  [name]:     " << material->GetName() << std::endl;
    std::cout << "  [density]:  " << material->GetDensity() << std::endl;
    std::cout << "  [state]:    " << material->GetState() << std::endl;
    std::cout << "  [temp]:     " << material->GetTemperature() << std::endl;
    std::cout << "  [pressure]: " << material->GetPressure() << std::endl;

    return physWorld;
}

void ArgonSphereDetectorConstruction::ConstructSDandField()
{
    // MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

    // if(logicDetector != NULL)
    //     logicDetector->SetSensitiveDetector(sensDet);
}