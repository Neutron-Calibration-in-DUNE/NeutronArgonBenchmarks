#include "ArgonSphereAnalysis.hh"

ArgonSphereAnalysis * ArgonSphereAnalysis::instance_ = NULL;

ArgonSphereAnalysis::ArgonSphereAnalysis()
: G4UImessenger()
, scoringVolume_(0)
{ 
    instance_ = this;
}

ArgonSphereAnalysis::~ArgonSphereAnalysis()
{
    instance_ = NULL;
}

void ArgonSphereAnalysis::Initialize()
{
    fNumberOfEvents = 0;
    fNumberOfNeutrons = 0;
    fEvent.clear(); fTrackId.clear(); fParentId.clear(); fPdgCode.clear(); 
    fBeginProcess.clear(); fBeginVolume.clear(); fBeginT.clear(); 
    fBeginX.clear(); fBeginY.clear(); fBeginZ.clear(); fBeginE.clear();
    fEndProcess.clear(); fEndVolume.clear(); fEndT.clear(); 
    fEndX.clear(); fEndY.clear(); fEndZ.clear(); fEndE.clear();
    fTrackLength.clear();
}

void ArgonSphereAnalysis::Save()
{
    //std::ofstream output_file(fOutputFilename); 
    std::ofstream output_file("output.csv");
    if (output_file)
    {
        for (size_t i = 0; i < fEvent.size(); i++)
        {
            output_file << fEvent[i] << ",";
            output_file << fTrackId[i] << ",";
            output_file << fParentId[i] << ",";
            output_file << fPdgCode[i] << ",";
            output_file << fBeginProcess[i] << ",";
            output_file << fBeginVolume[i] << ",";
            output_file << fBeginT[i] << ",";
            output_file << fBeginX[i] << ",";
            output_file << fBeginY[i] << ",";
            output_file << fBeginZ[i] << ",";
            output_file << fBeginE[i] << ",";
            output_file << fEndProcess[i] << ",";
            output_file << fEndVolume[i] << ",";
            output_file << fEndT[i] << ",";
            output_file << fEndX[i] << ",";
            output_file << fEndY[i] << ",";
            output_file << fEndZ[i] << ",";
            output_file << fEndE[i] << ",";
            output_file << fTrackLength[i] << "\n";
        }
    }
    output_file.close();
}

void ArgonSphereAnalysis::Step(const G4Step* step)
{
    // we only care about first and last steps, so check
    // those first
    if (step->IsFirstStepInVolume() || step->GetTrack()->GetTrackStatus() == fStopAndKill)
    {
        // check if particle is a neutron
        // get the particle name
        G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();
        // if its a neutron, 
        if (particleName == "neutron")
        {
            // get logical and physical volumes of the current step
            G4LogicalVolume* logicVolume 
            = step->GetPreStepPoint()->GetTouchableHandle()
                ->GetVolume()->GetLogicalVolume();

            G4VPhysicalVolume* physVolume 
            = step->GetPreStepPoint()->GetTouchableHandle()
            ->GetVolume();
            // collect t=0 information for all neutrons
            if (step->IsFirstStepInVolume() and physVolume->GetName() == "physSphere")
            {
                fNeutronMap[std::make_pair(fNumberOfEvents, step->GetTrack()->GetTrackID())] = fNumberOfNeutrons;
                fNeutronMapKeys.emplace_back(std::vector<int>({fNumberOfEvents,step->GetTrack()->GetTrackID()}));
                fNumberOfNeutrons++;
                fListOfNeutrons[fNumberOfEvents-1].emplace_back(step->GetTrack()->GetTrackID());
                // fill in the beginning information
                fEvent.emplace_back(fNumberOfEvents);
                fTrackId.emplace_back(step->GetTrack()->GetTrackID());
                fParentId.emplace_back(step->GetTrack()->GetParentID());
                fPdgCode.emplace_back(2112);
                fBeginProcess.emplace_back("initStep");
                fBeginVolume.emplace_back(physVolume->GetName());    

                fBeginT.emplace_back(step->GetPreStepPoint()->GetLocalTime());
                fBeginX.emplace_back(step->GetPreStepPoint()->GetPosition().x());
                fBeginY.emplace_back(step->GetPreStepPoint()->GetPosition().y());
                fBeginZ.emplace_back(step->GetPreStepPoint()->GetPosition().z());
                fBeginE.emplace_back(step->GetPreStepPoint()->GetKineticEnergy());
                // we must also emplace back
                // in the ending variables and assign their values later
                fEndT.emplace_back(step->GetPreStepPoint()->GetLocalTime());     
                fEndProcess.emplace_back(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
                fEndVolume.emplace_back(physVolume->GetName());      

                fEndX.emplace_back(step->GetPreStepPoint()->GetPosition().x());
                fEndY.emplace_back(step->GetPreStepPoint()->GetPosition().y());
                fEndZ.emplace_back(step->GetPreStepPoint()->GetPosition().z());
                fEndE.emplace_back(step->GetPreStepPoint()->GetKineticEnergy());

                fTrackLength.emplace_back(step->GetTrack()->GetTrackLength());
            }
            // check if neutron has been counted already

            if (step->GetTrack()->GetTrackStatus() == fStopAndKill and physVolume->GetName() == "physSphere")
            {
                if (checkListOfNeutrons(fNumberOfEvents, step->GetTrack()->GetTrackID()))
                {
                    int neutronIndex = fNeutronMap[std::make_pair(fNumberOfEvents,step->GetTrack()->GetTrackID())];
                    // otherwise we will save the ending information
                    fEndT[neutronIndex] = step->GetPreStepPoint()->GetLocalTime();     
                    fEndProcess[neutronIndex] = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
                    fEndVolume[neutronIndex] = physVolume->GetName();      

                    fEndX[neutronIndex] = step->GetPreStepPoint()->GetPosition().x();
                    fEndY[neutronIndex] = step->GetPreStepPoint()->GetPosition().y();
                    fEndZ[neutronIndex] = step->GetPreStepPoint()->GetPosition().z();
                    fEndE[neutronIndex] = step->GetPreStepPoint()->GetKineticEnergy();

                    fTrackLength[neutronIndex] = step->GetTrack()->GetTrackLength();
                }
            }
        }
    }
}

// check list of neutrons
bool ArgonSphereAnalysis::checkListOfNeutrons(size_t eventId, int trackId)
{
    if (eventId-1 >= fNumberOfEvents) { 
        return false; 
    }
    else {
        for (size_t i = 0; i < fListOfNeutrons[eventId-1].size(); i++) {
            if (fListOfNeutrons[eventId-1][i] == trackId) {
                return true;
            }
        }
    }
    return false;
}

void ArgonSphereAnalysis::BeginEvent(const G4Event*)
{   
    fNumberOfEvents++;
    fListOfNeutrons.emplace_back(std::vector<int>());
}

void ArgonSphereAnalysis::EndEvent(const G4Event*)
{   
}

void ArgonSphereAnalysis::BeginRun(const G4Run*)
{ 
    ArgonSphereAnalysis::instance()->Initialize();
}

void ArgonSphereAnalysis::EndRun(const G4Run* run)
{
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    ArgonSphereAnalysis::instance()->Save();

    const ArgonSpherePrimaryGenerator* generatorAction
    = static_cast<const ArgonSpherePrimaryGenerator*>
        (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

    G4String runCondition;
    if (generatorAction)
    {
        const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
        runCondition += particleGun->GetParticleDefinition()->GetParticleName();
        runCondition += " of ";
        G4double particleEnergy = particleGun->GetParticleEnergy();
        runCondition += G4BestUnit(particleEnergy,"Energy");
    }
         
    G4cout
        << G4endl
        << " The run consists of " << nofEvents << " " << runCondition << G4endl
        << G4endl;
}

class ArgonSphereAnalysisRunAction :  public G4UserRunAction 
{
public:
    virtual void BeginOfRunAction(const G4Run*r) {
        ArgonSphereAnalysis::instance()->BeginRun(r); 
    }
    virtual void EndOfRunAction(const G4Run*r) {
        ArgonSphereAnalysis::instance()->EndRun(r); 
    }  
};

class ArgonSphereAnalysisStepAction :  public G4UserSteppingAction
{
    virtual void UserSteppingAction(const G4Step* step) {
        ArgonSphereAnalysis::instance()->Step(step); 
    }
};
													   
class ArgonSphereAnalysisEventAction :   public G4UserEventAction
{
    virtual void BeginOfEventAction (const G4Event* evt) {
        ArgonSphereAnalysis::instance()->BeginEvent(evt); 
    }
    virtual void EndOfEventAction   (const G4Event* evt) {
        ArgonSphereAnalysis::instance()->EndEvent(evt);   
    }
};

void ArgonSphereAnalysisActionInitialization::BuildForMaster() const
{
    SetUserAction(new ArgonSphereAnalysisRunAction());
}

void ArgonSphereAnalysisActionInitialization::Build() const
{
    SetUserAction(new ArgonSpherePrimaryGenerator);
    SetUserAction(new ArgonSphereAnalysisRunAction);
    SetUserAction(new ArgonSphereAnalysisEventAction);
    SetUserAction(new ArgonSphereAnalysisStepAction);
}  

