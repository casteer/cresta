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
// $Id: PrimaryGeneratorMessenger.hh,v 1.6 2002/12/16 16:37:26 maire Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "PrimaryGeneratorAction_Shukla.hh"
#include "globals.hh"

class PrimaryGeneratorAction_Shukla;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger( PrimaryGeneratorAction_Shukla* );
   ~PrimaryGeneratorMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    PrimaryGeneratorAction_Shukla*      Action;

    G4UIdirectory*               ShuklaDir;
    G4UIcmdWithADoubleAndUnit*       Shukla_E0;
    G4UIcmdWithADoubleAndUnit*       Shukla_Epsilon;


    G4UIcmdWith3VectorAndUnit*    SourceSize;
    G4UIcmdWith3VectorAndUnit*    SourcePosition;// Should be set to the system offset

    G4UIcmdWithADoubleAndUnit*       Shukla_source_sx;
    G4UIcmdWithADoubleAndUnit*       Shukla_source_sy;
    G4UIcmdWithADoubleAndUnit*       Shukla_source_det_x;
    G4UIcmdWithADoubleAndUnit*       Shukla_energy_threshold;

    G4UIcmdWith3VectorAndUnit*       Shukla_target_boxes_positions;
    G4UIcmdWith3VectorAndUnit*       Shukla_target_boxes_sizes;

    G4UIcmdWith3VectorAndUnit*       Shukla_system_offset;

    G4UIcmdWithoutParameter* Shukla_clear_target_boxes;
/*    G4UIdirectory*               CRYDir;
    G4UIcmdWithAString*          FileCmd;
    G4UIcmdWithAString*          InputCmd;
    G4UIcmdWithoutParameter*     UpdateCmd;
    */
    std::string* MessInput;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

