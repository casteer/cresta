//******************************************************************************
// PrimaryGeneratorAction.hh
//
// This class is a class derived from G4VUserPrimaryGeneratorAction for
// constructing the process used to generate incident particles.
//
// 1.00 JMV, LLNL, JAN-2007:  First version.
//******************************************************************************
//
#ifndef PrimaryGeneratorAction_Shukla_h
#define PrimaryGeneratorAction_Shukla_h 1

#include "TF1.h"
#include <G4Box.hh>

#include "G4VUserPrimaryGeneratorAction.hh"

#include "PrimaryGeneratorMessenger.hh"

#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"

#include "Randomize.hh"
#include "globals.hh"
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYUtils.h"
#include "vector"
// #include "RNGWrapper.hh"
#include "G4Types.hh"

class PrimaryGeneratorMessenger;

namespace { G4Mutex myMutex = G4MUTEX_INITIALIZER; }


class PrimaryGeneratorAction_Shukla : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction_Shukla(G4Box);

    ~PrimaryGeneratorAction_Shukla();

    void AddTargetBoxSize(G4ThreeVector p){
        G4AutoLock lock(&myMutex);
        char buffer[36]; // make sure it's big enough
        snprintf(buffer, sizeof(buffer), "target_box%d", (int) target_boxes.size());
        target_boxes.push_back(G4Box(buffer, p.getX(),p.getY(),p.getZ()));
        lock.unlock();
    }

    void AddTargetBoxPosition(G4ThreeVector p){
        G4cout << "** Add target box at :  " << p << G4endl;
        target_box_positions.push_back(p);
    }

    void SetSourceSize(G4ThreeVector s){
        G4cout << "** Set source size to be :  " << s << G4endl;
        source_box = G4Box("source", s.getX(), s.getY(), s.getZ());
    }

    void SetSourcePosition(G4ThreeVector p){
        G4cout << "** Set source position to be :  " << p << G4endl;
        source_box_position = p;
    }

    void ClearTargetBoxes(){
        target_box_positions.clear();
        target_boxes.clear();
    }

    G4int efficiency_count;
    G4double global_time;

    G4double vertical_flux_rate;
    G4double flux_integrated;

    PrimaryGeneratorMessenger* gunMessenger;

    G4Box source_box;// source particles are generated on the surface of this box
    G4ThreeVector source_box_position;

    std::vector<G4Box> target_boxes;
    std::vector<G4ThreeVector> target_box_positions;

    G4double  CheckEnergyIntegrals(TF1* energyPDF, double);



    void SetEpsilon(G4double epsilon){
        energy_PDF->SetParameter(1,epsilon);
    }
    void SetE0(G4double E0){
        energy_PDF->SetParameter(0,E0);
    }
    void SetEnergyThreshold(G4double E0){
        min_energy_threshold = E0;
    }

    void SampleParticleType();
    G4ThreeVector SampleDirection();
    void GeneratePrimaries(G4Event* anEvent);


  private:

    G4double I0;//Vertical muon intensity

    G4ThreeVector Direction;
    G4ThreeVector Position;
    G4ThreeVector SystemOffset;

    G4double RE;// Radius of Earth
    G4double Hp;//Production height of muons
    // G4double r;// Ratio of the above two vars

    TF1 *energy_PDF ;
    TF1 *azimuthal_PDF;
    TF1 *zenith_PDF;

    G4ParticleGun*  fParticleGun; //pointer a to G4 service class

    // The source size
    G4double source_sx;
    G4double source_sy;
    G4double source_det_x;

    G4double min_energy_threshold;

};

#endif
