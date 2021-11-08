#include "ArgonSphereRunAction.hh"

ArgonSphereRunAction::ArgonSphereRunAction()
: G4UserRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
}

ArgonSphereRunAction::~ArgonSphereRunAction()
{}

void ArgonSphereRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    man->OpenFile("output"+strRunID.str()+".root");
}

void ArgonSphereRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}