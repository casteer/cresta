#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H


#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "G4Box.hh"
#include "ScintillatorPaddle.hh"
#include "TunnelChamber.hh"
#include "DetectorMessenger.hh"

#include "G4RunManager.hh"
#include "G4GeometryManager.hh"

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        DetectorConstruction();
        virtual ~DetectorConstruction();

        virtual G4VPhysicalVolume* Construct();
        void ConstructChamber(int);


        void ConstructSDandField();

        std::vector<ScintillatorPaddle*> detectors;
        std::vector<ScintillatorSD*> sensitive_detectors;


        std::vector<G4Material*> chamber_materials;
        std::vector<G4ThreeVector> chamber_positions;
        std::vector<G4ThreeVector> chamber_sizes;

        std::vector<TunnelChamber*> chambers;

        void ClearChambers(){
            chamber_materials.clear();
            chamber_positions.clear();
            chamber_sizes.clear();
           // chambers.clear();
        }

        void SetChamberMaterial(G4int i, G4String mat){

            if(i<chamber_materials.size()){
                chamber_materials[i] = G4Material::GetMaterial(mat);
            }else{
                chamber_materials.push_back(G4Material::GetMaterial(mat));
            }

            // (Re-)initialise this chamber
            G4cout << "** Materials " << chamber_materials.size() << G4endl;
            G4cout << "** Boxes " << chamber_sizes.size() << G4endl;
            G4cout << "** Positions " << chamber_positions.size() << G4endl;

            if( (chamber_materials.size()==chamber_sizes.size()) && (chamber_positions.size()==chamber_sizes.size()) )
                ConstructChamber(i);
        };

        void SetChamberVolume(int i, G4ThreeVector chamber_size){

            if(i<chamber_sizes.size()){
                chamber_sizes[i] = chamber_size;
            }else{
                chamber_sizes.push_back(chamber_size);
            }

            // (Re-)initialise this chamber
            G4cout << "** Materials " << chamber_materials.size() << G4endl;
            G4cout << "** Boxes " << chamber_sizes.size() << G4endl;
            G4cout << "** Positions " << chamber_positions.size() << G4endl;

            if( (chamber_materials.size()==chamber_sizes.size()) && (chamber_positions.size()==chamber_sizes.size()) )
                ConstructChamber(i);

        };
        void SetChamberPosition(int i, G4ThreeVector chamber_pos){

            if(i<chamber_positions.size()){
                chamber_positions[i] = chamber_pos;
            }else{
                chamber_positions.push_back(chamber_pos);
            }

            // (Re-)initialise this chamber
            G4cout << "** Materials " << chamber_materials.size() << G4endl;
            G4cout << "** Boxes " << chamber_sizes.size() << G4endl;
            G4cout << "** Positions " << chamber_positions.size() << G4endl;

            if( (chamber_materials.size()==chamber_sizes.size()) && (chamber_positions.size()==chamber_sizes.size()) )
                ConstructChamber(i);

        };


        void SetWorldBox(G4Box* wb){
            world_box = wb;
            G4cout << "** World box : " << *world_box << G4endl;

            // Change the geometry
            G4GeometryManager* gm = G4GeometryManager::GetInstance();
            gm->OpenGeometry(worldPhysical);

            // Change the world box
            worldPhysical->GetLogicalVolume()->SetSolid(world_box);

            // Close geometry for the portion modified ...
            gm->CloseGeometry(worldPhysical);

            // Needed to restart the production cuts
            (G4RunManager::GetRunManager())->GeometryHasBeenModified();
        };


        void SetWorldMaterial(G4String s){
            world_material = G4Material::GetMaterial(s);
            G4cout << "** World material : " << world_material << G4endl;

            // Change the geometry
            G4GeometryManager* gm = G4GeometryManager::GetInstance();
            gm->OpenGeometry(worldPhysical);

            // Change the world box
            worldPhysical->GetLogicalVolume()->SetMaterial(world_material);

            // Close geometry for the portion modified ...
            gm->CloseGeometry(worldPhysical);

            // Needed to restart the production cuts
            (G4RunManager::GetRunManager())->GeometryHasBeenModified();

        };

        void SetDetectorPosition(G4int i, G4ThreeVector p){
            // Change the geometry
            G4GeometryManager* gm = G4GeometryManager::GetInstance();

            // Set the detector's position
            gm->OpenGeometry(detectors.at(i)->get_physical_volume());
            detectors.at(i)->get_physical_volume()->SetTranslation(p);
            gm->CloseGeometry(detectors.at(i)->get_physical_volume());

            // Needed to restart the production cuts
            (G4RunManager::GetRunManager())->GeometryHasBeenModified();

        }


        void SetDetectorRotation(G4int i, G4ThreeVector p){

            G4RotationMatrix *r = new G4RotationMatrix();
            r->rotateX(p.getX()*deg);
            r->rotateY(p.getY()*deg);
            r->rotateZ(p.getZ()*deg);

            // Change the geometry
            G4GeometryManager* gm = G4GeometryManager::GetInstance();

            // Set the detector's rotation
            gm->OpenGeometry(detectors.at(i)->get_physical_volume());
            detectors.at(i)->get_physical_volume()->SetRotation(r);
            gm->CloseGeometry(detectors.at(i)->get_physical_volume());

            // Needed to restart the production cuts etc
            (G4RunManager::GetRunManager())->GeometryHasBeenModified();

        }

        void SetDetectorStandOff(G4double d){

            detector_set_separation=d;
            G4cout << "** Setting separation distance to be " << d << G4endl;

            // Change the geometry
            G4GeometryManager* gm = G4GeometryManager::GetInstance();

            for(int i=0; i<6;i++){
                gm->OpenGeometry(detectors.at(i)->get_physical_volume());
                detectors.at(i)->get_physical_volume()->SetTranslation(G4ThreeVector(-0.5*detector_set_separation, 0.0, ((G4double) i-2.5)*15.1*cm ));
                gm->CloseGeometry(detectors.at(i)->get_physical_volume());
            }

            for(int i=0; i<3;i++){
                gm->OpenGeometry(detectors.at(i+6)->get_physical_volume());
                detectors.at(i+6)->get_physical_volume()->SetTranslation(G4ThreeVector(0.5*detector_set_separation, 0.0, ((G4double) i-2.5)*15.1*cm));
                gm->CloseGeometry(detectors.at(i+6)->get_physical_volume());
            }

        };

        void PrintDetectorInfo(){
            G4cout << "** ======================== " << G4endl;
            G4cout << "** Detector translations... " << G4endl;
            G4cout << "** ======================== " << G4endl;
            for(int i=0; i<9;i++) G4cout << "** - Detector " << i << " : " << detectors.at(i)->get_physical_volume()->GetTranslation() << G4endl;

            G4cout << "** ======================== " << G4endl;
            G4cout << "** Detector rotations... " << G4endl;
            G4cout << "** ======================== " << G4endl;
            for(int i=0; i<9;i++) {
                G4cout << "** - Detector " << i << " : " << G4endl;
                G4RotationMatrix *R = (detectors.at(i)->get_physical_volume()->GetRotation());
                if(R!=nullptr){
                    G4cout << R->rowX() << G4endl;
                    G4cout << R->rowY() << G4endl;
                    G4cout << R->rowZ() << G4endl;
                    G4cout << G4endl;
                }else{
                    G4cout << 0 << G4endl;
                }
            }

            G4cout << "** ======================== " << G4endl;
        }


    private:

        int system_in_chamber;// index of chamber that contains the detector system
        G4double detector_set_separation;
        G4ThreeVector system_offset;
        G4Box *world_box;
        G4Material* world_material;

        G4VPhysicalVolume* worldPhysical;
        G4LogicalVolume* worldLogical;
        G4VSolid* worldSolid;

        DetectorMessenger* DetMessenger;


};

#endif // DETECTORCONSTRUCTION_H
