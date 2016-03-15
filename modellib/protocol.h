//################################################
// This header file contains protocol class
// declaration for simulation of excitable cell activity.
//
// Copyright (C) 2015 Thomas J. Hund.
// Updated 07/2015
// Email thomas.hund@osumc.edu
//#################################################

#ifndef PROTO
#define PROTO

#include <sstream>
#include <random>
#include <exception>
#include <string>
#include <list>
#include <unordered_map>

#include "kurata08.h"
#include "measure.h"
#include "output.h"

using namespace std;

typedef Cell* (*CellInitializer)(void);

struct GetSetRef {
    function<string(void)> get;
    function<void(const string&)> set;
    string type;
    GetSetRef Initialize(string type, function<string(void)> get, function<void(const string&)> set) {
        this->type = type;
        this->get = get;
        this->set = set;
        return *this;
    }
};

class Protocol
{
  public:
    Protocol();
    Protocol(const Protocol& toCopy);
    Protocol(Protocol&& toCopy);
    Protocol& operator=(const Protocol& toCopy);
    ~Protocol();
  
    //##### Declare class functions ##############
    virtual int stim();
    virtual int assign_cell_pars(vector<string> pnames, vector< vector<string> > pvals, int trialnum);
    virtual map<string, double*> resizemap(map<string,double*> varmap, string file);
    virtual map<string, double*> resizemap(map<string,double*> varmap, vector<string> names);
    virtual tuple< vector<string>, vector< vector<string> > > parse2Dmap(map<string,double*> varmap,map<string,double*> varmap2, string file);
    virtual tuple< vector<string>, vector< vector<string> > > parsemixedmap(map<string,double*> varmap, string file);
    virtual map<string,double> copymapvals(map<string, double*> varmap);
    virtual int map2screen(map<string, double*> varmap);
    virtual int map2screen(map<string, double> varmap);
    virtual int getNeededDOutputSize(); //get the size needed to construct the output array
    virtual int runSim();
    virtual bool runTrial();
    virtual int readpars(string file);
    virtual int parsemixedmap(map<string,double*> varmap, string file, vector<string>* cnames, vector<vector<string>>* twoDrnames);
    virtual int parse2Dmap(map<string,double*> varmap,map<string,double*> varmap2, string file, vector<string>* vnames, vector< vector<string> >* twoDmnames);
    virtual int resizemap(map<string,double*> varmap, string file, map<string, double*>* vars);
    virtual int readpvars();
    virtual bool writepars(string file); //write the contence of pars to a file
    virtual bool writedvars(string file); //write varmap keys to a file
    virtual bool readdvars(string file);
    virtual bool write2Dmap(vector<string> vnames, vector< vector<string> > twoDmnames, string file);
    virtual void setTrial(unsigned int current_trial);
    virtual unsigned int getTrial();
    virtual int parse2Dmap(map<string,double*> varmap,set<string> vars2, string file, vector<string>* vnames, vector< vector<string> >* twoDmnames);
    virtual bool writeMVarsFile(string file);
    virtual bool readMvarsFile(string filename);
    virtual bool setCell(string type, bool reset = false);
    virtual list<string> cellOptions();

    //##### Declare class variables ##############
    Cell* cell;        // pointer to cell class
    Output* douts;     // point to output class for writing data
    Output *ics;        // Output for saving SVs at end of sim.
    double vM;         // membrane potential, mV
    double time;       // time, ms
    //##### Declare class params ##############
    double bcl,stimval,stimdur,stimt;
    int numstims;   //variables for pacing.
    double meastime,writetime;
    double writeint;
    double doneflag;
    bool readflag,saveflag,writeflag,measflag,paceflag;
    int numtrials;
    bool stimflag;
    double stimcounter;
    int writestd;    
    double tMax;
    double maxdoutsize,maxmeassize;
    
    default_random_engine generator;
    
    string readfile,savefile,dvarfile,pvarfile, measfile, simvarfile, propertyoutfile, dvarsoutfile, finalpropertyoutfile, finaldvarsoutfile;

    string datadir;

    vector<string> pnames;              // stores cell param names
    vector< vector<string> > pvals;     // stores cell param vals
 
    
    //##### Declare maps for vars/params ##############
    map<string, GetSetRef> pars;
    map<string, double*> parmap; // map for output params
    map<string, double*> datamap; // map for output state vars

    const map<string,Measure>& Measures = cref(measures);
    bool addMeasure(Measure toInsert);
    void removeMeasure(string measureName);
    bool setMeasures(map<string,Measure> newMeasures);
    bool addToMeasreSelection(string measureName, string property);
    void removeFromMeasureSelection(string measureName, string property);

    private:
    unsigned int trial;
    map<string,Measure> measures; // set of measure class for measuring SV props.

    protected:
    void copy(const Protocol& toCopy);    
    map<string, CellInitializer> cellMap;

};

#endif
