# LongQt

LongQt is a user interface for heart cell models. It allows users to create and run
simulations of the electrical activity in single heart cells or in tissue. The aim of
LongQt is both to have a program which is useful for teaching new students trying to
understand the functions of heart cells as well as in the lab to easily run simulations
and experiments. To this end there are four components of LongQt.

*Hundlab Website: [hundlab.org](http://hundlab.org/)*

*Github: [github.com/hundlab/LongQt](https://github.com/hundlab/LongQt)*

*Documentation: [longqt.readthedocs.io](http://longqt.readthedocs.io)*

## Download

The latest executables can be downloaded and installed from
[our website](http://hundlab.org/)). The executables will
install LongQt and make it available to run.

## Developer Instructions

LongQt is built using cmake, and can be imported into QtCreator or any other IDE that
supports the cmake build system.

### Requirements

- Qt version 5.12:  https://www.qt.io/
  - windows: install the MSVC compiled version
- c++17 or greater compliant compiler
  - linux:      gcc
  - windows:    MSVC 2019
  - OS X:       clang
- cmake
- optional:
  - QtCreator

### Development Build

To start both the LongQt project and LongQt-model project need to be cloned from github into
the following directory structure

```
<parent_dir>/LongQt-model
<parent_dir>/LongQt
```

LongQt needs to be built with different build systems depending on the platform, for windows use MSVC 2019,
for linux use gcc (or clang), and for mac use clang. On windows MSCV 2019 can be installed as
a standalone package or as a part of the visual studio IDE. Qt 5.12 should be used, although
other versions of Qt will likely work as well.

To build LongQt, cmake needs to be told to look for LongQt-model as a subdirectory, this is why
the directory structure needs to follow the description above. When running cmake, specify
`LongQt_subdir:BOOL=TRUE`. If using QtCreator this can be done through the projects tab, if using
the command line, it can be specified as an argument to cmake `-DLongQt_subdir:BOOL=TRUE`.
When LongQt is built, it will also produce executables for LongQt-Grapher and LQ-GridEditor.

To install LongQt add the install target to the build process (in QtCreator) or run cmake
with the install target (for command line). It is often best to set the install directory
manually, I install into a install subdirectory in my home folder. Just set the
CMAKE_INSTALL_PREFIX, and ensure that it ends in LongQt. Installation is not particularly
necessary for LongQt development as it can be run directly from the build directory using
QtCreator (or any other IDE). To run LongQt-Grapher or LQ-GridEditor in QtCreator, change the
run option in the projects tab. For creating the installer used to distribute LongQt, see
the [github.com/hundlab/LongQt-Installer](https://github.com/hundlab/LongQt-Installer)
repository.
