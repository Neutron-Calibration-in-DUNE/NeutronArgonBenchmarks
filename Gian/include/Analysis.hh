#ifndef Analysis_h
#define Analysis_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UserRunAction.hh"
#include "G4UserEventAction.hh"
#include "G4UserSteppingAction.hh"
#include "G4VUserActionInitialization.hh"

#define CELL_GRID_SIZE 10

class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4Step;
class G4Event;
class G4Run;
class G4UIdirectory;
class TFile;
class TTree;
class TH1D;
class G4LogicalVolume;

class AnalysisActionInit :  public G4VUserActionInitialization {
public:
  AnalysisActionInit() : G4VUserActionInitialization() {}
  virtual ~AnalysisActionInit() {}  
  virtual void   BuildForMaster() const;
  virtual void   Build() const;          
};

class Analysis: 
  public G4UImessenger
{
  public:
  Analysis();
  virtual ~Analysis();  
  
  static Analysis * instance(){return instance_;}

  void   Step(const G4Step*);
  void   BeginRun(const G4Run*);
  void   EndRun(const G4Run*);
  void   BeginEvent(const G4Event*);
  void   EndEvent(const G4Event*);
  void   Book();
  void   Save();
  void   FillNtuple();      

  // statistics:
  struct stats {
    void reset();

    std::vector <float> i_posx; 
    std::vector <float> i_posy; 
    std::vector <float> i_posz; 
    std::vector <float> i_t; 
    std::vector <float> i_tk_len; 
    std::vector<int> i_proc ;
    std::vector<int> i_vol ;
    std::vector<int> i_id ;

    std::vector <float> f_posx; 
    std::vector <float> f_posy; 
    std::vector <float> f_posz; 
    std::vector <float> f_t; 
    std::vector <float> f_tk_len; 
    std::vector <float> f_kinE ;
    std::vector <float> f_cap_kinE ;
    std::vector<int> f_proc ;
    std::vector<int> f_vol ;
    std::vector<int> f_id ;

  };
  stats run_;
  stats evt_;
  int edep_evt;
  int adep_evt;



  // track progress:
  int events;


  // set config parameters:
  //virtual void SetNewValue(G4UIcommand * command,G4String arg);

private:
  //G4UIdirectory*        analysisDir_; 

  static Analysis * instance_;
  TFile * file_;
  TTree * ntuple_;
  G4LogicalVolume* scoringVolume_;
};


#endif

    
