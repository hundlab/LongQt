# Adding a cell electrophysiology model to LongQt

## Getting started
To add a new model LongQt first needs to be installed from source, the installing tab has instructions for how to
do this on Windows, Mac and Linux.

## Creating the new model files
The important files for adding a model can be found in LongQt/modellib or in qtcreator it will be LongQt/model in the
projects view. The cell directory contains all the cell models. To create a new one add a header and source file
to the cells directory. If done through qt creator then no further action will be necessary, however if they are added
some other way open the model.pri file found in the model folder and add an entry to both HEADERS and SOURCES.
The entries can be added to somewhere in the middle and will look something like this:
```
SOURCES +=
...
$$PWD/cell/yourfile.cpp\
...

HADERS +=
...
$$PWD/cell/yourfile.h\
...
```

## The new cell's class
New cells are implemented as as classes which inherit from the base cell classes (named Cell & CellKernel). The code for the base cell class can
be found in cell_kernel.(h/cpp) and cell.(h/cpp) though only cell.h must be included in the new cell file (do 
not attempt to only inherit from cell_kernel instead inherit from cell.) The code for the new class should contain
these lines

### yourfile.h
```c++
#ifndef NEWCELL
#define NEWCELL

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
	virtual const char* type() const;
protected:
	// helpful functions to have
	void Initialize();
private:
	void mkmap();
}

#endif
```

### yourfile.cpp
```c++
NewCell::NewCell()
{
    this->Initialize();
};
NewCell::NewCell(NewCell& toCopy) : Cell(toCopy){
    this->Initialize();
    // any needed copy logic
    CellKernel::copyVarPar(toCopy);
}
NewCell* NewCell::clone(){
    return new NewCell(*this);
}
void NewCell::Initialize(){
//constructor logic
}
```
To find the functions original entries read cell_kernel.h. In the design pattern we use Initialize() contains all of the
initial values for the model and does all other work needed in the constructor. Initialize will then be called in both
the default constructor NewCell() and the copy constructor NewCell(NewCell&). clone() is then built off of the copy
constructor to return a new copy of NewCell. CellKernel::copyVarPar(CellKernel&) helps to copy the internal state of your
new cell using two maps named vars and pars. It is important that you add your variables to these maps so that the model
can be correctly copied and used. map<string,double*> vars (short for variables) is used  things that change 
throughout the course of a simulation such as voltage or ion concentrations. map<string,double*> pars (short for
parameters) are things that do not change with in a simulation but may change between simulations such as temperature
or faraday's constant. Some variables and parameters have already been added to vars and pars in CellKernel::mkmap().
To change the values as set in CellKernel set them in your Initialize() function. Similarly to CellKernel we recommend
adding your variables and parameters to vars and pars in a separated function mkmap called from Initialize(). 

The final two necessary functions involve updating the model, they are updateCurr
and updateConc and which update currents and update concentrations, respectively. They will be called
during the simulation as updateCurr, apply stimulus if appropriate, updateConc, update voltage (vOld). After running
these functions the model should be up to date for the current time-step. The final function which is often useful to
override is the externalStim function which applied the stimulus to the cell. There is a default implementation which
is useful for most cells, but sometimes other currents need to be added to the stimulus (such as a K current to avoid
baseline drift). 

The name that will be used for your cell in LongQt and in the settings files is defined by the type() function.
Implement it as:
```c++
const char* NewCell::type() const {
	return "Canine Ventricular (Hund-Rudy 2009)";
}
```
Our names are the species, the cell type followed by the model name (authors of model paper and the year it was published).

## Final steps
Finally to add the cell to the list of cell models known to LongQt open LongQt-model/cellutils.cpp. This contains a map from
cell name to its constructor. Add this line
```c++
const map<string, CellUtils::CellInitializer> CellUtils::cellMap = {
...
{ NewCell().type(), [] () {return make_shared<NewCell>();}}
...
```
to the map and the cell should be added.

To add default settings for the simulation you can add an entry to the map further down as
```c++
const map<string, list<pair<string,string>>> CellUtils::protocolCellDefaults = {
...
    { NewCell().type(), {{"paceflag","false"},{"stimval","-60"},{"stimdur","1"},
                            {"tMax","500000"},{"writetime","495000"},{"bcl","1000"},
                            {"numstims","500"}}},
...
```

## Extras
Often cell models use flags to determine if the model should simulate the presence of some drug or genetic mutation e.g.
isoproterenol. To add these flags to LongQt include this line in your header file.
```c++
//    MUTA //genetic mutant A
//    MUTB //genetic mutant B
//    ISO //isoproterenol
MAKE_OPTIONS(MUTA,MUTB,ISO)

enum Options opts;
```
Then add these lines to your source (cpp) file.
```c++
void NewCell::Initialize() {
	opts = WT;
	conflicts = {{MUTA,MUTB}};
}

MAKE_OPTIONS_FUNCTIONS(NewCell)
```
This will define an enum with values for each of your flags and an additional WT flag. The flags are bit flags which means
that opts can be set to multiple flags at once by or-ing them together. e.g. NewCell::setOption(ISO|MUTA). To prevent certain
options from being set at the same time use the conflicts variable and add set of mutually exclusive variables. In this example
MUTA and MUTB cannot be set at the same time. Here is a list of functions that can be used to get and set the options:
```c++
virtual map<string,int> optionsMap() const;
virtual int option() const;
virtual string optionStr() const;
virtual void setOption(string opt);
virtual void setOption(int opt);
```
The declarations and for these functions are given by the MAKE_OPTIONS and MAKE_OPTIONS_FUNCTIONS macros. The definitions of these macros
can be found in cell_kernel.h. 
Finally, to check if an option is set:
```c++
if(opts & ISO) {
...
}
```

## Documentation
To add hover texts and units to the model go to Resources/LongQt.qrc/hoverText in qtcreator or to LongQt/hoverText in
the file system. The files of interest are dvarsDescriptions.txt, dvarsUnits.txt and pvarsDescriptions.txt. For any
variables who's names are not in dvarsDesciptions and dvarsUnits add them to the end and then on the following line add
their description or units respectively. For parameters add them in the same way to pvarsDescriptions.

