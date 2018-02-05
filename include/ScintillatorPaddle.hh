#ifndef SCINTILLATORPADDLE_H
#define SCINTILLATORPADDLE_H

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UIcommand.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "globals.hh"
#include "G4String.hh"
#include "G4SDManager.hh"
#include "ScintillatorSD.hh"
#include <string>

class ScintillatorPaddle
{
    public:
        ScintillatorPaddle( ){};
        ScintillatorPaddle( G4String name, G4int id, G4Material *mat);
        virtual ~ScintillatorPaddle();

        void set_name(G4String new_name)
        {
            paddle_name = new_name;
            G4cout << " NAME : "  << paddle_name << G4endl;
            set_physical_name();
            set_logical_name();
            set_solid_name();
            set_sensitive_name();
        }

        void SetSensitive();


        void set_position(G4ThreeVector new_pos){
            G4cout << " new pos : " << new_pos;
            position = new_pos;
        }
        void set_dimensions(G4ThreeVector dims){dimensions = dims;}

        G4ThreeVector get_dimensions(){return dimensions;}
        G4ThreeVector get_position(){return position;}

        G4VPhysicalVolume* Construct(G4VPhysicalVolume *world_physical);
        G4LogicalVolume* get_logical_volume(){return logical_volume;}
        G4VPhysicalVolume* get_physical_volume(){return physical_volume;}

        const G4String get_sensitive_name(){return sensitive_name;}


    protected:


    private:

        void set_physical_name(){ physical_name = G4String("physical_") + paddle_name;}
        void set_logical_name(){ logical_name = G4String("logical_") + paddle_name;}
        void set_solid_name(){ solid_name = G4String("solid_") + paddle_name;}
        void set_sensitive_name(){ sensitive_name = G4String("sensitive_") + paddle_name;}


        G4ThreeVector dimensions;
        G4ThreeVector position;

        G4Box* solid_volume;
        G4LogicalVolume* logical_volume;
        G4VPhysicalVolume* physical_volume;

        G4VSensitiveDetector* sensitive_detector;


        G4Material* detector_material;

        G4int id;
        G4String paddle_name;
        G4String physical_name;
        G4String logical_name;
        G4String solid_name;
        G4String sensitive_name;
};


#endif // SCINTILLATORPADDLE_H
