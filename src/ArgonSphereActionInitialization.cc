#include "ArgonSphereActionInitialization.hh"

ArgonSphereActionInitialization::ArgonSphereActionInitialization()
{}

ArgonSphereActionInitialization::~ArgonSphereActionInitialization()
{}

void ArgonSphereActionInitialization::BuildForMaster() const
{
    ArgonSphereRunAction *runAction = new ArgonSphereRunAction();
    SetUserAction(runAction);
}

void ArgonSphereActionInitialization::Build() const
{
    ArgonSpherePrimaryGenerator *generator = new ArgonSpherePrimaryGenerator();
    SetUserAction(generator);
    
    ArgonSphereRunAction *runAction = new ArgonSphereRunAction();
    SetUserAction(runAction);
    
    ArgonSphereEventAction *eventAction = new ArgonSphereEventAction(runAction);
    SetUserAction(eventAction);
    
    ArgonSphereSteppingAction *steppingAction = new ArgonSphereSteppingAction(eventAction);
    SetUserAction(steppingAction);
}