// 5/10/2016
//################################################
// a 2D grid of cells of various types
// class declaration
//################################################

#ifndef CELLGRID_H
#define CELLGRID_H

#include <set>
#include <list>
#include <array>
#include <string>
#include <cmath>
#include <memory>

#include "fiber.h"
#include "cellutils.h"

struct CellInfo {
	//necessary
    CellInfo(int X=-1, int Y=-1, double dx=0.01, double dy=0.01, int np=1,
        shared_ptr<Cell> cell=0, array<double,4> c={NAN,NAN,NAN,NAN},
        bool c_perc=false) {
        this->X = X;
        this->Y = Y;
        this->dx = dx;
        this->dy = dy;
        this->np = np;
        this->cell = cell;
        this->c = c;
        this->c_perc = c_perc;
    }
    ~CellInfo() {}
    int X = -1;
    int Y = -1;
    double dx = 0.01;
    double dy = 0.01;
    int np = 1;
	//if cell == NULL then cell will not be changed
    shared_ptr<Cell> cell = 0;
	array<double,4> c = {NAN,NAN,NAN,NAN};
	bool c_perc = false;
};

class Grid {
  public:

    Grid();
    Grid(const Grid& other);
    ~Grid();

    typedef std::vector<Fiber>::const_iterator const_iterator;

//	inline virtual edge(int x, int y, CellUtils::Side s);
    virtual void addRow(int pos); //create new row at 0 <= pos < len of empty cells
    virtual void addRows(unsigned int num, int position = 0);
    virtual void addColumn(int pos); //same but for culumns
    virtual void addColumns(unsigned int num, int position = 0);
    virtual void removeRow(int pos);
    virtual void removeRows(unsigned int num, int position = 0);
    virtual void removeColumn(int pos);
    virtual void removeColumns(unsigned int num, int position = 0);
    virtual void setCellTypes(list<CellInfo>& cells);
    virtual void setCellTypes(const CellInfo& singleCell);
    virtual int rowCount();
    virtual int columnCount();
    virtual pair<int,int> findNode(const Node* node);
    virtual shared_ptr<Node> operator()(const pair<int,int>& p);
    virtual shared_ptr<Node> operator()(const int x, const int y);
	virtual void reset();
	virtual void updateB(CellInfo node, CellUtils::Side s);

    virtual const_iterator begin() const;
    virtual const_iterator end() const;

    vector<Fiber> fiber;
    vector<Fiber> fibery;
};
#endif
