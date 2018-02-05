#ifndef TunnelChamber_H
#define TunnelChamber_H

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
#include <string>

class TunnelChamber
{
    public:
        TunnelChamber( ){};
        TunnelChamber( G4String name, G4int id, G4Material *mat);
        virtual ~TunnelChamber();

        void set_name(G4String new_name)
        {
            tunnel_name = new_name;
            G4cout << " NAME : "  << tunnel_name << G4endl;
            set_physical_name();
            set_logical_name();
            set_solid_name();
        }

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

        void set_physical_volume(G4VPhysicalVolume* pv){physical_volume=pv;}

    protected:


    private:

        void set_physical_name(){ physical_name = G4String("physical_") + tunnel_name;}
        void set_logical_name(){ logical_name = G4String("logical_") + tunnel_name;}
        void set_solid_name(){ solid_name = G4String("solid_") + tunnel_name;}

        G4ThreeVector dimensions;
        G4ThreeVector position;

        G4Box* solid_volume;
        G4LogicalVolume* logical_volume;
        G4VPhysicalVolume* physical_volume;

        G4Material* tunnel_material;

        G4int id;
        G4String tunnel_name;
        G4String physical_name;
        G4String logical_name;
        G4String solid_name;
};


#endif // TunnelChamber_H
