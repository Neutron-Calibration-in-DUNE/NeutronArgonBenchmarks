/**
 * @file    ArgonSphereAnalysis.hh
 * @brief   The ArgonSphereAnalysis function for the neutron simulation
 * @ingroup ArgonSphereAnalysis
 * @author  Gian Caceres [gcaceres@ucdavis.edu], Nicholas Carrara [nmcarrara@ucdavis.edu],
**/
#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UserRunAction.hh"
#include "G4UserEventAction.hh"
#include "G4UserSteppingAction.hh"
#include "G4VUserActionInitialization.hh"
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

#include "ArgonSphereGenerator.hh"

class ArgonSphereAnalysisActionInitialization :  public G4VUserActionInitialization 
{
public:
    ArgonSphereAnalysisActionInitialization() : G4VUserActionInitialization() {}
    virtual ~ArgonSphereAnalysisActionInitialization() {}  
    virtual void   BuildForMaster() const;
    virtual void   Build() const;          
};

class ArgonSphereAnalysis : public G4UImessenger
{
public:
    ArgonSphereAnalysis();
    virtual ~ArgonSphereAnalysis();  

    static ArgonSphereAnalysis * instance() {return instance_;}

    void Step(const G4Step*);
    void BeginRun(const G4Run*);
    void EndRun(const G4Run*);
    void BeginEvent(const G4Event*);
    void EndEvent(const G4Event*);
    void Initialize();
    void Save();
    bool checkListOfNeutrons(size_t eventId, int trackId);
 
    // output csv file
    std::string fOutputFilename;
    // number of events
    size_t fNumberOfEvents;
    // map from (event_id,track_id) -> index
    int fNumberOfNeutrons;
    std::map<std::pair<size_t,int>, int> fNeutronMap;
    std::vector<std::vector<int>> fNeutronMapKeys;
    std::vector<std::vector<int>> fListOfNeutrons;

    // tuples for holding event information
    std::vector<size_t> fEvent;
    std::vector<size_t> fParentId;
    std::vector<size_t> fTrackId;
    std::vector<int>    fPdgCode;
    std::vector<double> fBeginT;
    std::vector<double> fBeginX;
    std::vector<double> fBeginY;
    std::vector<double> fBeginZ;
    std::vector<double> fBeginE;
    std::vector<double> fEndT;
    std::vector<double> fEndX;
    std::vector<double> fEndY;
    std::vector<double> fEndZ;
    std::vector<double> fEndE;
    std::vector<double> fTrackLength;
    std::vector<std::string> fBeginProcess;
    std::vector<std::string> fEndProcess;
    std::vector<std::string> fBeginVolume;
    std::vector<std::string> fEndVolume;

private:
    static ArgonSphereAnalysis * instance_;
    G4LogicalVolume* scoringVolume_;
};

