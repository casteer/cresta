#ifndef COSMIC_RUN_HH
#define COSMIC_RUN_HH

#include "G4Run.hh"

class CosmicRun: public G4Run
{
 public:
  CosmicRun();
  virtual ~CosmicRun(){};

  virtual void RecordEvent(const G4Event*);
  virtual void Merge(const G4Run*);

private:
  G4int fEDepScoreID;
  G4double fEDep;

};

#endif // COSMIC_RUN_HH
