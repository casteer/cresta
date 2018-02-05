
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: exampleB5.cc 70284 2013-05-28 17:26:43Z perl $
//
/// \file exampleB5.cc
/// \brief Main program of the analysis/B5 example

#include "DetectorConstruction.hh"
#include "CosmicActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "Shielding.hh"
#include "G4StepLimiterPhysics.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
#ifdef G4UI_USE
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
#endif

// CANNOT RUN IN MULTITHREADED MODE WITH CRY :-(
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(3);
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Mandatory user initialization classes
  runManager->SetUserInitialization(new DetectorConstruction);

  G4VModularPhysicsList* physicsList = new Shielding;
  //physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new CosmicActionInitialization());
  runManager->Initialize();

#ifdef G4VIS_USE
  // Visualization manager construction
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if ( argc != 1 ) {
    // execute an argument macro file if exist
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);

    // Write out the final stats
    // B5DetectorConstruction* b5 = (B5DetectorConstruction*) runManager->GetUserDetectorConstruction();
    // // Get the Pb thickness
    // G4double pb_thickness = b5->GetPbHeight();
    // // Get the muon energy
    // B5PrimaryGeneratorAction* b5_pg = (B5PrimaryGeneratorAction*) runManager->GetUserPrimaryGeneratorAction();
    // G4double mom = b5_pg->GetMomentum();

    // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    // G4cout << "\n\n ----> Total Histograms Statistic \n" << G4endl;
    // G4cout << mom;
    // G4cout << " " << pb_thickness ;
    // G4int ind = (b5->GetN())+1;
    // G4cout << " " <<  analysisManager->GetH1(ind)->entries() ;
    // for(int i=1;i<=(int) b5->GetN();i++){
    //   G4cout << " " << analysisManager->GetH1(i)->entries();
    // }
    // G4cout << " " << analysisManager->GetH1(0)->entries() << G4endl;

  }  else {
#ifdef G4UI_USE
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
    UImanager->ApplyCommand("/control/execute init.mac");
#endif
    if (ui->IsGUI()) {
         UImanager->ApplyCommand("/control/execute gui.mac");
    }
    // start interactive session
    ui->SessionStart();
    delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  G4cout<< "Delete G4analysis" << G4endl;
  delete runManager;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
