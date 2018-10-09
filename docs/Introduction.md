# Introduction and Overview

LongQt is a user interface for heart cell models. It allows users to create and run simulations of the electrical
activity in single heart cells or in tissue. The aim of LongQt is both to have a program which is useful for teaching
new students trying to understand the functions of heart cells as well as in the lab to easily run simulations
and experiments. To this end there are four components of LongQt.

1. LongQt (main) - this is where simulations are designed, run and visualised. It contains the core application
	and is the best place to start for new users.
2. LongQt Grapher - the visualisation components from LongQt for viewing previously run simulations
3. LongQt GridEditor - this is for creating more advanced grid simulations. While LongQt (main) can also create and
	run grid simulations GridEditor provides more control over the cells and how they are connected.
4. PyLongQt - while PyLongQt is not apart of this project or documentation it is worth noting that LongQt has bindings
	so simulations can be built and run in Python 3. It is very useful when more customization and control is 
	required than is provided in LongQt. For more info see [PyLongQt's Github page](https://github.com/hundlab/PyLongQt)

LongQt supports most major operating systems including Windows, Mac OSX and Linux. Instructions for installing can be found under
the getting tab.

