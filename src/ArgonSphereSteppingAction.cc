#include "ArgonSphereSteppingAction.hh"

ArgonSphereSteppingAction::ArgonSphereSteppingAction(ArgonSphereEventAction *eventAction)
: G4UserSteppingAction()
{
    fEventAction = eventAction;
}

ArgonSphereSteppingAction::~ArgonSphereSteppingAction()
{}

void ArgonSphereSteppingAction::UserSteppingAction(const G4Step *step)
{   
    G4LogicalVolume *volume 
        = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
    const ArgonSphereDetectorConstruction *detectorConstruction 
        = static_cast<const ArgonSphereDetectorConstruction*> 
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
}