# Downloading LongQT, Grapher and Grid Editor
Downloads for Mac and Windows can be found at https://hundlab.engineering.osu.edu/research/LongQt. The downloads should contain (.app)s or (.exe)s for each of the different LongQt components.

#Installation
##Windows
No installation is necessary. Move the executables to a continent location and run from there.

##Mac
Hold the control key and click on the application. Select "Open" and a prompt will ask you if you would like to run
an application from an unidentified developer select "Open" and LongQt should start. Do this for each of the apps.
To install drag LongQt (and Grapher and Grid Editor) into the Apps folder. Installation is not necessary but it will
allow LongQt to be run from a finder search.

#Installation from source
LongQt depends on the [Qt Application Framework] (https://www.qt.io/). There is an open source version of Qt that can
be used for free from their website, the commercial version should not be necessary. Any version of Qt beyond 5 should
work (version 5.9 was used for development). It is best to install QtCreator (which should be a part of the Qt downlaod).

##Windows and Mac
Clone recursively or download the github repository https://github.com/hundlab/LongQt. If you did not clone LongQt recursively then you still need the model repository. Get this by cloning or downloading LongQt-model from https://github.com/hundlab/LongQt-model. Then move the LongQt-model folder into the LongQt folder and rename it to modellib.
To use simply open QtCreator and select open existing project and open the .pro file located in the LongQt folder.
There should be a .pro file for LongQt, Grapher and Grid Editor.

## Linux
Install git and qt5 from your distributions repository.
On ubuntu to do this just run
```bash
apt-get install qt5-default git
```
Then clone recursively from https://github.com/hundlab/LongQt
```bash
git clone --recursive https://github.com/hundlab/LongQt.git
```
At this point open the project in qtcreator and select build or to build from the command line run
```bash
qmake LongQt.pro
make
```
To install find the compiled executable (from a qtcreator build it will be in the build directory specified by 
qtcreator, from a command line build it will be in LongQt/build) and move it to somewhere in the path 
(e.g. /usr/local/bin)

#Troubleshooting
For issues contact __ hundlab __
