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
// $Id: B5EventAction.cc 87359 2014-12-01 16:04:27Z gcosmo $
//
/// \file B5EventAction.cc
/// \brief Implementation of the B5EventAction class

#include "CosmicEventAction.hh"
#include "CosmicRunAction.hh"

#include "ScintillatorHit.hh"
//#include "B5DriftChamberHit.hh"
//#include "DetectorConstruction.hh"
//#include "B5EmCalorimeterHit.hh"
//#include "B5HadCalorimeterHit.hh"
#include "CosmicAnalysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include <math.h>
#include "G4PrimaryParticle.hh"

#include <vector>
#include <algorithm>
#include <numeric>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CosmicEventAction::CosmicEventAction() : G4UserEventAction()
{
  // set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(100);

  // Create the messenger file
  EvtMessenger = new CosmicEventActionMessenger(this);

  for(int i=0;i<12;i++) {
        efficiencies.push_back(1.0);
        mean_delays.push_back(0.0);
        std_delays.push_back(0.0);
  }
  cable_speed=200.0;

 // analysed_events=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CosmicEventAction::~CosmicEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CosmicEventAction::BeginOfEventAction(const G4Event* event)
{


//      B5DetectorConstruction* b5 = (B5DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
      // G4cout << "Event " << (int) b5->GetN() << G4endl;

//      fHH.clear();

//	G4cout << "HELLO!!!! " << b5->GetN() << G4endl;

//    for(int i=0;i<(int) b5->GetN();i++){
//
//      char char_sdname[20];
//      sprintf(char_sdname,"paddles1/det%d",i);
//      G4String sdname(char_sdname);
//
//      fHH.push_back(sdManager->GetCollectionID(sdname));
//
//    }

      // fHHC1ID = sdManager->GetCollectionID("paddle0/hodoscopeColl");
      // fHHC2ID = sdManager->GetCollectionID("paddle1/hodoscopeColl");
      // fHHC3ID = sdManager->GetCollectionID("paddle2/hodoscopeColl");

      // fDHC1ID = sdManager->GetCollectionID("chamber1/driftChamberColl");
      // fDHC2ID = sdManager->GetCollectionID("chamber2/driftChamberColl");
      // fECHCID = sdManager->GetCollectionID("EMcalorimeter/EMcalorimeterColl");
      // fHCHCID = sdManager->GetCollectionID("HadCalorimeter/HadCalorimeterColl");

}

void CosmicEventAction::WriteAnalysisNtuple(const G4Event* event){

    int ndetectors=9;

    std::vector<ScintillatorHitsCollection*> hc;
    G4bool hc_exists=true;
    for(int i=0;i<ndetectors;i++){
        hc.push_back((ScintillatorHitsCollection*) event->GetHCofThisEvent()->GetHC(i));
        if(!hc.back()) hc_exists=false;
    }

    if (!hc_exists)
    {
        G4ExceptionDescription msg;
        msg << "No hits collection of this event found." << G4endl;
        G4Exception("CosmicEventAction::EndOfEventAction()",
                    "CosmicCode001", JustWarning, msg);
        return;
    }

    G4double T = (event->GetPrimaryVertex())->GetT0();

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0,event->GetEventID());

    // Sort information into detectors
    std::vector<double> energy_deposited(ndetectors,0.0);
    std::vector<double> angles_XZ(ndetectors,0.0);
    std::vector<double> angles_YZ(ndetectors,0.0);
    std::vector<double> time_of_arrival(ndetectors,0.0);
    std::vector<G4ThreeVector> positions(ndetectors,G4ThreeVector(0,0,0));

    std::vector<bool> is_hit_eliminated(ndetectors,false);

    for(int i=0;i<ndetectors;i++){

        // Get the number of hits
        int nhits = (int)  (hc.at(i))->GetSize();

        for(int ihit=0;ihit<nhits;ihit++){
            // std::cout << " nhits " << nhits << " " << ihit << std::endl;
            energy_deposited[i]+=( *(hc.at(i)) )[ihit]->GetEdep()/MeV;
            time_of_arrival[i]+= (( ( *(hc.at(i)) )[ihit]->GetTime()/ns) - T/ns);
            angles_XZ[i]+=( *(hc.at(i)) )[ihit]->GetThetaXZ();
            angles_YZ[i]+=( *(hc.at(i)) )[ihit]->GetThetaYZ();
            positions[i]+=( *(hc.at(i)) )[ihit]->GetPos()/m;
        }

        if(nhits>0){
            energy_deposited[i]/=nhits;
            time_of_arrival[i]/=nhits;
            angles_XZ[i]/=nhits;
            angles_YZ[i]/=nhits;
            positions[i]/=nhits;
            // G4cout << "Positions : " << positions[i] << G4endl;
        }

        if(std_delays.at(i)>0) {
            double time_delay = G4RandGauss::shoot(mean_delays.at(i),std_delays.at(i));
            time_of_arrival[i]+=time_delay;
        }

        // Remove hit if random number is larger than the efficiency
        double r = G4UniformRand();
        if(r>efficiencies.at(i)){
            energy_deposited[i]=0.0;
            time_of_arrival[i]=0.0;
            angles_XZ[i]=0.0;
            angles_YZ[i]=0.0;
            positions[i]=G4ThreeVector(0.0,0.0,0.0);
            is_hit_eliminated[i]=true;
        }


    }// End of loop over the detectors

    // Find if there have only been two detectors been hit in this event
    int n_detectors_hit = 0;
    double min_energy = 0.0;// min energy threshold in MeV for trigger
    for(int i=0;i<ndetectors;i++) if(energy_deposited[i]>min_energy) n_detectors_hit++;
    bool is_two_detectors_only = (n_detectors_hit==2);


    // Find the two detectors with the largest energy deposited
    // - create a vector of indices
    std::vector<int> y(energy_deposited.size());
    std::iota(y.begin(), y.end(), 0);

    // - create a comparator based on the energy deposited
    // - sort based on the comparator (lowest index of y corresponds to the highest energy deposited)
    // DEBUG: for(int i=0;i<ndetectors;i++) G4cout << " US Detector " << i << " : " << energy_deposited.at(i) << G4endl;
    std::sort( y.begin(),
                   y.end(),
                   [&](int i1, int i2) { return (energy_deposited[i1] > energy_deposited[i2]); } );

    int detector_index0 = y.at(0);
    int detector_index1 = y.at(1);

    // Find the detector hit which is in the trigger set of detectors

    // for(int i=0;i<ndetectors;i++) G4cout << " S Detector " << i << " : " << energy_deposited.at(i) << " \t " << y.at(i) << G4endl;

    // G4cout << " detector_index0 " << detector_index0 << G4endl;
    // G4cout << " detector_index1 " << detector_index1 << G4endl;
    bool is_energy_in_both_detectors = (energy_deposited.at(detector_index0)>0)&&(energy_deposited.at(detector_index1)>0);

    // Check that the hits are both sides of the system
    bool is_on_both_sides = ((detector_index0<=5)&&(detector_index1>=6))||((detector_index0>=6)&&(detector_index1<=5));

    // Eliminated event
    bool is_event_eliminated = (is_hit_eliminated[detector_index0]) || (is_hit_eliminated[detector_index1]);

    // Set the trigger hits (Detector number = 6,7, and 8) on as the Detector0 in output tuple
    if((detector_index0<=5)&&(detector_index1>=6)){
        int tmp = detector_index1;
        detector_index1 = detector_index0;
        detector_index0 = tmp;
    }

    // G4cout << " Largest Edep :  " << energy_deposited.at(detector_index0) << G4endl;
    // G4cout << " Next Largest Edep :  " << energy_deposited.at(detector_index1) << G4endl;
    // G4cout << " Is Edep > 0 :  " << is_energy_in_both_detectors << G4endl;

    if( is_energy_in_both_detectors && is_on_both_sides && (!is_event_eliminated) && is_two_detectors_only  ){

        CosmicRunAction* ra = (CosmicRunAction*) G4RunManager::GetRunManager()->GetUserRunAction();
        ra->increment_data_events();
        ra->increment_counts(detector_index0);
        ra->increment_counts(detector_index1);

        analysisManager->FillNtupleIColumn(1,detector_index0);
        analysisManager->FillNtupleDColumn(2,energy_deposited[detector_index0]);
        analysisManager->FillNtupleDColumn(3,time_of_arrival[detector_index0]);
        analysisManager->FillNtupleDColumn(4,angles_XZ[detector_index0]);
        analysisManager->FillNtupleDColumn(5,angles_YZ[detector_index0]);
        analysisManager->FillNtupleDColumn(6,positions[detector_index0].getX());
        analysisManager->FillNtupleDColumn(7,positions[detector_index0].getY());
        analysisManager->FillNtupleDColumn(8,positions[detector_index0].getZ());

        analysisManager->FillNtupleIColumn(9,detector_index1);
        analysisManager->FillNtupleDColumn(10,energy_deposited[detector_index1]);
        analysisManager->FillNtupleDColumn(11,time_of_arrival[detector_index1]);
        analysisManager->FillNtupleDColumn(12,angles_XZ[detector_index1]);
        analysisManager->FillNtupleDColumn(13,angles_YZ[detector_index1]);
        analysisManager->FillNtupleDColumn(14,positions[detector_index1].getX());
        analysisManager->FillNtupleDColumn(15,positions[detector_index1].getY());
        analysisManager->FillNtupleDColumn(16,positions[detector_index1].getZ());

        // Only add source event information if it deposits energy in both detectors
        G4double X0 = (event->GetPrimaryVertex())->GetX0();
        G4double Y0 = (event->GetPrimaryVertex())->GetY0();
        G4double Z0 = (event->GetPrimaryVertex())->GetZ0();
        G4double Px = (event->GetPrimaryVertex())->GetPrimary()->GetPx();
        G4double Py = (event->GetPrimaryVertex())->GetPrimary()->GetPy();
        G4double Pz = (event->GetPrimaryVertex())->GetPrimary()->GetPz();
        G4double E = (event->GetPrimaryVertex())->GetPrimary()->GetTotalEnergy();

        ra->increment_average_energy(E);
        ra->increment_min_energy(E);
        ra->increment_real_time(T);
        ra->increment_min_angleXZ(angles_XZ[detector_index1]);
        ra->increment_max_angleXZ(angles_XZ[detector_index1]);
        ra->increment_min_angleYZ(angles_YZ[detector_index1]);
        ra->increment_max_angleYZ(angles_YZ[detector_index1]);

        analysisManager->FillNtupleDColumn(17,X0);
        analysisManager->FillNtupleDColumn(18,Y0);
        analysisManager->FillNtupleDColumn(19,Z0);

        analysisManager->FillNtupleDColumn(20,Px);
        analysisManager->FillNtupleDColumn(21,Py);
        analysisManager->FillNtupleDColumn(22,Pz);
        analysisManager->FillNtupleDColumn(23,E);
        analysisManager->FillNtupleDColumn(24,T);// in ns

        analysisManager->AddNtupleRow();

    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void CosmicEventAction::EndOfEventAction(const G4Event* event)
{

    //G4cout << "Event " << event->GetEventID() << G4endl;
    WriteAnalysisNtuple(event);

    G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    CosmicRunAction* ra = (CosmicRunAction*) (G4RunManager::GetRunManager())->GetUserRunAction();
    if ((event->GetEventID()>0) && ( event->GetEventID() % printModulo == 0))
    {
        G4cout << " - Events with data: " << ra->get_data_events_counter() << G4endl;
        G4cout << " - Events average primary energy: " << ra->get_average_energy()/GeV << " GeV " << G4endl;
        G4cout << " - Events min. primary energy: " << ra->get_min_energy()/GeV << " GeV " << G4endl;
        G4cout << " - Events track XZ angles: " << (180/CLHEP::pi)*ra->get_min_angleXZ() << " to " << (180/CLHEP::pi)*ra->get_max_angleXZ() << G4endl;
        G4cout << " - Events track YZ angles: " << (180/CLHEP::pi)*ra->get_min_angleYZ() << " to " << (180/CLHEP::pi)*ra->get_max_angleYZ() << G4endl;

        G4double t = ra->get_real_time()/s;
        G4cout << " - Time : " << t << G4endl;

        std::vector<G4int> c = ra->get_counts();
        G4cout << " - Events detector counts: ";
        for(int i=0;i<12;i++) G4cout << " " << std::setw(8) << c.at(i);
        G4cout << G4endl;
        G4cout << " - Events detector rates (min^-1): ";
        G4double total_rate = 0.0;
        for(int i=0;i<12;i++) {
            G4cout << " " << std::setw(8) << std::setprecision(4) << 60.0*((G4double) c.at(i))/t;
            total_rate += 60.0*((G4double) c.at(i))/t;
        }
        G4cout << G4endl << " - Total Event Rate (min^-1): " << total_rate << G4endl;
        G4cout << G4endl;
        G4cout << G4endl;
        G4cout << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
