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
// $Id: DetectorMessenger.hh,v 1.6 2002/12/16 16:37:26 maire Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "DetectorConstruction.hh"
#include "globals.hh"

class PrimaryGeneratorAction_Shukla;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger( DetectorConstruction* this_dc);
   ~DetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    DetectorConstruction*      DC;

    G4UIdirectory*               DetectorDir;

    G4UIdirectory*               WorldDir;
    G4UIcmdWith3VectorAndUnit*    WorldSize;
    G4UIcmdWithAString*    WorldMaterial;

    G4UIcmdWith3VectorAndUnit*    ChamberSize;
    G4UIcmdWithAString*    ChamberMaterial;
    G4UIcmdWithoutParameter*    ChambersReset;
    G4UIcmdWith3VectorAndUnit*    ChamberPosition;

    G4UIcmdWithAnInteger* SetCurrentChamber;

    G4UIcmdWithADoubleAndUnit*    SetDetectorStandOff;
    G4UIcmdWithAnInteger* SetCurrentDetector;
    G4UIcmdWith3VectorAndUnit*    DetectorPosition;
    G4UIcmdWith3Vector*    DetectorRotation;
    G4UIcmdWithoutParameter* PrintDetectorInfo;


    int current_chamber;
    int current_detector;


    // G4UIcmdWithADoubleAndUnit*       Shukla_E0;
    // G4UIcmdWithoutParameter* Shukla_clear_target_boxes;

    std::string* MessInput;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

