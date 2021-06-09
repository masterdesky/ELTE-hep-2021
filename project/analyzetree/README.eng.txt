How to install

LOAD THE ROOT FRAMEWORK:
source /path_to_install_or_build_dir/bin/thisroot.sh 

COMPILE:
mkdir exe/
mkdir exe/object/
mkdir exe/object/dependencies/
make exe/analyzetree.exe
(there is an m32 switch in the makefile, you may need to remove it if your system and your ROOT install are both 64 bit)

DATA NEEDED TO RUN:
download the root datafile (to the same folder)
https://www.phenix.bnl.gov/phenix/WWW/publish/csanad/analyzetree/data.root

RUN:
exe/analyzetree.exe <input filename> <output filename> <No. events to analyze>
e.g
exe/analyzetree.exe data.root analyzetree.root 1000

PLOT:
root.exe -b -q Plot_analyzetree.C\(\"analyzetree.root\",\"figs\") 

RESULTS:
Plots in the figs folder

A FEW DETAILS ABOUT THE DATA:

Event-characteristics:
- Zvertex: place of the given collision within the beam,+- few centimeters from the collision point, it is useful to have a look at the distribution
- Centrality: the centrality of the given event (the degree of overlap of the nuclei); 0% is the fully central, 100% is the fully peripheral event. The n% means, that n percent of the events are more central than the given event. It is also useful to look at this distribution.
- ReactionPlane: the direction of the reaction plane of the given event. It is the angle of the plane that contains the position and momentum vector of the two nuclei with regards to the x-z plane in the lab-frame. It is also worth to check out the distribution.
- Ntracks: Number of tracks in the given event. It is also useful to look at it's distribution.

Particle properties (you should also check out all of these distributions):
- px,py,pz,E: four-momentum (from this you can calculate pt=sqrt(px^2+py^2) or m=sqrt(E^2-px^2-py^2-pz^2))
- ch: charge
- Mch: matching variable, more about this later
- isPi: PID variable, more about this later
- detp: phi coordinate in the detector that is used for particle identification 
- detz: z coordinate in the detector that is used for particle identification 

MORE DETAILS:
The detector used for the measurement looks like this:
https://www.phenix.bnl.gov/WWW/run/drawing/Phenix_2012.jpg
You can see the multi-layered structure. Here is an old animation:
http://www.phenix.bnl.gov/WWW/software/luxor/ani/PHENIXRun2/phenixRun2Large.mpg
This is how the data-taking works:
https://www.phenix.bnl.gov/phenix/WWW/intro/dataflow/index.html

Actually we have hits in different detector layers, which are used by the "tracking" algorithm. It fits a track (which do not go through the points exactly, just approximately, because of course the position of these points are not perfectly measured). This is called reconstruction, we get the observed tracks from this. 

The quality of these tracks are different, because the tracking algorithm of course cannot be perfect. We can approximate how good the quality of a track is by for example checking how far is the track projection in a given layer from the nearest hit. The distribution of these distances will be approximately Gaussian, with sigma width. We can tell for every track how many sigmas is the distance from the nearest actual hit - this is what we call matching variable which can be defined for every detector layer. 

The particle ID is also somewhat similar. In PHENIX particularly we use the TimeOfFlight method. The essence of this is that time(TOF) = distance/velocity, and between the velocity and the momentum the connection is the mass, so the relativistic equation is m^2 = p^2 * (1-v^2)/v^2). From this we can create a TOF vs. momentum plot like this:
https://www.phenix.bnl.gov/WWW/run/drawing/tofpid_0800_02.gif
From this the mass vs. momentum looks like this:
https://www.phenix.bnl.gov/WWW/run/drawing/mass2tof.gif
On this we can see the pi,K,p particles nicely separated, and that the masses have a Gaussian distribution around the theoretical value. This can also be parametrized and we can give the sigma(p) momentum dependent width. For a given particle we can tell, how many sigmas are the distance from the nominal mass. This will be the isPi variable (which is of course rounded to integer or half integer values here)

