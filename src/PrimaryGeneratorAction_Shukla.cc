//******************************************************************************
// PrimaryGeneratorAction.cc
//
// 1.00 JMV, LLNL, Jan-2007:  First version.
//******************************************************************************
//

#include <iomanip>
#include "PrimaryGeneratorAction_Shukla.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"

#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"

#include "TF1.h"
#include "TMath.h"
#include <G4Box.hh>

#include <cmath>


using namespace std;


//----------------------------------------------------------------------------//
PrimaryGeneratorAction_Shukla::PrimaryGeneratorAction_Shukla(G4Box s) : source_box(s)
{

    G4AutoLock lock(&myMutex);

    min_energy_threshold=0.1;

    SystemOffset = G4ThreeVector(48*m,0,-40*m);


    gunMessenger = new PrimaryGeneratorMessenger(this);

    target_boxes.push_back(G4Box("targetbox0",3*cm,0.4*m,0.4*m));
    target_boxes.push_back(G4Box("targetbox1",3*cm,0.4*m,0.4*m));

    target_box_positions.push_back(SystemOffset+G4ThreeVector(0.9*m,0,0.0*m) );
    target_box_positions.push_back(SystemOffset+G4ThreeVector(-0.9*m,0,0.0*m) );

    G4int efficiency_count=0;

    G4int nofParticles = 1;
    fParticleGun  = new G4ParticleGun(nofParticles);

    // Parameters : [0] = E0, [2] = E1, x = E
    G4double min_energy = 0.1;
    energy_PDF = new TF1("energy_PDF",
                         "[3]*((x+[0])**(-[2]))*([1]/([1]+x))",0.1*GeV,1000*GeV);

    // 0 degrees parameters from fit in Nottingham
    energy_PDF->SetParameter(0,4.28*GeV);
    energy_PDF->SetParameter(1,854*GeV);
    energy_PDF->SetParameter(2,3.0);
    energy_PDF->SetParameter(3,1.0);

    // Normalize the PDF
    G4double norm = energy_PDF->Integral(0.1*GeV,1000*GeV);
    energy_PDF->SetParameter(3,1.0/norm);

    // From Nottingham dataset fit in Shukla paper
    vertical_flux_rate = 88.5;// m-2 s-1 sr-1
    std::cout << "** Vertical Source Flux : " << vertical_flux_rate << " m-2 s-1 sr-1" << std::endl;

    // Parameters : [0] = r, x = cos_theta
    zenith_PDF = new TF1("zenith_PDF",
                         "(TMath::Sqrt([0]*[0]*x*x+2*[0]+1) - [0]*x)**(-[1]+1)",0.0,1.0);
    zenith_PDF->SetParameter(0,6371.0/15.0);
    zenith_PDF->SetParameter(1,3.0);

    // Integrate the zenith angle PDF wrt solid angle and energy
    // Angular integral from 0 to 90 degrees as it is through a plane

    // For horizontal source
    //flux_integrated = vertical_flux_rate*CLHEP::twopi*zenith_PDF->Integral(0.0,1.0)*energy_PDF->Integral(0.1,1000.0);

    // For a vertical source, 0 < phi < 2*pi (both directions), 0 < theta < pi/2 (0 < cos_theta < 1)
    flux_integrated = vertical_flux_rate*CLHEP::twopi*zenith_PDF->Integral(0.0,1.0)*energy_PDF->Integral(0.1,1000.0);

    // The muon rate
    std::cout << "** Source Integrated Flux : " << flux_integrated << " m-2 s-1" << std::endl;
    std::cout << "** Source Integrated Flux : " << 60.0*flux_integrated << " m-2 min-1" << std::endl;

    // Global time
    global_time = 0;

    // Sample the starting sampled position here
    source_sx = 5.0;
    source_sy = 5.0;
    source_det_x = 1.0;

    // source_box = G4Box("source", source_sx,source_sy,1.0*mm);
    source_box_position = SystemOffset;

}

G4double PrimaryGeneratorAction_Shukla::CheckEnergyIntegrals(TF1* energyPDF, double lower_thresh)
{
    // lower thresh in GeV
    return energyPDF->Integral(lower_thresh,1000*GeV);
}

//----------------------------------------------------------------------------//
PrimaryGeneratorAction_Shukla::~PrimaryGeneratorAction_Shukla()
{
    G4cout << "** Simulation efficiency : " << efficiency_count << G4endl;

    //delete particleGun;
    //delete vect;
}


//----------------------------------------------------------------------------//
G4ThreeVector PrimaryGeneratorAction_Shukla::SampleDirection(){
//        G4double min_cos_theta, G4double max_cos_theta){

    // Reduce the range of the angular integration
    // zenith_PDF->SetRange(min_cos_theta,max_cos_theta);//

    G4double cos_theta = zenith_PDF->GetRandom();
    G4double phi = -CLHEP::pi + CLHEP::twopi*G4UniformRand(); //phi uniform in [-pi, pi];
    G4double sin_theta = std::sqrt(1 - cos_theta*cos_theta);
    G4double x = sin_theta*cos(phi);
    G4double y = sin_theta*sin(phi);
    G4double z = cos_theta;
    return G4ThreeVector(x,y,-z);
}


void PrimaryGeneratorAction_Shukla::SampleParticleType(){

    // From fig 24.5 in PDG Fmu+/Fmu- is 1.3, so Fmu-/(Fmu+Fmu-) = 0.43
    // At a future date, an effect due to energy should be included.

    G4double r = G4UniformRand();
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

    if(r<0.43){
        G4ParticleDefinition* particle = particleTable->FindParticle("mu-");
        fParticleGun->SetParticleDefinition(particle);
    }else{
        G4ParticleDefinition* particle = particleTable->FindParticle("mu+");
        fParticleGun->SetParticleDefinition(particle);
    }

}


/**
 * @brief PrimaryGeneratorAction_Shukla::GeneratePrimaries
 * The required GEANT4 function to generate primary muons, which are sampled
 * according to Shukla's paper. Only tracks that will intersect the target boxes
 * are used by rejection sampling.
 * @param G4Event* anEvent
 */
void PrimaryGeneratorAction_Shukla::GeneratePrimaries(G4Event* anEvent){

    G4double E;


    // Check the intersection with the detector box
    // Check intersection
    global_time = 0;

    // Create a mutex lock for the ROOT sampling functions (not thread-safe)
    G4AutoLock lock(&myMutex);

    // By setting a min_energy_threshold it is possible to remove a portion
    // of the muon energy spectra which you're sure will not contribute
    // to the detector counts
    // i.e. for a geometry involving a thick layer of rock, you could ignore
    // the energies that will definitely range out before the detector.
    // These muons with energy<min_energy_threshold will be generated but rejected here.
    energy_PDF->SetRange(min_energy_threshold,1000.0*GeV);// E in MeV

    // Sample the energy and particle type
    E = energy_PDF->GetRandom();// E in MeV
    SampleParticleType();

    // How much the total PDF area has been reduced
    G4double speed_up_factor = CheckEnergyIntegrals(energy_PDF, min_energy_threshold);

    // Print this out for the first event
    if(anEvent->GetEventID()==0) G4cout << "** Speed-up factor : " << 1.0/speed_up_factor << G4endl;

    // Everything after here can be run on separate threads
    lock.unlock();

    // Start of the rejection sampling of muon tracks
    bool good_event=false;
    do{
        good_event=true;

        // Thread mutex lock for sampling of the track direction
        // G4AutoLock lock(&myMutex);
        //===========
        Direction = SampleDirection();
        //===========
        // lock.unlock();

        // Sample the position
        Position = source_box.GetPointOnSurface() + source_box_position;

        // Test for the intersection with the target boxes
        for(int i=0;i<target_boxes.size();i++){
            // G4cout << Position << " \t " << SystemOffset << " \t " <<  target_box_positions.at(i) << G4endl;

            // Intersection distance to the target boxes
            G4double d = (target_boxes.at(i)).DistanceToIn(
                        Position-target_box_positions.at(i),Direction);
            //G4cout << d << G4endl;
            if((d==kInfinity)||(E<min_energy_threshold)) {
                good_event=false;
                break;
            }
        }

    // G4cout << "good event " << good_event << G4endl;

    efficiency_count++;

    // Update the time which is in seconds
    // Total source rate ( in s-1) = flux_integrated (over angle and energy) * source area
    // G4cout << " sx " << source_sx/m << " sy " << source_sy/m << G4endl;

    // The source particles are generated on a very thin source box, the surface area is then
    // twice the area of the larger sides. So we divide the surface area by two
    // Source area is in mm2, whereas flux rate in in m-2 s-1, need to multiply area by 1e-6
    global_time -= s*std::log(1 - G4UniformRand())*(1.0/(speed_up_factor*flux_integrated*source_box.GetSurfaceArea()*1.0e-6));

    // Uncomment to turn off and debug sampling
    // good_event=true;
    } while(!good_event);

    /*
    G4cout << "EventID " << anEvent->GetEventID() << G4endl;
    G4cout << " - Position " << Position << G4endl;
    G4cout << " - Direction " << Direction << G4endl;
    G4cout << " - Global Time " << global_time << G4endl;
    */

    fParticleGun->SetParticleEnergy(E);
    fParticleGun->SetParticleTime(global_time);
    fParticleGun->SetParticleMomentumDirection(Direction);
    fParticleGun->SetParticlePosition(Position);
    fParticleGun->GeneratePrimaryVertex(anEvent);

}
