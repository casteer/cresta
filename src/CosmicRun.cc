#include "CosmicRun.hh"


#include "G4SDManager.hh"

CosmicRun::CosmicRun()
{
}

void CosmicRun::RecordEvent(const G4Event* evt)
{
  G4Run::RecordEvent(evt);
}

void CosmicRun::Merge(const G4Run* aRun)
{
  G4Run::Merge(aRun);
}

