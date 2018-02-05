//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: PrimaryGeneratorMessenger.cc,v 1.8 2002/12/16 16:37:27 maire Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction_Shukla.hh"
#include "G4UIdirectory.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction_Shukla* Gun)
  :Action(Gun)
{

    ShuklaDir = new G4UIdirectory("/Shukla/");    
    ShuklaDir->SetGuidance("Parameters of Shukla CR Spectrum");

    Shukla_E0 = new G4UIcmdWithADoubleAndUnit("/Shukla/E0", this);
    Shukla_E0->SetGuidance("Shukla E0 parameter");
    Shukla_E0->SetParameterName("E0",false);
    Shukla_E0->SetDefaultValue(4.28*GeV);
    Shukla_E0->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);

    Shukla_Epsilon = new G4UIcmdWithADoubleAndUnit("/Shukla/Epsilon", this);
    Shukla_Epsilon->SetGuidance("Shukla Epsilon parameter");
    Shukla_Epsilon->SetParameterName("Epsilon",false);
    Shukla_Epsilon->SetDefaultValue(854.0*GeV);
    Shukla_Epsilon->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);

    SourceSize = new G4UIcmdWith3VectorAndUnit("/Shukla/SourceSize", this);
    SourceSize->SetGuidance("Set the size of the source plane, one dimension should be much smaller (i.e. ~1mm) than the others (of order of metres)");
    SourceSize->SetParameterName("Half-length X ","Half-length Y ","Half-length Z ",false);
    SourceSize->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);

    SourcePosition = new G4UIcmdWith3VectorAndUnit("/Shukla/SourcePosition", this);
    SourcePosition->SetGuidance("Set the centre position of the source plane");
    SourcePosition->SetParameterName("X ","Y ","Z ",false);
    SourcePosition->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);



    Shukla_energy_threshold = new G4UIcmdWithADoubleAndUnit("/Shukla/EnergyThreshold", this);
    Shukla_energy_threshold->SetGuidance("Ignores events generated less than this");
    Shukla_energy_threshold->SetParameterName("Energy threshold",false);
    Shukla_energy_threshold->SetDefaultValue(0.0*GeV);
    Shukla_energy_threshold->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);

    Shukla_target_boxes_positions = new G4UIcmdWith3VectorAndUnit("/Shukla/TargetBoxPosition", this);
    Shukla_target_boxes_positions->SetGuidance("Adds a target box at this position (needs to be paired with a /Shukla/TargetBoxSize command)");
    Shukla_target_boxes_positions->SetParameterName("Position X ","Position Y ","Position Z ",false);
    Shukla_target_boxes_positions->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);

    Shukla_target_boxes_sizes = new G4UIcmdWith3VectorAndUnit("/Shukla/TargetBoxSize", this);
    Shukla_target_boxes_sizes->SetGuidance("Adds a target box with this size (needs to be paired with a /Shukla/TargetBoxPosition command)");
    Shukla_target_boxes_sizes->SetParameterName("Position X ","Position Y ","Position Z ",false);
    Shukla_target_boxes_sizes->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);

    Shukla_clear_target_boxes = new G4UIcmdWithoutParameter("/Shukla/ClearTargetBoxes", this);
    Shukla_clear_target_boxes->SetGuidance("Clears set target boxes");
    Shukla_clear_target_boxes->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);


    MessInput = new std::string;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
    delete ShuklaDir;
    delete Shukla_E0;
    delete Shukla_Epsilon;
    delete Shukla_source_sx;
    delete Shukla_source_sy;
    delete Shukla_target_boxes_sizes;
    delete Shukla_target_boxes_positions;
    delete Shukla_system_offset;
    delete Shukla_clear_target_boxes;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{
  if( command == Shukla_Epsilon ){// Convert the value to a double
      Action->SetEpsilon(((G4UIcmdWithADoubleAndUnit*) command)->GetNewDoubleValue(newValue.c_str()));
  }

  if( command == Shukla_E0)
  {
      // Convert the value to a double
      Action->SetE0(((G4UIcmdWithADoubleAndUnit*) command)->GetNewDoubleValue(newValue.c_str()));

  }

  if( command == Shukla_clear_target_boxes) Action->ClearTargetBoxes();

  if( command == Shukla_energy_threshold){
      G4double e = Shukla_energy_threshold->GetNewDoubleValue(newValue);
      Action->SetEnergyThreshold(e);
    }

  if( command == SourceSize){
      G4ThreeVector p = SourceSize->GetNew3VectorValue(newValue);
      Action->SetSourceSize(p);
    }

  if( command == SourcePosition){
      G4ThreeVector p = SourceSize->GetNew3VectorValue(newValue);
      G4cout << "** p " << p << G4endl;
      Action->SetSourcePosition(p);
    }

  if( command == Shukla_target_boxes_positions){
      G4ThreeVector p = Shukla_target_boxes_positions->GetNew3VectorValue(newValue);
      Action->AddTargetBoxPosition(p);
    }

  if( command == Shukla_target_boxes_sizes){
      G4ThreeVector p = Shukla_target_boxes_sizes->GetNew3VectorValue(newValue);
      Action->AddTargetBoxSize(p);
    }

  if( command == Shukla_clear_target_boxes) Action->ClearTargetBoxes();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

