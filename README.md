## Download

unpack the .app or executable from the archive format (.zip)

Skip to the **Install** instructions or simply run the unpacked executable

## Build

### Requirements
- Qt version 5+:  https://www.qt.io/
- c++11 or greater compliant compiler
 	- linux:      gcc
  - windows:    mingw or visual studio compiler
  - OS X:       clang

=======
*(note: this project can be imported into QtCreator and built from there which 
this is often the easiest way to build it)*

### Build
```
 qmake LongQt.pro
 make
 qmake LongQtGrapher.pro
 make
```
the executable/.app will usually be found in the folder where it was made
execpt on linux where it will be found in the build directory

*(note: if built with QtCreator the files will be in a build directory located
in the same directory as the source files are located
when using QtCreator on windows it is recomended to copy LongQt with all of
the other contents in the directory with it as they are often necessary for 
execution)*

## Install
copy the execuatable from ./build/ to a convenient location to run it
- on linux this would usually be ~/bin or /usr/local/bin
- on windows the Desktop is often convenient as the location is not important
- on Mac dragging the app into the Applications folder will work

## Documentation
The complete documentation can be found [here](http://longqt.readthedocs.io)

