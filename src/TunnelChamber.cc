#include "TunnelChamber.hh"



TunnelChamber::TunnelChamber(G4String chamber_name, G4int id, G4Material *mat)
{
    // Set the chamber ID (must be called before set_name)
    id = id;

    // Set the names
    set_name(chamber_name);

    // Set default sizes and positions
    dimensions = G4ThreeVector(4*cm,15*cm,80*cm);
    position = G4ThreeVector(0.0,0.0,0.0);

    // Set the material
    tunnel_material = mat;

    // Construct
    //    worldPhysical = Construct(world_logical);

}




TunnelChamber::~TunnelChamber()
{
    //dtor
}


G4VPhysicalVolume* TunnelChamber::Construct(G4VPhysicalVolume *world_physical)
{

    G4LogicalVolume* world_logical = world_physical->GetLogicalVolume();

    // Create the box
    solid_volume= new G4Box(solid_name,0.5*dimensions.getX(),0.5*dimensions.getY(),0.5*dimensions.getZ());

    // Create the logical volume
    logical_volume = new G4LogicalVolume(solid_volume,tunnel_material,logical_name);

   // Create the physical volume
   physical_volume =  new G4PVPlacement(0,position,logical_volume,physical_name,world_logical,false,id,G4bool(true));


return world_physical;

}

