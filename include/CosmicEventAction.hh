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
// $Id: B5EventAction.hh 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file B5EventAction.hh
/// \brief Definition of the B5EventAction class

#ifndef CosmicEventAction_h
#define CosmicEventAction_h 1


#include "G4UserEventAction.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include <vector>
#include "CosmicEventActionMessenger.hh"

/// Event action

class CosmicEventAction : public G4UserEventAction
{
public:
    CosmicEventAction();
    virtual ~CosmicEventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void SetDelaysBool(G4bool val) { fdelays_on=val; }
    void SetMCTruthBool(G4bool val) { fuse_mctruth=val; }

    void SetMinTime(G4double val) { fmintime=val; }
    void SetMinEnergy(G4double val) { fminenergy=val; }
    
    void WriteAnalysisNtuple(const G4Event* event);
    void WriteSourceAnalysisNtuple(const G4Event* event);

    void SetEfficiency(G4double e, G4int i){
        if((e<=1)&&(e>=0)) {
            G4cout << "** Set detector " << i << " efficiency to " << e << G4endl;
            efficiencies[i] = e;
        }
    }
    void SetCableLength(G4double l, G4int i){
        G4cout << "** Set detector " << i << " cable length to " << l << " mm " << G4endl;
        mean_delays[i] = ns*((l*mm)/cable_speed);
    }
    void SetCableSpeed(G4double l){
        G4cout << "** Set cable signal speed to " << l << " mm/ns" << G4endl;
        cable_speed = l*mm;
    }
    void SetMeanDelay(G4double e, G4int i){
        G4cout << "** Set cable mean delay to " << e << " ns" << G4endl;
        mean_delays[i] = e;
    }
    void SetStdDelay(G4double e, G4int i){
        std_delays[i] = e;
    }

private:
    G4int fHHC1ID;
    G4int fHHC2ID;
    G4int fHHC3ID;

//    G4int analysed_events;

    std::vector<G4double> efficiencies;
    std::vector<G4double> mean_delays;
    G4double cable_speed;
    std::vector<G4double> std_delays;

    std::vector<G4int> fHH;

  	G4bool fdelays_on;
    G4bool fuse_mctruth;

    G4double fmintime;
    G4double fminenergy;


    CosmicEventActionMessenger* EvtMessenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
