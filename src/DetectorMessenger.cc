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
// $Id: DetectorMessenger.cc,v 1.8 2002/12/16 16:37:27 maire Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "PrimaryGeneratorAction_Shukla.hh"
#include "G4UIdirectory.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* dc_in) : DC(dc_in)
{
    current_chamber=0;
    current_detector=0;

    WorldDir = new G4UIdirectory("/Cosmic/");
    WorldDir->SetGuidance("Parameters of the world");

    WorldSize = new G4UIcmdWith3VectorAndUnit("/Cosmic/WorldBoxSize", this);
    WorldSize->SetGuidance("Specifies the world box size");
    WorldSize->SetParameterName("Half-length X ","Half-length Y ","Half-length Z ",false);
    WorldSize->AvailableForStates(G4State_PreInit,G4State_Idle);

    WorldMaterial = new G4UIcmdWithAString("/Cosmic/WorldMaterial", this);
    WorldMaterial->SetGuidance("Specifies the name of the NIST material inside the world box (e.g. G4_AIR) ");
    WorldMaterial->SetParameterName("NIST material name",false);
    WorldMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);

    ChamberSize = new G4UIcmdWith3VectorAndUnit("/Cosmic/ChamberBoxSize", this);
    ChamberSize->SetGuidance("Specifies the chamber box size");
    ChamberSize->SetParameterName("Half-length X ","Half-length Y ","Half-length Z ",false);
    ChamberSize->AvailableForStates(G4State_PreInit,G4State_Idle);

    ChamberPosition = new G4UIcmdWith3VectorAndUnit("/Cosmic/ChamberPosition", this);
    ChamberPosition->SetGuidance("Specifies the tunnel centre position ");
    ChamberPosition->SetParameterName("Half-length X ","Half-length Y ","Half-length Z ",false);
    ChamberPosition->AvailableForStates(G4State_PreInit,G4State_Idle);

    ChamberMaterial = new G4UIcmdWithAString("/Cosmic/ChamberMaterial", this);
    ChamberMaterial->SetGuidance("Specifies the name of the NIST material inside the chamber box (e.g. G4_AIR) ");
    ChamberMaterial->SetParameterName("NIST material name",false);
    ChamberMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);

    ChambersReset = new G4UIcmdWithoutParameter("/Cosmic/ResetChambers", this);
    ChambersReset->SetGuidance("Clears all of the chamber information, ready for new definitions");
    ChambersReset->AvailableForStates(G4State_PreInit,G4State_Idle);

    SetCurrentChamber= new G4UIcmdWithAnInteger("/Cosmic/SetCurrentChamber", this);
    SetCurrentChamber->SetGuidance("Sets chamber ID to be altered");
    SetCurrentChamber->SetParameterName("ChamberIndex",false);
    SetCurrentChamber->AvailableForStates(G4State_PreInit,G4State_Idle);

    SetDetectorStandOff= new G4UIcmdWithADoubleAndUnit("/Cosmic/SetDetectorStandOff", this);
    SetDetectorStandOff->SetGuidance("Sets the distance between detector stacks");
    SetDetectorStandOff->SetParameterName("DetSetDistance",false);
    SetDetectorStandOff->AvailableForStates(G4State_PreInit,G4State_Idle);

    DetectorPosition = new G4UIcmdWith3VectorAndUnit("/Cosmic/DetectorPosition", this);
    DetectorPosition->SetGuidance("Sets the current detector position ");
    DetectorPosition->SetParameterName("X Coordinate","Y Coordinate","Z Coordinate",false);
    DetectorPosition->AvailableForStates(G4State_PreInit,G4State_Idle);

    DetectorRotation = new G4UIcmdWith3Vector("/Cosmic/DetectorRotation", this);
    DetectorRotation->SetGuidance("Sets the current detector rotation (angles in degrees)");
    DetectorRotation->SetParameterName("X rotation angle","Y rotation angle","Z rotation angle",false);
    DetectorRotation->AvailableForStates(G4State_PreInit,G4State_Idle);



    SetCurrentDetector = new G4UIcmdWithAnInteger("/Cosmic/SetCurrentDetector", this);
    SetCurrentDetector ->SetGuidance("Sets Detector ID to be altered");
    SetCurrentDetector ->SetParameterName("DetectorIndex",false);
    SetCurrentDetector ->AvailableForStates(G4State_PreInit,G4State_Idle);


    PrintDetectorInfo= new G4UIcmdWithoutParameter("/Cosmic/PrintDetectorInfo", this);
    PrintDetectorInfo -> SetGuidance("Prints the detectors translation and rotation information.");
    PrintDetectorInfo -> AvailableForStates(G4State_PreInit,G4State_Idle);


    MessInput = new std::string;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
    delete WorldDir;
    delete WorldSize;
    delete WorldMaterial;
    delete ChamberSize;
    delete ChamberPosition;
    delete SetCurrentChamber;
    delete ChambersReset;
    delete SetDetectorStandOff;
    delete DetectorPosition;
    delete SetCurrentDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{


  if( command == WorldSize){// Convert the value to a three vector
      G4ThreeVector t = ((G4UIcmdWith3VectorAndUnit*) command)->GetNew3VectorValue(newValue.c_str());
      DC->SetWorldBox( new G4Box("world_volume", t.getX(),t.getY(),t.getZ()) );
  }

  if( command == WorldMaterial){
        DC->SetWorldMaterial(newValue);
  }

  if( command == ChamberMaterial){
        DC->SetChamberMaterial(current_chamber,newValue);
  }

  if( command == ChamberSize){// Convert the value to a three vector
      G4ThreeVector t = ((G4UIcmdWith3VectorAndUnit*) command)->GetNew3VectorValue(newValue.c_str());
      DC->SetChamberVolume(current_chamber,t);
  }

  if( command == ChamberPosition){// Convert the value to a three vector
      G4ThreeVector t = ((G4UIcmdWith3VectorAndUnit*) command)->GetNew3VectorValue(newValue.c_str());
      DC->SetChamberPosition(current_chamber,t);
  }

  if( command == ChambersReset){
      DC->ClearChambers();
  }

  if( command == SetCurrentChamber){
      current_chamber=((G4UIcmdWithAnInteger*) command)->GetNewIntValue(newValue.c_str());
  }

  if( command == PrintDetectorInfo){
        DC->PrintDetectorInfo();
  }

  if( command == SetCurrentDetector){
      current_detector=((G4UIcmdWithAnInteger*) command)->GetNewIntValue(newValue.c_str());
  }

  if( command == DetectorPosition){// Convert the value to a three vector
      G4ThreeVector t = ((G4UIcmdWith3VectorAndUnit*) command)->GetNew3VectorValue(newValue.c_str());
      DC->SetDetectorPosition(current_detector,t);
  }

  if( command == DetectorRotation){// Convert the value to a three vector
      G4ThreeVector t = ((G4UIcmdWith3Vector*) command)->GetNew3VectorValue(newValue.c_str());
      DC->SetDetectorRotation(current_detector,t);
  }

  if( command == SetDetectorStandOff){
      G4double d = ((G4UIcmdWithADoubleAndUnit*) command)->GetNewDoubleValue(newValue.c_str());
      DC->SetDetectorStandOff(d);
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

