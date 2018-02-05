#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "G4NistManager.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"


#include "ScintillatorSD.hh"
#include "ScintillatorPaddle.hh"
#include "TunnelChamber.hh"
#include <string>


DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction()
{
    /*
     * Create the materials
     *
     */
    G4NistManager* nistManager = G4NistManager::Instance();
    nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    nistManager->FindOrBuildMaterial("G4_AIR");
    nistManager->FindOrBuildMaterial("G4_CONCRETE");
    nistManager->FindOrBuildMaterial("G4_CALCIUM_CARBONATE");
    nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

    system_offset = G4ThreeVector(48.0*m,0,-40.0*m);
    world_box = new G4Box("worldBox",150.*m,150*m,150*m);
    world_material = G4Material::GetMaterial("G4_AIR");

    chamber_materials.push_back(G4Material::GetMaterial("G4_AIR"));
    chamber_positions.push_back(G4ThreeVector(0.0,0.0,0.0));
    chamber_sizes.push_back(G4ThreeVector(20.0*m,20.0*m,20.0*m));

    // Create the messenger too
    DetMessenger = new DetectorMessenger(this);

    detector_set_separation = 1.8*m;

}

DetectorConstruction::~DetectorConstruction()
{
    //dtor
}


/*
 *
 */
void DetectorConstruction::ConstructChamber(int i){

    // Does this chamber already exist?
    if(i<(int) chambers.size()){
        // Yes, so modify its properties

        // Open geometry for the physical volume to be modified ...
        G4GeometryManager* gm = G4GeometryManager::GetInstance();
        gm->OpenGeometry((chambers.at(i)->get_physical_volume()));

        // Set the position
        (chambers.at(i)->get_physical_volume())->SetTranslation(chamber_positions.at(i));

        // Set the box size
        G4String name("chamber_box");
        name += std::to_string((int) i);
        (chambers.at(i)->get_logical_volume())->SetSolid(
                    new G4Box(name, chamber_sizes.at(i).getX(),
                              chamber_sizes.at(i).getY(),
                              chamber_sizes.at(i).getZ()));

        // Re-Set the material
        (chambers.at(i)->get_logical_volume())->SetMaterial(chamber_materials.at(i));

        // Close geometry for the portion modified ...
        gm->CloseGeometry(chambers.at(i)->get_physical_volume());


    }else{

        // No it doesn't exist so let's add it here
        G4String name("chamber");
        name += std::to_string((int) i);

        // Create a tunnel and/or chamber
        chambers.push_back(new TunnelChamber(name,((G4int) 1000)+i,chamber_materials.at(i)));

        // Set the position and size
        (chambers.back())->set_position(chamber_positions.at(i));
        (chambers.back())->set_dimensions(chamber_sizes.at(i));

        //
        worldPhysical = (chambers.back())->Construct(worldPhysical);

    }

    // Needed to restart the production cuts
    (G4RunManager::GetRunManager())->GeometryHasBeenModified();

}

G4VPhysicalVolume* DetectorConstruction::Construct(){

    G4bool checkOverlaps = true;


    // Get the basic materials other ones are stored in the class's vectors
    G4Material* scintillator = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); // density 1.032*g/cm^3
    G4Material* air = G4Material::GetMaterial("G4_AIR");


    /*
     * Create the world volume
     */
   // Create a world box
   worldSolid = world_box ;//= new G4Box("worldBox",50.*m,15*m,50*m);
   // And a logical box
   worldLogical = new G4LogicalVolume(worldSolid,world_material,"worldLogical");
   // And a physical box
   worldPhysical = new G4PVPlacement(0,G4ThreeVector(0,0,0),worldLogical,"worldPhysical",0,false,100,checkOverlaps);

   /*
    * Create the chambers within the world volume
    */
   chambers.clear();//  Just in case there's anything already there
   for(int i=0;i<(int) chamber_sizes.size();i++) ConstructChamber(i);

   G4cout << "** Constructing paddles... " << G4endl;
   for(int i=0; i<6;i++){

       G4String name("paddle0");
       name += std::to_string((int) i);
       // Create the name of the detector volume
       G4cout << "**  - Paddles name : " << name << G4endl;

       // Create the detector pointer
       detectors.push_back(new ScintillatorPaddle(name,(G4int) i,scintillator));
       // - Set the dimensions and position
       (detectors.back())->set_dimensions( G4ThreeVector(2.5*cm, 80.*cm, 15.*cm) );
       G4cout << "**  - Paddle dimensions : " << G4ThreeVector(2.5*cm, 80.*cm, 15.*cm) << G4endl;
       // G4cout << "Centre " << G4ThreeVector(-0.6*m, 0.0, ((G4double) i-2.5)*15.1*cm + vertical_offset) << G4endl;
       (detectors.back())->set_position( G4ThreeVector(-0.5*detector_set_separation, 0.0, ((G4double) i-2.5)*15.1*cm ) );
       G4cout << "**  - Paddle position : " << G4ThreeVector(-0.5*detector_set_separation, 0.0, ((G4double) i-2.5)*15.1*cm ) << G4endl;

       // - Place the detectors in the right chamber volume
       G4cout << "**  - Placing in chamber " << G4endl;

       (chambers.back())->set_physical_volume(
               (detectors.back())->Construct(
                       (chambers.back())->get_physical_volume()
                       )
            );

       // G4cout << " ** Detector " << i << " \t " << G4ThreeVector(-0.5*detector_set_separation, 0.0, ((G4double) i-2.5)*15.1*cm + vertical_offset) << G4endl;
   }


   // Three target detectors
   //   DETECTOR #1
   // Create the name of the detector volume
   G4String name0("paddle10");
   // Create the detector pointer
   detectors.push_back(new ScintillatorPaddle(name0,(G4int) 6,scintillator));
   // - Set the dimensions and position
   (detectors.back())->set_dimensions( G4ThreeVector(2.5*cm, 80.*cm, 15.*cm) );
   (detectors.back())->set_position(G4ThreeVector(0.5*detector_set_separation-1.0*m, 0.0, -2.5*15.1*cm ) );
   // - Place the detectors in the right chamber volume
   (chambers.back())->set_physical_volume(
           (detectors.back())->Construct(
                   (chambers.back())->get_physical_volume()
                   )
        );


    //   DETECTOR #2
    // Create the name of the detector volume
    G4String name1("paddle11");
    // Create the detector pointer
    detectors.push_back(new ScintillatorPaddle(name1,(G4int) 7,scintillator));
    // - Set the dimensions and position
    (detectors.back())->set_dimensions( G4ThreeVector(2.5*cm, 80.*cm, 15.*cm) );
    (detectors.back())->set_position(G4ThreeVector(0.5*detector_set_separation, 0.0,-2.5*15.1*cm ) ) ;
    // - Place the detectors in the right chamber volume
    (chambers.back())->set_physical_volume(
            (detectors.back())->Construct(
                    (chambers.back())->get_physical_volume()
                    )
         );


    //   DETECTOR #3
    // Create the name of the detector volume
    G4String name2("paddle12");
    // Create the detector pointer
    detectors.push_back(new ScintillatorPaddle(name2,(G4int) 8,scintillator));
    // - Set the dimensions and position
    (detectors.back())->set_dimensions( G4ThreeVector(2.5*cm, 80.*cm, 15.*cm) );
    (detectors.back())->set_position(G4ThreeVector(0.5*detector_set_separation, 0.0, -1.5*15.1*cm+7.5*cm ));
    // - Place the detectors in the right chamber volume
    (chambers.back())->set_physical_volume(
            (detectors.back())->Construct(
                    (chambers.back())->get_physical_volume()
                    )
         );


   /*
     for(G4int i=0; i<3;i++){

       // Create the name of the detector volume
       G4String name("paddle1");
       name += std::to_string((int) i);

       // Create the detector pointer
       detectors.push_back(new ScintillatorPaddle(name,(G4int) i+6,scintillator));
       // - Set the dimensions and position
       (detectors.back())->set_dimensions( G4ThreeVector(2.5*cm, 80.*cm, 15.*cm) );
       (detectors.back())->set_position(G4ThreeVector(0.5*detector_set_separation, 0.0, ((G4double) i-2.5)*15.1*cm + detector_adjustments.at(i)) );

       // - Place the detectors in the right chamber volume
       (chambers.back())->set_physical_volume(
               (detectors.back())->Construct(
                       (chambers.back())->get_physical_volume()
                       )
            );

       G4cout << " ** Detector " << i+6 << " \t " << G4ThreeVector(0.5*detector_set_separation, 0.0, ((G4double) i-2.5)*15.1*cm + vertical_offset) << G4endl;

   }
*/

   return worldPhysical;

}

void DetectorConstruction::ConstructSDandField()
{
    // Get the SD manager
    G4SDManager* SDman = G4SDManager::GetSDMpointer();

    sensitive_detectors.clear();
    // Create the sensitive detectors for one detector set
    for(G4int i=0; i<6;i++){

        G4String name("TrackerHitsCollection0");
        name += std::to_string((int) i);
        G4cout << " SDName : " << name << G4endl;

        sensitive_detectors.push_back(new ScintillatorSD((detectors.at(i))->get_sensitive_name(),name));

        SDman->AddNewDetector(sensitive_detectors.back());
        SetSensitiveDetector((detectors.at(i))->get_logical_volume(),sensitive_detectors.back());

    }

    // Create the sensitive detector for the other detector set
    for(G4int i=0; i<3;i++){

        G4String name("TrackerHitsCollection1");
        name += std::to_string((int) i);
        G4cout << " SDName : " << name << G4endl;

        sensitive_detectors.push_back(new ScintillatorSD((detectors.at(i+6))->get_sensitive_name(),name));

        SDman->AddNewDetector(sensitive_detectors.back());
        SetSensitiveDetector((detectors.at(i+6))->get_logical_volume(),sensitive_detectors.back());

    }

}
