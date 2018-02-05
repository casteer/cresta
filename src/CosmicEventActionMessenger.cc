


#include "CosmicEventActionMessenger.hh"

#include "CosmicEventAction.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"
// #include "G4UIcmdWithAString.hh"
// #include "G4UIcmdWithoutParameter.hh"
// #include "G4UIcmdWithADouble.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4SystemOfUnits.hh"

CosmicEventActionMessenger::CosmicEventActionMessenger(CosmicEventAction* EvtAction) : Action(EvtAction)
{
	EventDir = new G4UIdirectory("/event/");
  	EventDir->SetGuidance("EventAction commands.");

	delays_onCmd = new G4UIcmdWithABool("/event/delays_on",this);
	delays_onCmd->SetGuidance("Use time delay.");
	delays_onCmd->SetParameterName("delays",true);
	delays_onCmd->SetDefaultValue(false);
	delays_onCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	use_mctruthCmd = new G4UIcmdWithABool("/event/use_mctruth",this);
	use_mctruthCmd->SetGuidance("Use MC truth (otherwise, trigger on time).");
	use_mctruthCmd->SetParameterName("MCTruth",true);
	use_mctruthCmd->SetDefaultValue(false);
	// use_mctruthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	mintimeCmd = new G4UIcmdWithADoubleAndUnit("/event/mintime",this);
	mintimeCmd->SetGuidance("Minimum time before trigger.");
	mintimeCmd->SetParameterName("MinTime",true);
	mintimeCmd->SetDefaultValue(3*ns);
	// mintimeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	minenergyCmd = new G4UIcmdWithADoubleAndUnit("/event/minenergy",this);
	minenergyCmd->SetGuidance("Minimum energy deposited to trigger.");
	minenergyCmd->SetParameterName("MinEnergy",true);
	minenergyCmd->SetDefaultValue(0*keV);
	// minenergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

        // Vector of directories for the detectors
        DetCableSpeedCmd = new G4UIcmdWithADoubleAndUnit("/event/minenergy",this);
        DetCableSpeedCmd->SetGuidance("Minimum energy deposited to trigger.");
        DetCableSpeedCmd->SetParameterName("MinEnergy",true);
        DetCableSpeedCmd->SetDefaultValue(200);// mm/ns

        for(int i=0;i<12;i++){

            char buffer[36]; // make sure it's big enough
            snprintf(buffer, sizeof(buffer), "/detector%d/", i);
            DetDirs.push_back(new G4UIdirectory(buffer));

            snprintf(buffer, sizeof(buffer), "Set detector %d properties.", i);
            (DetDirs.back())->SetGuidance(buffer);

            snprintf(buffer, sizeof(buffer), "/detector%d/efficiency", i);
            DetEffCmds.push_back(new G4UIcmdWithADouble(buffer,this));
            (DetEffCmds.back())->AvailableForStates(G4State_PreInit,G4State_Idle);

            snprintf(buffer, sizeof(buffer), "/detector%d/mean_delay", i);
            DetMeanDelayCmds.push_back(new G4UIcmdWithADoubleAndUnit(buffer,this));
            (DetMeanDelayCmds.back())->AvailableForStates(G4State_PreInit,G4State_Idle);

            snprintf(buffer, sizeof(buffer), "/detector%d/cable_length", i);
            DetCableLengthCmds.push_back(new G4UIcmdWithADoubleAndUnit(buffer,this));
            (DetCableLengthCmds.back())->AvailableForStates(G4State_PreInit,G4State_Idle);

            snprintf(buffer, sizeof(buffer), "/detector%d/std_delay", i);
            DetStdDelayCmds.push_back(new G4UIcmdWithADoubleAndUnit(buffer,this));
            (DetStdDelayCmds.back())->AvailableForStates(G4State_PreInit,G4State_Idle);

        }

}

CosmicEventActionMessenger::~CosmicEventActionMessenger()
{
  	delete EventDir;
	delete delays_onCmd;
	delete use_mctruthCmd;
	delete mintimeCmd;
	delete minenergyCmd;
}

void CosmicEventActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 

    for(int i=0;i<12;i++){



        if( command == DetCableLengthCmds.at(i) )
        {
            G4double l = (DetCableLengthCmds.at(i))->GetNewDoubleValue(newValue);
            Action->SetCableLength( l, i);
        }
        if( command == DetEffCmds.at(i) )
        {
            G4double eff = (DetEffCmds.at(i))->GetNewDoubleValue(newValue);
            Action->SetEfficiency( eff, i);
        }
        if( command == DetMeanDelayCmds.at(i) )
        {
            G4double mean_delay = (DetMeanDelayCmds.at(i))->GetNewDoubleValue(newValue);
            Action->SetMeanDelay( mean_delay, i);
        }
        if( command == DetStdDelayCmds.at(i) )
        {
            G4double stddelay = (DetStdDelayCmds.at(i))->GetNewDoubleValue(newValue);
            Action->SetStdDelay( stddelay, i);
        }
    }


    if( command == DetCableSpeedCmd )
    {
            // Action->SetDelaysBool(G4UIcommand::ConvertToBool(newValue));
            Action->SetCableSpeed(DetCableSpeedCmd->GetNewDoubleValue(newValue));
    }
    if( command == delays_onCmd )
    {
            // Action->SetDelaysBool(G4UIcommand::ConvertToBool(newValue));
            Action->SetDelaysBool(delays_onCmd->GetNewBoolValue(newValue));
    }

	if( command == use_mctruthCmd )
	{ 
		Action->SetMCTruthBool(G4UIcommand::ConvertToBool(newValue));
	}

	if( command == mintimeCmd )
	{
		Action->SetMinTime(G4UIcommand::ConvertToDouble(newValue));
	}

	if( command == minenergyCmd )
	{
		Action->SetMinEnergy(G4UIcommand::ConvertToDouble(newValue));
	}

}
