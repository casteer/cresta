


#ifndef CosmicEventActionMessenger_h
#define CosmicEventActionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class CosmicEventAction;
class G4UIcmdWithABool;
class G4UIdirectory;
// class G4UIcmdWithAString;
// class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;



class CosmicEventActionMessenger: public G4UImessenger
{
  public:
    CosmicEventActionMessenger(CosmicEventAction*);
   ~CosmicEventActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    CosmicEventAction*               Action;
    G4UIdirectory*               EventDir;
    G4UIcmdWithABool*            delays_onCmd;
    G4UIcmdWithABool*            use_mctruthCmd;
    G4UIcmdWithADoubleAndUnit*   mintimeCmd;
    G4UIcmdWithADoubleAndUnit*   minenergyCmd;

    std::vector<G4UIdirectory*> DetDirs;
    std::vector<G4UIcmdWithADouble*> DetEffCmds;

    G4UIcmdWithADoubleAndUnit*   DetCableSpeedCmd;
    std::vector<G4UIcmdWithADoubleAndUnit*> DetCableLengthCmds;
    std::vector<G4UIcmdWithADoubleAndUnit*> DetMeanDelayCmds;
    std::vector<G4UIcmdWithADoubleAndUnit*> DetStdDelayCmds;


    // G4UIcmdWithAString*          FileCmd; 
    // G4UIcmdWithAString*          InputCmd;
    // G4UIcmdWithoutParameter*     UpdateCmd;
    // std::string* MessInput;
    // G4UIcmdWithADouble*          BeamOnCmd;
};


#endif
