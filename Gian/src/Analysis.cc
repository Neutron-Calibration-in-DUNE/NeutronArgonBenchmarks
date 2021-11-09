#include <iostream>
#include <algorithm>

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>

#include "G4UIdirectory.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4UIdirectory.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"

#include "PDDetectorConstruction.hh"
//#include "PDPrimaryGeneratorAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "Analysis.hh"

using namespace std;


Analysis * Analysis::instance_ = NULL;

void Analysis::stats::reset(){

  i_posx.clear(); 
  i_posy.clear(); 
  i_posz.clear(); 
  i_t.clear(); 
  i_proc.clear();
  i_vol.clear(); 
  i_id.clear(); 
  i_tk_len.clear(); 

  f_posx.clear(); 
  f_posy.clear(); 
  f_posz.clear(); 
  f_t.clear(); 
  f_proc.clear();
  f_vol.clear(); 
  f_id.clear(); 
  f_tk_len.clear(); 
  f_cap_kinE.clear(); 
  f_kinE.clear(); 
}

Analysis::Analysis()
: 
  G4UImessenger(),
  //analysisDir_(0),
  file_(0),
  ntuple_(0),
  scoringVolume_(0)
{ 

  //analysisDir_ = new G4UIdirectory("/analysis/");
  //analysisDir_->SetGuidance("Analysis commands");

  instance_ = this;
}

Analysis::~Analysis()
{
  instance_ = NULL;
  //delete analysisDir_;
  if (file_) delete file_;
}



void Analysis::Book()
{ 
  // Creating a tree container to handle histograms and ntuples.
  // This tree is associated to an output file.
  //
  G4String fileName = "test.root";

  cout<< " before creation line of the file " << endl;
  file_ = new TFile(fileName,"RECREATE");
  cout<< " after creation line of the file " << endl;
  if (! file_) {
    G4cout << " Analysis::Book :" 
           << " problem creating the ROOT TFile "
           << G4endl;
    return;
  }
  
  // create ntuple
  ntuple_ = new TTree("neutrons_tracks", "");

  ntuple_->Branch("i_proc",&evt_.i_proc);
  ntuple_->Branch("i_vol", &evt_.i_vol);
  ntuple_->Branch("i_id",  &evt_.i_id);
  ntuple_->Branch("i_posx", &evt_.i_posx);
  ntuple_->Branch("i_posy", &evt_.i_posy);
  ntuple_->Branch("i_posz", &evt_.i_posz);
  ntuple_->Branch("i_t", &evt_.i_t);
  ntuple_->Branch("i_tk_len", &evt_.i_tk_len);

  ntuple_->Branch("f_proc",&evt_.f_proc);
  ntuple_->Branch("f_vol", &evt_.f_vol);
  ntuple_->Branch("f_id",  &evt_.f_id);
  ntuple_->Branch("f_posx", &evt_.f_posx);
  ntuple_->Branch("f_posy", &evt_.f_posy);
  ntuple_->Branch("f_posz", &evt_.f_posz);
  ntuple_->Branch("f_t", &evt_.f_t);
  ntuple_->Branch("f_tk_len", &evt_.f_tk_len);
  ntuple_->Branch("f_kinE", &evt_.f_kinE);
  ntuple_->Branch("f_cap_kinE", &evt_.f_cap_kinE);

  G4cout << "\n----> Output file is open in " << fileName << G4endl;
}


void Analysis::Save()
{ 
  if (! file_) return;
   
  cout<< " file was created and filled"<< endl;
  file_->Write();       // Writing the histograms to the file
  cout<< " file was written"<< endl;
  file_->Close();       // and closing the tree (and the file)
  cout<< " file was closed"<< endl;
  
  G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
}

void Analysis::FillNtuple(){
  if (!ntuple_) return;
  ntuple_->Fill();
}

void Analysis::Step(const G4Step* step)
{
  cout<<"Start of Step " << endl;

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  G4VPhysicalVolume* pv 
    = step->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume();


  int init_proc_name, fin_proc_name;  
  vector <float> fin_pos ;
  int init_vol, fin_vol;
  int init_id, fin_id;
  float init_t, fin_t ;
  float init_tk_len, fin_tk_len ;
  float x1; 
  float y1;
  float z1;
  float kinE; 
  float fin_kinE; 

  string vol;
  string proc_name;

  G4String name = step->GetTrack()->GetDefinition()->GetParticleName();

  if (name == "neutron" ){

    //
    //First step at Time zero
    //
    if (pv->GetName() == "Sphere" && volume->GetName() =="Sphere" && step->IsFirstStepInVolume() )
    {
      init_t = step->GetPreStepPoint()->GetLocalTime();
      init_id = step->GetTrack()->GetParentID();
      init_tk_len = step->GetTrack()->GetTrackLength();

      cout<<"ti check = "<<init_t << endl;

      proc_name = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
      vol = pv->GetName();

      if (vol == "Sphere") init_vol = 0 ; 
      if (vol != "Sphere") init_vol = 1 ; 

      // initStep process can't be retrieved with step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
      init_proc_name = 0 ; 

      x1 = step->GetPreStepPoint()->GetPosition().x() ;
      y1 = step->GetPreStepPoint()->GetPosition().y() ;
      z1 = step->GetPreStepPoint()->GetPosition().z() ;

      (evt_.i_posx).push_back(x1);
      (evt_.i_posy).push_back(y1);
      (evt_.i_posz).push_back(z1);
      (evt_.i_t).push_back(init_t);

      (evt_.i_proc).push_back(init_proc_name);
      (evt_.i_vol).push_back(init_vol);
      (evt_.i_id).push_back(init_id);
      (evt_.i_tk_len).push_back(init_tk_len);

	//
	//checking some magnitudes 
	//

      //cout<<"prestep point = "<<x1<<", " <<y1 << ", " <<z1 << endl;
      //cout<<"i pos = "<<evt_.i_posx[0]<<", " <<evt_.i_posy[0] << ", " <<evt_.i_posz[0] << " " << evt_.i_t[0]<<endl;
      //if(init_id == 1)
      //cout<<"i pos in 1 = "<<evt_.i_posx[1]<<", " <<evt_.i_posy[1] << ", " <<evt_.i_posz[1] << endl;
    }


    // the information of the last step of the track (status fStopAndKill) is retrieved regardless of its location 

    G4Track * theTrack = step->GetTrack();
    if (theTrack->GetTrackStatus()==fStopAndKill)
    {
      fin_tk_len = step->GetTrack()->GetTrackLength();
      fin_t = step->GetPostStepPoint()->GetLocalTime();
      fin_id = step->GetTrack()->GetParentID();
      fin_kinE = step->GetPreStepPoint()->GetKineticEnergy(); 
      kinE     = step->GetPostStepPoint()->GetKineticEnergy(); 
      cout<<"fin_t check = "<<fin_t << endl;

      proc_name = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
      vol = pv->GetName();

      if (vol == "Sphere") fin_vol = 0 ; 
      if (vol != "Sphere" ) fin_vol = 1 ; 

      //initStep
      if (proc_name == "nCapture") fin_proc_name = 1 ; 
      if (proc_name == "neutronInelastic") fin_proc_name = 2 ; 
      if (proc_name != "nCapture" && proc_name !="neutronInelastic")
	{ fin_proc_name = 3 ; 
      	cout<<"new process = "<< proc_name << endl;
	}

      x1 = step->GetPostStepPoint()->GetPosition().x() ;
      y1 = step->GetPostStepPoint()->GetPosition().y() ;
      z1 = step->GetPostStepPoint()->GetPosition().z() ;

      (evt_.f_posx).push_back(x1);
      (evt_.f_posy).push_back(y1);
      (evt_.f_posz).push_back(z1);
      (evt_.f_t).push_back(fin_t);

      (evt_.f_proc).push_back(fin_proc_name);
      (evt_.f_vol).push_back(fin_vol);
      (evt_.f_id).push_back(fin_id);
      (evt_.f_tk_len).push_back(fin_tk_len);
      (evt_.f_cap_kinE).push_back(fin_kinE);
      (evt_.f_kinE).push_back(kinE);

      cout<<"f_cap_kinE = "<< fin_kinE << endl;
      cout<<"f_kinE = "<< kinE << endl;
      cout<<"f_proc = "<< fin_proc_name << endl;
      cout<<"status of the prestep = " <<  step->GetPreStepPoint()->GetStepStatus()<< endl;
      cout<<"status of the poststep = " <<  step->GetPostStepPoint()->GetStepStatus()<< endl;

	//cheking the available statuses of the steps 	
	/*
	if(step->GetPreStepPoint()->GetStepStatus()== fWorldBoundary) cout<<"fWorldBoundary ="<< step->GetPreStepPoint()->GetStepStatus() << endl;
	if(step->GetPreStepPoint()->GetStepStatus()== fGeomBoundary)  cout<<"fGeomBoundary ="<< step->GetPreStepPoint()->GetStepStatus() << endl;

	if(step->GetPreStepPoint()->GetStepStatus()== fAtRestDoItProc)  cout<<"fAtRestDoItProc ="<< step->GetPreStepPoint()->GetStepStatus() << endl;
	if(step->GetPreStepPoint()->GetStepStatus()== fAlongStepDoItProc)  cout<<"fAlongStepDoItProc ="<< step->GetPreStepPoint()->GetStepStatus() << endl;
	if(step->GetPreStepPoint()->GetStepStatus()== fPostStepDoItProc)  cout<<"fPostStepDoItProc ="<< step->GetPreStepPoint()->GetStepStatus() << endl;
	if(step->GetPreStepPoint()->GetStepStatus()== fUserDefinedLimit)  cout<<"fUserDefinedLimit ="<< step->GetPreStepPoint()->GetStepStatus() << endl;
	if(step->GetPreStepPoint()->GetStepStatus()== fExclusivelyForcedProc)  cout<<"fExclusivelyForcedProc ="<< step->GetPreStepPoint()->GetStepStatus() << endl;
	if(step->GetPreStepPoint()->GetStepStatus()== fUndefined)  cout<<"fUndefined ="<< step->GetPreStepPoint()->GetStepStatus() << endl;

	if(step->GetPostStepPoint()->GetStepStatus()== fWorldBoundary) cout<<"fWorldBoundary ="<< step->GetPostStepPoint()->GetStepStatus() << endl;
	if(step->GetPostStepPoint()->GetStepStatus()== fGeomBoundary)  cout<<"fGeomBoundary ="<< step->GetPostStepPoint()->GetStepStatus() << endl;
	if(step->GetPostStepPoint()->GetStepStatus()== fAtRestDoItProc)  cout<<"fAtRestDoItProc ="<< step->GetPostStepPoint()->GetStepStatus() << endl;
	if(step->GetPostStepPoint()->GetStepStatus()== fAlongStepDoItProc)  cout<<"fAlongStepDoItProc ="<< step->GetPostStepPoint()->GetStepStatus() << endl;
	if(step->GetPostStepPoint()->GetStepStatus()== fPostStepDoItProc)  cout<<"fPostStepDoItProc ="<< step->GetPostStepPoint()->GetStepStatus() << endl;
	if(step->GetPostStepPoint()->GetStepStatus()== fUserDefinedLimit)  cout<<"fUserDefinedLimit ="<< step->GetPostStepPoint()->GetStepStatus() << endl;
	if(step->GetPostStepPoint()->GetStepStatus()== fExclusivelyForcedProc)  cout<<"fExclusivelyForcedProc ="<< step->GetPostStepPoint()->GetStepStatus() << endl;
	if(step->GetPostStepPoint()->GetStepStatus()== fUndefined)  cout<<"fUndefined ="<< step->GetPostStepPoint()->GetStepStatus() << endl;
	*/

	//cheking the available statuses of the tracks 	
	/*
	G4Track * theTrack = step->GetTrack();
	if(theTrack->GetTrackStatus()==fAlive) cout<< "fAlive = "<<  theTrack->GetTrackStatus()<< endl;
	if(theTrack->GetTrackStatus()==fStopButAlive) cout<< "fStopButAlive ="<<  theTrack->GetTrackStatus()<< endl;
	if(theTrack->GetTrackStatus()==fStopAndKill) cout<< "fStopAndKill ="<<  theTrack->GetTrackStatus()<< endl;
	if(theTrack->GetTrackStatus()==fKillTrackAndSecondaries) cout<< "fKillTrackAndSecondaries ="<<  theTrack->GetTrackStatus()<< endl;
	if(theTrack->GetTrackStatus()==fSuspend) cout<< "fSuspend ="<<  theTrack->GetTrackStatus()<< endl;
	if(theTrack->GetTrackStatus()==fPostponeToNextEvent) cout<< "fPostponeToNextEvent ="<<  theTrack->GetTrackStatus()<< endl;
	*/
	
	//
	//cheking some magnitudes 
	//
      cout<<"prestep point = "<<x1<<", " <<y1 << ", " <<z1 << endl;
      cout<<"i pos = "<<evt_.i_posx[0]<<", " <<evt_.i_posy[0] << ", " <<evt_.i_posz[0] << " " << evt_.i_t[0]<<endl;
      if(init_id == 1)
      cout<<"i pos in 1 = "<<evt_.i_posx[1]<<", " <<evt_.i_posy[1] << ", " <<evt_.i_posz[1] << endl;
    }

    G4cout
      << "INFO:  A primary particle of type "
      << name
      << G4endl;

}

  cout<<"End of Step " << endl;
}

void Analysis::BeginEvent(const G4Event*)
{   
 
  cout<<"Start of BeginEvent " << endl;
  events++;
  if (!(events  % 10000)) cout << "PROGRESS:  event count " << events << "\n";
  evt_.reset();

  cout<<"End of BeginEvent " << endl;
}

void Analysis::EndEvent(const G4Event*)
{   
  cout<<"Start of EndEvent " << endl;
    FillNtuple();
  cout<<"End of EndEvent " << endl;
}

void Analysis::BeginRun(const G4Run*)
{ 
  //inform the runManager to save random number seed
  cout<<"Start of BeginRun " << endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  Analysis::instance()->Book();
  run_.reset();
  events = 0;
  cout<<"End of BeginRun " << endl;
}

void Analysis::EndRun(const G4Run* run)
{
  cout<<"Start of EndRun " << endl;
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
 
  cout << "Number of events  = " << nofEvents << endl;
  Analysis::instance()->Save();

  const B1PrimaryGeneratorAction* generatorAction
   = static_cast<const B1PrimaryGeneratorAction*>
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

  cout<<"End of EndRun " << endl;
}


// G4 zealously deletes our classes for us, so need this bit of misdirection for a clean exits:

class AnalysisRunAction :  public G4UserRunAction {
public:
  virtual void   BeginOfRunAction(const G4Run*r) {Analysis::instance()->BeginRun(r); }
  virtual void   EndOfRunAction(const G4Run*r) {Analysis::instance()->EndRun(r); }  
};

class AnalysisStepAction :  public G4UserSteppingAction{
  virtual void   UserSteppingAction(const G4Step* step) {Analysis::instance()->Step(step); }
};
													   
class AnalysisEventAction :   public G4UserEventAction{
  virtual void   BeginOfEventAction (const G4Event* evt) {Analysis::instance()->BeginEvent(evt); }
  virtual void   EndOfEventAction   (const G4Event* evt) {Analysis::instance()->EndEvent(evt);   }
};

void AnalysisActionInit::BuildForMaster() const
{
  SetUserAction(new AnalysisRunAction());
}

void AnalysisActionInit::Build() const
{
  SetUserAction(new B1PrimaryGeneratorAction);
  SetUserAction(new AnalysisRunAction);
  SetUserAction(new AnalysisEventAction);
  SetUserAction(new AnalysisStepAction);
}  

