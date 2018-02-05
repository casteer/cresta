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
// $Id: B5RunAction.hh 74204 2013-10-01 07:04:43Z ihrivnac $
// 
/// \file B5RunAction.hh
/// \brief Definition of the B5RunAction class

#ifndef CosmicRunAction_h
#define CosmicRunAction_h 1

#include "PrimaryGeneratorAction_Shukla.hh"


#include "G4UserRunAction.hh"
#include "globals.hh"

class CosmicRun;

/// Run action class

class CosmicRunAction : public G4UserRunAction
{

    public:

    CosmicRunAction();
    virtual ~CosmicRunAction();
    G4Run* GenerateRun();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void increment_data_events(){events_with_data++;};
    void increment_counts(G4int i){counts[i]++;};
    void increment_real_time(G4double t){events_real_time+=t;};
    void increment_average_energy(G4double e){events_average_energy+=e;};
    void increment_min_energy(G4double e){if(events_min_energy>e) events_min_energy = e;};

    void increment_min_angleXZ(G4double a){if(a>0) if(events_min_angleXZ>a) events_min_angleXZ=a;};
    void increment_min_angleYZ(G4double a){if(a>0) if(events_min_angleXZ>a) events_min_angleYZ=a;};
    void increment_max_angleXZ(G4double a){if(a>0) if(events_max_angleXZ<a) events_max_angleXZ=a;};
    void increment_max_angleYZ(G4double a){if(a>0) if(events_max_angleXZ<a) events_max_angleYZ=a;};

    G4int get_data_events_counter(){return events_with_data;};
    G4double get_average_energy(){return events_average_energy/((G4double) events_with_data);};
    G4double get_min_energy(){return events_min_energy;};

    G4double get_min_angleXZ(){return events_min_angleXZ;};
    G4double get_max_angleXZ(){return events_max_angleXZ;};

    G4double get_min_angleYZ(){return events_min_angleYZ;};
    G4double get_max_angleYZ(){return events_max_angleYZ;};

    G4double get_real_time(){return events_real_time;};
    std::vector<G4int> get_counts(){return counts;};

private:
    std::vector<G4int> counts;
    G4int events_with_data;
    G4double events_average_energy;
    G4double events_min_energy;
    G4double events_real_time;

    G4double events_max_angleYZ;
    G4double events_max_angleXZ;
    G4double events_min_angleYZ;
    G4double events_min_angleXZ;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

