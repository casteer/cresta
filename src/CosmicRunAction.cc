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
// $Id: B5RunAction.cc 74204 2013-10-01 07:04:43Z ihrivnac $
//
/// \file B5RunAction.cc
/// \brief Implementation of the B5RunAction class

#include "CosmicRunAction.hh"
#include "CosmicRun.hh"
// #include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction_Shukla.hh"
#include "B5Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <math.h>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CosmicRunAction::CosmicRunAction() : G4UserRunAction()
{

    // Get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // Set default filename
    G4String s("cosmic_rays");
    analysisManager->SetFileName(s);


    for(int i=0;i<12;i++) counts.push_back(0);

  // Create analysis manager
  analysisManager->SetVerboseLevel(1);
  // analysisManager->SetNtupleMerging(true);


  analysisManager->CreateNtuple("DetectorEvents", "Hits");          // NTuple Id = 0
  analysisManager->CreateNtupleIColumn("EventNumber");         // column Id = 0

  analysisManager->CreateNtupleIColumn("Detector1");         // column Id = 1
  analysisManager->CreateNtupleDColumn("E1");         // column Id = 2
  analysisManager->CreateNtupleDColumn("T1");         // column Id = 3
  analysisManager->CreateNtupleDColumn("ThetaXZ1");         // column Id = 4
  analysisManager->CreateNtupleDColumn("ThetaYZ1");         // column Id = 5
  analysisManager->CreateNtupleDColumn("X1");         // column Id = 6
  analysisManager->CreateNtupleDColumn("Y1");         // column Id = 7
  analysisManager->CreateNtupleDColumn("Z1");         // column Id = 8

  analysisManager->CreateNtupleIColumn("Detector2");         // column Id = 9
  analysisManager->CreateNtupleDColumn("E2");         // column Id = 10
  analysisManager->CreateNtupleDColumn("T2");         // column Id = 11
  analysisManager->CreateNtupleDColumn("ThetaXZ2");         // column Id = 12
  analysisManager->CreateNtupleDColumn("ThetaYZ2");         // column Id = 13
  analysisManager->CreateNtupleDColumn("X2");         // column Id = 14
  analysisManager->CreateNtupleDColumn("Y2");         // column Id = 15
  analysisManager->CreateNtupleDColumn("Z2");         // column Id = 16

  analysisManager->CreateNtupleDColumn("Source_X");         // column Id = 14
  analysisManager->CreateNtupleDColumn("Source_Y");         // column Id = 15
  analysisManager->CreateNtupleDColumn("Source_Z");         // column Id = 14
  analysisManager->CreateNtupleDColumn("Source_PX");         // column Id = 14
  analysisManager->CreateNtupleDColumn("Source_PY");         // column Id = 15
  analysisManager->CreateNtupleDColumn("Source_PZ");         // column Id = 14
  analysisManager->CreateNtupleDColumn("Source_Energy");         // column Id = 15
  analysisManager->CreateNtupleDColumn("Source_Time");         // column Id = 15

  analysisManager->FinishNtuple();


  // Book histograms, ntuple
  //

  // Creating 1D histograms

  /*
  analysisManager->CreateH1("Det0_Energies","Det0_Energies",200, 0., 20*MeV);
  analysisManager->CreateH1("Det1_Energies","Det1_Energies",200, 0., 20*MeV);
  analysisManager->CreateH1("TOF","TOF",250, -10*ns, 10*ns);
  analysisManager->CreateH1("AnglesXZ","ThetaXZ",200,0,M_PI);
  analysisManager->CreateH1("AnglesYZ","ThetaYZ",200,0,M_PI);
  */

  /* analysisManager->CreateH2("Chamber1 Blob Plot","PH vs. TOF", 120, -10*ns, 10*ns, 40, 0., 20*MeV);
  analysisManager->CreateH2("Chamber2 Blob Plot","PH vs. TOF", 120, -10*ns, 10*ns,40, 0., 20*MeV);
  analysisManager->CreateH2("Chamber1 Angle-Energy Plot","Angle-Energy", 40,1.001,1.4, 40, 4., 10*MeV);
  analysisManager->CreateH2("Chamber2 Angle-Energy Plot","Angle-Energy", 40,1.001,1.4, 40, 4., 10*MeV);
*/



/*
  analysisManager->CreateNtupleDColumn("E00");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T00");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X00");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y00");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z00");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E01");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T01");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X01");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y01");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z01");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E02");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T02");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X02");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y02");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z02");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E03");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T03");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X03");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y03");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z03");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E04");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T04");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X04");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y04");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z04");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E05");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T05");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X05");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y05");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z05");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E10");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T10");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X10");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y10");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z10");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E11");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T11");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X11");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y11");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z11");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E12");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T12");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X12");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y12");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z12");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E13");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T13");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X13");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y13");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z13");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E14");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T14");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X14");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y14");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z14");         // column Id = 0

  analysisManager->CreateNtupleDColumn("E15");         // column Id = 0
  analysisManager->CreateNtupleDColumn("T15");     // column Id = 1
  analysisManager->CreateNtupleDColumn("X15");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Y15");         // column Id = 0
  analysisManager->CreateNtupleDColumn("Z15");         // column Id = 0
*/



/*
  B5DetectorConstruction* b5 = (B5DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  G4cout << "Event " << (int) b5->GetN() << G4endl;
  for(int i=0;i<(int) b5->GetN();i++){
      char char_sdname[20];
      sprintf(char_sdname,"Paddle %d",i);
      G4String name(char_sdname);
      sprintf(char_sdname,"Paddle %d Hits",i);
      G4String title(char_sdname);
      analysisManager->CreateH1(name,title, 5, 0., 4);
  }

  analysisManager->CreateH1("Anticoincidence","Anticoincidence Hits",5,0 ,4); // h1 Id = N
  analysisManager->CreateH1("Coincidence","Coincidence Hits",5,0 ,4); // h1 Id = N+1
  analysisManager->CreateH1("Stopped","Stopped",5,0 ,4); // h1 Id = N+2
*/

 // analysisManager->CreateH1("All_Hit_time","Time",1000,0*ns,500*ns);
  //analysisManager->CreateH1("Travel time","Time",1000,0*ns,20*ns);
  //analysisManager->CreateH1("Neutron Multiplicity","Neutron Multiplicity",6,0,5);
  //analysisManager->CreateH1("Gamma Multiplicity","Gamma Multiplicity",6,0,5);

  /*
  analysisManager->CreateH1("Anti Neutrons Multiplicity","Anti Neutron Multiplicity",5,0 ,4); // h1 Id = N+2
  analysisManager->CreateH1("Anti Neutrons Timing","Anti Neutron Timing",5,0 ,4); // h1 Id = N+3
  analysisManager->CreateH1("Anti Gamma Multiplicity","Anti Gamma Multiplicity",5,0 ,4); // h1 Id = N+2
  analysisManager->CreateH1("Anti Gamma Timing","Anti Gamma Timing",5,0 ,4); // h1 Id = N+3
  */






  // analysisManager->CreateH1("Paddle1","Paddle 1 # Hits", 5, 0., 4); // h1 Id = 1
  // analysisManager->CreateH1("Paddle2","Paddle 2 # Hits", 5, 0., 4); // h1 Id = 2
  // analysisManager->CreateH1("Paddle3","Paddle 3 # Hits", 5, 0., 4); // h1 Id = 3

  // analysisManager->CreateNtuple("B5", "EventsHits");
  // analysisManager->CreateNtupleIColumn("Paddle1 Edep");  // column Id = 0
  // analysisManager->CreateNtupleIColumn("Paddle2 Edep");  // column Id = 1
  // analysisManager->CreateNtupleIColumn("Paddle3 Edep");  // column Id = 2


  // Creating 2D histograms
  // analysisManager
  //   ->CreateH2("Chamber1 XY","Drift Chamber 1 X vs Y",           // h2 Id = 0
  //              50, -1000., 1000, 50, -300., 300.);
  // analysisManager
  //   ->CreateH2("Chamber2 XY","Drift Chamber 2 X vs Y",           // h2 Id = 1
  //              50, -1500., 1500, 50, -300., 300.);

  // Creating ntuple
  //
//  analysisManager->CreateNtuple("B5", "Hits");          // NTuple Id = 0
//  analysisManager->CreateNtupleDColumn("Time");         // column Id = 0
//  analysisManager->CreateNtupleDColumn("RealTime");     // column Id = 1
//  analysisManager->CreateNtupleDColumn("EnergyDep");    // column Id = 2
//  analysisManager->CreateNtupleDColumn("Particle");     // column Id = 3
//  analysisManager->CreateNtupleDColumn("WichMuon");     // column Id = 4
//  analysisManager->CreateNtupleDColumn("Paddle");       // column Id = 5
//  analysisManager->CreateNtupleDColumn("MuonEnergy");   // column Id = 6
//  analysisManager->CreateNtupleDColumn("MuonTime");     // column Id = 7
//  analysisManager->CreateNtupleDColumn("EventNumber");  // column Id = 8
//  analysisManager->FinishNtuple();
//
//  analysisManager->CreateNtuple("Events", "Events");            // NTuple Id = 1
//  analysisManager->CreateNtupleDColumn("MinTime");              // column Id = 0
//  analysisManager->CreateNtupleDColumn("RealMinTime");          // column Id = 1
//  analysisManager->CreateNtupleDColumn("NeutronMultiplicity");  // column Id = 2
//  analysisManager->CreateNtupleDColumn("GammaMultiplicity");    // column Id = 3
//  analysisManager->CreateNtupleDColumn("ProtonMultiplicity");   // column Id = 4
//  analysisManager->CreateNtupleDColumn("ElectronMultiplicity"); // column Id = 5
//  analysisManager->CreateNtupleDColumn("PositronMultiplicity"); // column Id = 6
//  analysisManager->CreateNtupleDColumn("TotalMultiplicity");    // column Id = 7
//  analysisManager->CreateNtupleDColumn("MuonEnergy");           // column Id = 8
//  analysisManager->CreateNtupleDColumn("MuonTime");             // column Id = 9
//  analysisManager->FinishNtuple();

  // analysisManager->CreateNtuple("SpHits", "SpHits");    // NTuple Id = 2
  // analysisManager->CreateNtupleDColumn("Time");         // column Id = 0
  // analysisManager->CreateNtupleDColumn("RealTime");     // column Id = 1
  // analysisManager->CreateNtupleDColumn("EnergyDep");    // column Id = 2
  // analysisManager->CreateNtupleDColumn("Particle");     // column Id = 3
  // analysisManager->CreateNtupleDColumn("WichMuon");     // column Id = 4
  // analysisManager->CreateNtupleDColumn("Paddle");       // column Id = 5
  // analysisManager->CreateNtupleDColumn("MuonEnergy");   // column Id = 6
  // analysisManager->CreateNtupleDColumn("MuonTime");     // column Id = 7
  // analysisManager->CreateNtupleDColumn("EventNumber");  // column Id = 8
  // analysisManager->FinishNtuple();

  // analysisManager->CreateNtuple("SpEvents", "SpEvents");        // NTuple Id = 3
  // analysisManager->CreateNtupleDColumn("MinTime");              // column Id = 0
  // analysisManager->CreateNtupleDColumn("RealMinTime");          // column Id = 1
  // analysisManager->CreateNtupleDColumn("NeutronMultiplicity");  // column Id = 2
  // analysisManager->CreateNtupleDColumn("GammaMultiplicity");    // column Id = 3
  // analysisManager->CreateNtupleDColumn("ProtonMultiplicity");   // column Id = 4
  // analysisManager->CreateNtupleDColumn("ElectronMultiplicity"); // column Id = 5
  // analysisManager->CreateNtupleDColumn("PositronMultiplicity"); // column Id = 6
  // analysisManager->CreateNtupleDColumn("TotalMultiplicity");    // column Id = 7
  // analysisManager->CreateNtupleDColumn("MuonEnergy");           // column Id = 8
  // analysisManager->CreateNtupleDColumn("MuonTime");             // column Id = 9
  // analysisManager->FinishNtuple();

  // analysisManager->CreateNtupleIColumn("Paddle0Hits");  // column Id = 0
  // analysisManager->CreateNtupleIColumn("Paddle1Hits");  // column Id = 1
  // analysisManager->CreateNtupleIColumn("Paddle2Hits");  // column Id = 2
  // analysisManager->CreateNtupleIColumn("Paddle3Hits");  // column Id = 3
  // analysisManager->CreateNtupleIColumn("Paddle4Hits");  // column Id = 4
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CosmicRunAction::~CosmicRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CosmicRunAction::BeginOfRunAction(const G4Run* run)
{
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  // it can be overwritten in a macro
  analysisManager->OpenFile();


  // Initialize the counters
  events_with_data=0;
  events_min_energy=1e9;
  events_average_energy=0;
  events_real_time=0;
  events_max_angleYZ=0;
  events_max_angleXZ=0;
  events_min_angleYZ=1e9;
  events_min_angleXZ=1e9;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* CosmicRunAction::GenerateRun()
{ return new CosmicRun; }


void CosmicRunAction::EndOfRunAction(const G4Run* run)
{


  if(IsMaster()){

  }else{

      G4cout << " - Events with data: " << get_data_events_counter() << G4endl;
      G4cout << " - Events average primary energy: " << get_average_energy()/GeV << " GeV " << G4endl;
      G4cout << " - Events min. primary energy: " << get_min_energy()/GeV << " GeV " << G4endl;
      G4cout << " - Events track XZ angles: " << (180/CLHEP::pi)*get_min_angleXZ() << " to " << (180/CLHEP::pi)*get_max_angleXZ() << G4endl;
      G4cout << " - Events track YZ angles: " << (180/CLHEP::pi)*get_min_angleYZ() << " to " << (180/CLHEP::pi)*get_max_angleYZ() << G4endl;

      G4double t = get_real_time()/s;
      G4cout << " - Time : " << t << G4endl;

      std::vector<G4int> c = get_counts();
      G4cout << " - Events detector counts: ";
      for(int i=0;i<12;i++) G4cout << " " << std::setw(8) << c.at(i);
      G4cout << G4endl;
      G4cout << " - Events detector rates (min^-1): ";
      for(int i=0;i<12;i++) G4cout << " " << std::setw(8) << std::setprecision(4) << 60.0*((G4double) c.at(i))/t;
      G4cout << G4endl;
      G4cout << G4endl;
      G4cout << G4endl;

      G4cout << "Local end of run with " << run->GetNumberOfEvent() << " Events" << G4endl;
  }

  // save histograms & ntuple
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
