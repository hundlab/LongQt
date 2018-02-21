# Adding a cell electrophysiology model to LongQt

## Getting started
To add a new model LongQt first needs to be installed from source, the installing tab has instructions for how to
do this on Windows, Mac and Linux.

## Creating the new model files
The important files for adding a model can be found in LongQt/modellib or in qtcreator it will be LongQt/model in the
projects view. The cell directory contains all the cell models. To create a new one simply add a header and source file
to the cells directory. If done through qt creator then no further action will be necessary, however if they are added
some other way simply open the model.pri file found in the model folder and add an entry to both HEADERS and SOURCES.
The entries can be added to somewhere in the middle and will look something like this:
```
$$PWD/cell/yourfile.cpp\

$$PWD/cell/yourfile.h\
```

## The new cell's class
New cells are implemented as as classes which inherit from the base cell class. The code for the base cell class can
be found in cell_kernel.(h/cpp) and cell.(h/cpp) though only cell.h must be included in the new cell file (i.e. do 
not attempt to only inherit from cell_kernel instead inherit from cell.) The code for the new class should contain
these lines
```c++
#include "cell.h"
class NewCell : public Cell {
	// minimum necessary functions to implement
	NewCell();
	NewCell(const NewCell&);
	virtual NewCell* clone();

	virtual void updateCurr();
	virtual void updateConc();
	// other potentially helpful functions to override
	virtual int externalStim(double stimval);
protected:
	void Initialize();
	// helpful functions to have
private:
	void mkmap();
}
```
All of these functions come from cell_kernel.h so it will be quite helpful to look at that file as well. The functions
which are minimally necessary to inherit from Cell are the pure virtual functions. These functions have no definition
in CellKernel or Cell and thus must be implement in your NewCell class. NewCell(), and NewCell(const NewCell&) are the
different kinds of constructors used by LongQt. In addition LongQt requires a clone() function which is similar to
the copy constructor but is more broadly useful than a copy constructor. It is also recommended to create an Initialize
function which contains the core constructor logic so that it can be reused in multiple constructors.
```c++
NewCell::NewCell()
{
    this->Initialize();
};
NewCell::NewCell(NewCell& toCopy) : Cell(toCopy){
    this->Initialize();
    CellKernel::copyVarPar(toCopy);
}
NewCell* NewCell::clone(){
    return new NewCell(*this);
}
void NewCell::Initialize(){
//constructor logic
}
```
The copy function in this case should be capable of copying the cell well enough to be able to continue the simulation
in place of the original. To do this in the simplest way possible CellKernel contains two maps: vars and pars 
(variables and parameters). Variables is for things that change, and things that would be interesting to output as this
is the map used in the Select Outputs menu. Parameters are for the constants in the model which are not changed in the
simulation. Optionally, if there is something in the model which needs to be copied for a copy of the model to function
properly but would not be interesting to track it can be added to pars. To do this in existing models we use a private
function mkmap which is called from Initialize, but this is primary for readability and is not functionally necessary.
Another note when dealing with variables, some variables are already provided in CellKernel. It is important for
correct functionality that if a variable/parameter representing the same thing exists in the new model it be replaced 
with the given variable/parameter. The final two necessary functions involve updating the model, they are updateCurr
and updateConc and their function is to update currents and update concentrations, respectively. They will be called
during the simulation as updateCurr, apply stimulus if appropriate, updateConc, update voltage (vOld). After running
these functions the model should be up to date for the current time-step. The final function which is often useful to
override is the externalStim function which applied the stimulus to the cell. There is a default implementation which
is useful for most cells, but sometimes other currents need to be added to the stimulus for proper function. 

## Final steps
Finally to add the cell to the list of cell models known to LongQt open model/cellutils.cpp. This contains a map from
cell name to its constructor. Add this line
```c++
{ NewCell().type(), [] () {return make_shared<NewCell>();}}
```
to the map and the cell should be added.

## Documentation
To add hover texts and units to the model go to Resources/LongQt.qrc/hoverText in qtcreator or to LongQt/hoverText in
the file system. The files of interest are dvarsDescriptions.txt, dvarsUnits.txt and pvarsDescriptions.txt. For any
variables who's names are not in dvarsDesciptions and dvarsUnits add them to the end and then on the following line add
their description or units respectively. For parameters add them in the same way to pvarsDescriptions.

