# CRESTA : Cosmic Rays for Environmental Science and Technology Applications 

This is a GEANT4 application to simulate the interaction of sea-level cosmic rays with the natural environment. 

## Requirements

Tested under Ubuntu 17.10 using GCC version 7.2 with the following software installed (with their dependencies): 

* GEANT4.10.03.p03
* ROOT 6.10.08


## Developing New Geometries 

New geometries can be developed within CRESTA. It is best if you copy an existing example macro file and then use that as the basis for your simulation geometry. The steps are 

* Run ./cosmic in order to boot up the Qt geometry viewer
* In your macro, comment out any lines where you call /run/beamOn 
* Modify your script 
* In the Qt viewer, run /control/execute [YOUR_SCRIPT_FILENAME]

Repeat the last two steps as needed. 

### Helpful visualization options 

When transporting particles through large amounts of rock, a lot of secondary particles can be produced. It really helps the viewer if these are removed from the Qt viewer visualization. To do this, add these lines to your script: 

> # =========================================
> /vis/ogl/set/displayListLimit 10000000
>
> /vis/filtering/trajectories/create/particleFilter CRShowerFilter
>
> /vis/filtering/trajectories/CRShowerFilter/add gamma
>
> /vis/filtering/trajectories/CRShowerFilter/invert true
>
> /vis/filtering/trajectories/CRShowerFilter/add e+
>
> /vis/filtering/trajectories/CRShowerFilter/invert true
>
> /vis/filtering/trajectories/CRShowerFilter/add e-
>
> /vis/filtering/trajectories/CRShowerFilter/invert true
>
> /vis/filtering/trajectories/CRShowerFilter/add pi+
>
> /vis/filtering/trajectories/CRShowerFilter/invert true
>
> /vis/filtering/trajectories/CRShowerFilter/add pi-
>
> /vis/filtering/trajectories/CRShowerFilter/invert true
>
> /vis/scene/add/axes 50 0 0 10 m
>
> # =========================================

The last command adds axes which may help. 


### Output ROOT Tree Description 


