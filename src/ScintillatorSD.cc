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
// $Id: B5HodoscopeSD.cc 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file B5HodoscopeSD.cc
/// \brief Implementation of the B5HodoscopeSD class

#include "ScintillatorSD.hh"
#include "ScintillatorHit.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Neutron.hh"
#include "G4Gamma.hh"
#include "G4Proton.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorSD::ScintillatorSD(const G4String& name, const G4String& hitsCollectionName)
: G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1)
{
    collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorSD::~ScintillatorSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection = new ScintillatorHitsCollection(SensitiveDetectorName,collectionName[0]);
    if(fHCID <0) fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    hce->AddHitsCollection(fHCID,fHitsCollection);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 1. Which Detector has been hit
// 2. Total energy deposited
// 3. Time of hit
G4bool ScintillatorSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{

    G4double edep = step->GetTotalEnergyDeposit();
    //G4cout << " ENERGY DEP : " << edep << G4endl;
    if (edep==0.) return true;


    G4Track* track = step->GetTrack();
    int pdg = track->GetParticleDefinition()->GetPDGEncoding();

    // bool is_muon = ((pdg==13)||(pdg==-13));
    // if(!is_muon) return true;

    // so we have a hit with non-zero energy deposited


    // Get the step inside the detector
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
    G4int copyNo = touchable->GetVolume()->GetCopyNo();

    // Get the hitTime
    G4double hitTime = preStepPoint->GetGlobalTime();

        ScintillatorHit* hit = new ScintillatorHit();
        hit->SetParticleType(pdg);
        hit->SetEdep(edep);
        hit->SetTime(hitTime);
        hit->SetPos(preStepPoint->GetPosition());
        // G4cout << "Hit position : " << preStepPoint->GetPosition() << G4endl;
        hit->SetAngles(track->GetMomentumDirection());
        fHitsCollection->insert(hit);






//
//    // Only want to increment the rest if it is a primary muon (parentID==0)
//    //G4Track* track = step->GetTrack();
//    //G4int parentID = track->GetParentID();
//
//
//    G4StepPoint* preStepPoint = step->GetPreStepPoint();
//    G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
//    G4int copyNo = touchable->GetVolume()->GetCopyNo();
//
//    // Get the hitTime
//    G4double hitTime = preStepPoint->GetGlobalTime();
//
//    //
//    B5ScintillatorHit* hit = new B5ScintillatorHit(copyNo, hitTime, edep);
//
//    fHitsCollection->insert(hit);
//
//
//
//
////
////
////
////
////    G4bool muon_minus_flag=false;
////    G4bool muon_plus_flag=false;
////    G4bool neutron_flag=false;
////    G4bool gamma_flag=false;
////    G4bool proton_flag=false;
////    G4bool electron_flag=false;
////    G4bool positron_flag=false;
////
////    if (track->GetDefinition() == G4MuonMinus::Definition()) muon_minus_flag=true; // muon
////    if (track->GetDefinition() == G4MuonPlus::Definition()) muon_plus_flag=true; // muon
////    if (track->GetDefinition() == G4Neutron::Definition()) neutron_flag=true; //
////    if (track->GetDefinition() == G4Gamma::Definition()) gamma_flag=true; //
////    if (track->GetDefinition() == G4Proton::Definition()) proton_flag=true; //
////    if (track->GetDefinition() == G4Electron::Definition()) electron_flag=true; //
////    if (track->GetDefinition() == G4Positron::Definition()) positron_flag=true; //
////
////    G4int particle = track->GetDefinition()->GetPDGEncoding();
////
////    G4double kinEnergy = track->GetKineticEnergy();
////
//////     if(!muon_flag){
////
////      //    G4cout << "...incrementing " << G4endl;
////
////    G4int copyNo = touchable->GetVolume()->GetCopyNo();
////    G4double hitTime = preStepPoint->GetGlobalTime();
////
////    // check if this finger already has a hit
////    G4int ix = -1;
////    for (G4int i=0;i<fHitsCollection->entries();i++)
////    {
////        if ((*fHitsCollection)[i]->GetID()==copyNo)
////        {
////            ix = i;
////            break;
////        }
////    }
////
//// //    if (ix>=0)
//// //        // if it has, then take the earlier time
//// //    {
//// //        if ((*fHitsCollection)[ix]->GetTime()>hitTime)
//// //        {
////	// 	(*fHitsCollection)[ix]->SetTime(hitTime);
//// //        (*fHitsCollection)[ix]->AddEdep(edep);
////	// }
////
//// //    }
//// //    else
//// //        // if not, create a new hit and set it to the collection
//// //    {
////        B5HodoscopeHit* hit = new B5HodoscopeHit(copyNo,hitTime);
////
////        G4VPhysicalVolume* physical = touchable->GetVolume();
////        hit->SetLogV(physical->GetLogicalVolume());
////        G4AffineTransform transform  = touchable->GetHistory()->GetTopTransform();
////        transform.Invert();
////
////        hit->SetRot(transform.NetRotation());
////        hit->SetPos(transform.NetTranslation());
////        hit->SetEdep(edep);
////        hit->SetTime(hitTime);
////        hit->SetKinE(kinEnergy);
////    	// // hit->Print();
////        if(muon_minus_flag) hit->SetType(0);
////    	else if(muon_plus_flag) hit->SetType(1);
////    	else if(neutron_flag) hit->SetType(2);
////    	else if(gamma_flag) hit->SetType(3);
////        else if(proton_flag) hit->SetType(4);
////        else if(electron_flag) hit->SetType(5);
////        else if(positron_flag) hit->SetType(6);
////        else hit->SetType(7);
////
////        // hit->SetType(particle);
////
////     //    G4double e = G4UniformRand();
////    	// if(e<0.4) fHitsCollection->insert(hit);
////        fHitsCollection->insert(hit);
////
////    // }
////
////   // }// End if parentID==0

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
