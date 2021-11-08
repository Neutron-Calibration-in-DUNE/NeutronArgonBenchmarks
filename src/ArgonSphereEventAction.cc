#include "ArgonSphereEventAction.hh"

ArgonSphereEventAction::ArgonSphereEventAction(ArgonSphereRunAction*)
: G4UserEventAction()
{
}

ArgonSphereEventAction::~ArgonSphereEventAction()
{}

void ArgonSphereEventAction::BeginOfEventAction(const G4Event*)
{
}

void ArgonSphereEventAction::EndOfEventAction(const G4Event*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
}
