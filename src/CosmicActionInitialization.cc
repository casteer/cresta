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
// $Id: B5ActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file B5ActionInitialization.cc
/// \brief Implementation of the B5ActionInitialization class

#include "CosmicActionInitialization.hh"
// #include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction_Shukla.hh"
#include "CosmicRunAction.hh"
#include "CosmicEventAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CosmicActionInitialization::CosmicActionInitialization()
 : G4VUserActionInitialization()
{
 //   PGA = new PrimaryGeneratorAction_Shukla();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CosmicActionInitialization::~CosmicActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CosmicActionInitialization::BuildForMaster() const
{
  SetUserAction(new CosmicRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CosmicActionInitialization::Build() const
{
  // PrimaryGeneratorAction_Shukla* PGA = new PrimaryGeneratorAction_Shukla();
  SetUserAction(new PrimaryGeneratorAction_Shukla(G4Box("source", 1.0*m,1.0*m,1.0*mm)));
  SetUserAction(new CosmicRunAction());
  SetUserAction(new CosmicEventAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
