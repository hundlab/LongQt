//################################################
// This code file contains protocol class
// definition for simulation of excitable cell activity.
//
// Copyright (C) 2015 Thomas J. Hund.
// Updated 07/2015
// Email thomas.hund@osumc.edu
//#################################################


#include "protocol.h"


//######################################################
// Default Constructor 
//######################################################
Protocol::Protocol()
{
    //##### Assign default parameters ##################
    
    doneflag = 1;       // set to 0 to end simulation
    
    tMax = 10000;   // max simulation time, ms
    
    readflag = 0;       // 1 to read SV ICs from file
    saveflag = 0;       // 1 to save final SV vals to file
    readfile = "./data/r.txt"; // File to read SV ICs
    savefile = "./data/s.txt"; // File to save final SV

    myId = 1;
    trial = 0;
   
    writestd = 0; 
    writeflag = 0;      // 1 to write data to file during sim
    dvarfile = "dvars.txt";  // File with SV to write.
    writetime = 0;      // time to start writing.
    writeint = 50;     // interval for writing.
    maxdoutsize = 10;   // Max number of dout files.
    
    pvarfile = "pvars.txt"; // File to specify cell params
    simvarfile = "simvars.txt";  // File to specify sim params

    propertyoutfile = "./data/dt%d_%s.dat";
    dvarsoutfile = "./data/dt%d_dvars.dat";
    finalpropertyoutfile = "./data/dss%d_%s.dat";
    finaldvarsoutfile = "./data/dss%d_pvars.dat";

    measflag = 0;       // 1 to track SV props during sim
    measfile = "mvars.txt"; // File containing property names to track
    meastime = 0;       // time to start tracking props
    maxmeassize = 10;   // Max number of SVs to track
    
    numtrials = 1;
    //######## Params for pacing protocol #########################
    paceflag = 0;   // 1 to pace cell.
    numstims = 1;   // # of stimuli to apply
    bcl = 1000;     // basic cycle length, ms
    stimval = 0.0;  // stim current amplitude, uA/uF
    stimdur = 1.0;  // stim duration, ms
    stimt = 0.0;    // time of first stim, ms
    
    stimcounter = 0;
    stimflag = 0;
    
    //##### Initialize variables ##################
    time=0.0;
    vM =  -88.0;
    //type = "Cell";  // class name
    
    // make map of params
    pars["tMax"]=&tMax;
    pars["bcl"]=&bcl;
    pars["stimval"]=&stimval;
    pars["stimdur"]=&stimdur;
    pars["stimt"]=&stimt;
    pars["numstims"]=&numstims;
    pars["numtrials"]=&numtrials;
    pars["readflag"]=&readflag;
    pars["saveflag"]=&saveflag;
    pars["writeflag"]=&writeflag;
    pars["writetime"]=&writetime;
    pars["measflag"]=&measflag;
    pars["meastime"]=&meastime;
    pars["paceflag"]=&paceflag;
    pars["maxdoutsize"]=&maxdoutsize;

};

//######################################################
// Deep Copy Constructor 
//######################################################
Protocol::Protocol(const Protocol& toCopy)
{
    unsigned int i = 0;
    std::map<string, double*>::iterator it;

    //##### Assign default parameters ##################
    
    doneflag = toCopy.doneflag;       // set to 0 to end simulation
    
    tMax = toCopy.tMax;   // max simulation time, ms
    
    readflag = toCopy.readflag;       // 1 to read SV ICs from file
    saveflag = toCopy.saveflag;       // 1 to save final SV vals to file
    readfile = toCopy.readfile; // File to read SV ICs
    savefile = toCopy.savefile; // File to save final SV

    myId = toCopy.myId;
    trial = toCopy.trial;   
    writestd = toCopy.writestd; 
    writeflag = toCopy.writeflag;      // 1 to write data to file during sim
    dvarfile = toCopy.dvarfile;  // File with SV to write.
    writetime = toCopy.writetime;      // time to start writing.
    writeint = toCopy.writeint;     // interval for writing.
    maxdoutsize = toCopy.maxdoutsize;   // Max number of dout files.
    
    pvarfile = toCopy.pvarfile; // File to specify cell params
    simvarfile = toCopy.simvarfile;  // File to specify sim params

    propertyoutfile = toCopy.propertyoutfile;
    dvarsoutfile = toCopy.dvarsoutfile;
    finalpropertyoutfile = toCopy.finalpropertyoutfile;
    finaldvarsoutfile = toCopy.finaldvarsoutfile;

    measflag = toCopy.measflag;       // 1 to track SV props during sim
    measfile = toCopy.measfile; // File containing property names to track
    meastime = toCopy.meastime;       // time to start tracking props
    maxmeassize = toCopy.maxmeassize;   // Max number of SVs to track
    
    numtrials = toCopy.numtrials;
    //######## Params for pacing protocol #########################
    paceflag = toCopy.paceflag;   // 1 to pace cell.
    numstims = toCopy.numstims;   // # of stimuli to apply
    bcl = toCopy.bcl;     // basic cycle length, ms
    stimval = toCopy.stimval;  // stim current amplitude, uA/uF
    stimdur = toCopy.stimdur;  // stim duration, ms
    stimt = toCopy.stimt;    // time of first stim, ms
    
    stimcounter = toCopy.stimcounter;
    stimflag = toCopy.stimflag;
    
    //##### Initialize variables ##################
    time= toCopy.time;
    vM = toCopy.vM;

    // make map of params
    pars["tMax"]=&tMax;
    pars["bcl"]=&bcl;
    pars["stimval"]=&stimval;
    pars["stimdur"]=&stimdur;
    pars["stimt"]=&stimt;
    pars["numstims"]=&numstims;
    pars["numtrials"]=&numtrials;
    pars["readflag"]=&readflag;
    pars["saveflag"]=&saveflag;
    pars["writeflag"]=&writeflag;
    pars["writetime"]=&writetime;
    pars["measflag"]=&measflag;
    pars["meastime"]=&meastime;
    pars["paceflag"]=&paceflag;
    pars["maxdoutsize"]=&maxdoutsize;

   
    pnames = vector<string>(toCopy.pnames);
    for(i = 0; i < toCopy.pvals.size(); i++) {
        pvals.push_back(vector<string>(toCopy.pvals.at(i)));
    }
    mvnames = vector<string>(toCopy.mvnames);
    for(i = 0; i < toCopy.mpnames.size(); i++) {
        mpnames.push_back(vector<string>(toCopy.mpnames.at(i)));
    }
    //###### Duplicate cells, measures outputs and maps######
    if(toCopy.cell != NULL) {
        cell = toCopy.cell->clone();
    }
    if(toCopy.douts != NULL) {
        int size = int(toCopy.mvnames.size()+1+toCopy.numtrials*(toCopy.mvnames.size()+1));
        douts = new Output[size];
    }
    if(toCopy.ics != NULL) {
            ics = new Output(*toCopy.ics);
    }
    if(toCopy.measures != NULL) {
        measures = new Measure[int(maxmeassize)];
        for(i = 0; i < (int)maxmeassize; i++) {
            measures[i] = *(new Measure(toCopy.measures[i]));
        }
    }

    parmap = map<string, double*>(toCopy.parmap);
    for(it = parmap.begin(); it != parmap.end(); it++) {
        parmap[it->first] = cell->pars[it->first];
    }
    datamap = map<string, double*>(toCopy.datamap);
    for(it = datamap.begin(); it != datamap.end(); it++) {
        datamap[it->first] =  cell->vars[it->first];
    }
};
//######################################################
// Destructor for parent cell class.
//#####################################################
Protocol::~Protocol(){};

// External stimulus.
int Protocol::stim()
{
  if(cell->t>=stimt&&cell->t<(stimt+stimdur)){
      if(stimflag==0){
          stimcounter++;
          stimflag=1;
          if(stimcounter>=int(numstims)){
              doneflag = 0;
              return 0;
          }
      }
      cell->externalStim(stimval);
    }
    else if(stimflag==1){     //trailing edge of stimulus
        stimt=stimt+bcl;
        stimflag=0;
        cell->apTime = 0.0;
    }

    cell->apTime = cell->apTime+cell->dt;

    doneflag = 1;
    return 1;
};

//############################################################
// Assign parameter values based on pnames and 2D vector pvals
//############################################################

int Protocol::assign_cell_pars(vector<string> pnames, vector< vector<string> > pvals, int trialnum)
{
    unsigned int j;
    double logmean = 0.0;
    double logstdev = 0.2;
    double mean = 1.0;
    double stdev = 0.2;
    double increment;
    
    lognormal_distribution<double> logdistribution(logmean,logstdev);
    normal_distribution<double> distribution(mean,stdev);
    
    for (j=0; j<pnames.size(); j++) {
        if (pvals[j][0] == "random" ) {  // if first arg is "random" = choose rand val
            if(pvals[j].size()>1){
                if (pvals[j][1] == "lognormal"){
                    if (pvals[j].size()>3) {
                        logmean = atof(pvals[j][2].c_str());
                        logstdev = atof(pvals[j][3].c_str());
                        lognormal_distribution<double> logdistribution(logmean,logstdev);
                        *cell->pars[pnames[j]] = logdistribution(generator) ;
                    }
                    else
                        *cell->pars[pnames[j]] = logdistribution(generator) ;
                }
                else
                    *cell->pars[pnames[j]] = distribution(generator) ;
            }
            else
                *cell->pars[pnames[j]] = distribution(generator) ;
        }
        
        else if (pvals[j][0] == "iter"){  // if first arg is "iter" = iterate
            if (pvals[j].size()>2) {     // if 3 args, 2nd arg is initial val, 3rd arg is inc
                if (trialnum>0){
                    increment = atof(pvals[j][2].c_str());
                    *cell->pars[pnames[j]] = *cell->pars[pnames[j]] - increment;
                }
                else
                    *cell->pars[pnames[j]] = atof(pvals[j][1].c_str());
            }
            else if (pvals[j].size()>1){  // if only two args, 2nd arg is inc
                if (trialnum>0){
                    increment = atof(pvals[j][1].c_str());
                    *cell->pars[pnames[j]] = *cell->pars[pnames[j]] - increment;
                }
            }
            else {
                if (trialnum>0){     // if only one arg, set incr to vary param over range
                    increment = *cell->pars[pnames[j]]/(numtrials-trialnum);
                    *cell->pars[pnames[j]] = *cell->pars[pnames[j]] - increment;
                }
            }
        }
        
        else                  // if first arg is just a number
            *cell->pars[pnames[j]]=atof(pvals[j][0].c_str());
    }

    return 1;
};



//#############################################################
// Resize varmap based on list in file.
//#############################################################
map<string, double*> Protocol::resizemap(map<string,double*> varmap, string file)
{
    ifstream ifile;
    map<string, double*> vars;
    string name;
    
    ifile.open(file);
    if(!ifile.is_open()){
        cout << "Error opening " << file << endl;
        exit(1);
    }
    
    while(!ifile.eof()){
        ifile >> name;
        if(varmap.count(name)>0)
            vars[name] = varmap[name];
    }
    ifile.close();
    return vars;
};

// Resize varmap based on list.
map<string, double*> Protocol::resizemap(map<string,double*> varmap, vector<string> names)
{
    unsigned int i;
    map<string, double*> vars;
    
    for (i=0; i<names.size(); i++){
        if(varmap.count(names[i])>0)
            vars[names[i]] = varmap[names[i]];
    }
    
    return vars;
};

//#############################################################
// Create vectors based on 2D list in file.  First row element is checked in varmap
// and gets stored in vector, rest of row checked in varmap2 and used to create row in 2D vector.
// Returns vector with first column and rest of file in 2D vector.
//#############################################################
tuple< vector<string>, vector< vector<string> > > Protocol::parse2Dmap(map<string,double*> varmap,map<string,double*> varmap2, string file)
{
    ifstream ifile;
    string line, name;
    
    vector<string> vnames;
    vector<string> mnames;
    
    vector< vector<string> > twoDmnames;
    
    ifile.open(file);
    if(!ifile.is_open()){
        cout << "Error opening " << file << endl;
        exit(1);
    }
    
    int counter = 0;
    
    while(!ifile.eof()){
        getline(ifile,line);
        stringstream linestream(line);
        linestream >> name;
        
        if(varmap.count(name)>0){
            vnames.push_back(name);
            
            while (!linestream.eof()) {
                linestream >> name;
                if(varmap2.count(name)>0){
                    mnames.push_back(name);
                }
            }
            if (mnames.size()>0)
                twoDmnames.push_back(mnames);
            else
                vnames.pop_back();
            
            mnames.clear();
        }
        counter++;
        
    }
    ifile.close();
    return make_tuple(vnames,twoDmnames);
};

//#############################################################
// Create vectors based on mixed list in file.  First row element is checked in varmap
// and gets stored in vector, rest of row used to create row in 2D vector.
// Returns vector with first column and rest of file in 2D vector.
//#############################################################
tuple< vector<string>, vector< vector<string> > > Protocol::parsemixedmap(map<string,double*> varmap, string file)
{
    ifstream ifile;
    string line, name;
    
    vector<string> cnames;
    vector<string> rnames;
    
    vector< vector<string> > twoDrnames;
    
    ifile.open(file);
    if(!ifile.is_open()){
        cout << "Error opening " << file << endl;
        exit(1);
    }
    
    int counter = 0;
    
    while(!ifile.eof()){
        getline(ifile,line);
        stringstream linestream(line);
        linestream >> name;
        
        if(varmap.count(name)>0){
            cnames.push_back(name);
            
            while (!linestream.eof()) {
                linestream >> name;
                rnames.push_back(name);
            }
            if (rnames.size()>0)
                twoDrnames.push_back(rnames);
            else
                cnames.pop_back();
            
            rnames.clear();
        }
        counter++;
        
    }
    ifile.close();
    return make_tuple(cnames,twoDrnames);
};

//#############################################################
// Returns copy of variable values (not pointers) in varmap.
//#############################################################
map<string,double> Protocol::copymapvals(map<string, double*> varmap)
{
    map<string, double> vars;
    map<string, double*>::iterator p;
    
    for(p=varmap.begin(); p!=varmap.end(); ++p)
        vars[p->first] = *p->second;
    
    return vars;
};

//#############################################################
// Write values of variables in varmap to screen.
//#############################################################
int Protocol::map2screen(map<string, double*> varmap)
{
    map<string, double*>::iterator p;
    
    for(p=varmap.begin(); p!=varmap.end(); ++p)
        cout << p->first << " = " << *p->second << "; ";
    
    cout << endl;
    
    return 1;
};

//#############################################################
// Write values of variables in varmap to screen.
//#############################################################
int Protocol::map2screen(map<string, double> varmap)
{
    map<string, double>::iterator p;
    
    for(p=varmap.begin(); p!=varmap.end(); ++p)
        cout << p->first << " = " << p->second << "; ";
    
    cout << endl;
    
    return 1;
};

//#############################################################
// Set params for measuring state var props (dur, min, max, etc).
//#############################################################
int Protocol::initializeMeasure(int measureSize) {
    unsigned int i;
    bool toReturn = true;

     measures = new Measure[measureSize];   // Array of measure streams.
    
    //Parse and resize 2D map based on measfile...move into measure?
    
    if (int(measflag)==1){
       for (i=0; i<mvnames.size(); i++) {
            if (i<int(maxmeassize)) {
                set<string> temp = measures[i].getSelection();
                temp.insert(mpnames[i].begin(), mpnames[i].end());
                toReturn = measures[i].setSelection(temp);
                measures[i].varname = mvnames[i];
            }
        }
    }
    return (int)toReturn;

};

//###############################################################
// Read file into mvnames and mpnames
// ##############################################################
bool Protocol::readMvars(string file){
     set<string> temp = Measure().getVariables();
     bool ret = !(bool)parse2Dmap(cell->vars,temp, measfile, &mvnames, &mpnames);
    return ret;
};

//############################################################
// Get the number of Doutput arrays to create
//############################################################
int Protocol::getNeededDOutputSize(){
    return int(mvnames.size()+1+numtrials*(mvnames.size()+1));
};

//############################################################
// Run the cell simulation
//############################################################
int Protocol::runSim() {
    int return_val;

    //###############################################################
    // Loop over number of trials
    //###############################################################
    for(;trial<int(numtrials);setTrial(getTrial() +1))
    {
        return_val = (int)runTrial(); 
    }
    return return_val;
};

bool Protocol::runTrial() {
        unsigned int j =0;
        char writefile[50];     // Buffer for storing filenames

        time = cell->t = 0.0;      // reset time
        doneflag=1;     // reset doneflag
  
//        if (int(readflag)==1)
//            readvals(cell->vars, readfile);  // Init SVs before each trial.
        
        //###############################################################
        // Every time step, currents, concentrations, and Vm are calculated.
        //###############################################################
        int pCount = 0;

        for(j=0; j<mvnames.size(); j++) {
            sprintf(writefile,propertyoutfile.c_str(),trial,mvnames[j].c_str());
            measures[j].setOutputfile(writefile);
        }
        
        while(int(doneflag)&&(time<tMax)){
            
            time = cell->tstep(stimt);    // Update time
            cell->updateCurr();    // Update membrane currents
            if(int(paceflag)==1)  // Apply stimulus
                stim();

            cell->updateConc();   // Update ion concentrations
            vM=cell->updateV();     // Update transmembrane potential

            //##### Output select variables to file  ####################
            if(int(measflag)==1&&cell->t>meastime){
                for (j=0; j<mvnames.size(); j++) {
                    measures[j].measure(cell->t,*cell->vars[measures[j].varname]);
                    if(int(writeflag)==1) {
                        measures[j].write(true, !(int(doneflag)&&(time<tMax)));
                    }
                }
            }
            if (int(writeflag)==1&&time>writetime&&pCount%int(writeint)==0) {
                sprintf(writefile,dvarsoutfile.c_str(),trial);
                douts[mvnames.size()+(trial+1)*(mvnames.size()+1)].writevals(datamap,writefile,'a');
            }
            cell->setV(vM); //Overwrite vOld with new value
            pCount++;
        }
    
      if(writestd)   
        map2screen(parmap);  // send final property values to console
      // Output final (ss) property values for each trial
      for (j=0; j<mvnames.size(); j++){
          if(writestd)
            map2screen(measures[j].getVariablesMap());
          sprintf(writefile,finalpropertyoutfile.c_str(), trial, mvnames[j].c_str());
          measures[j].setOutputfile(writefile);
          measures[j].write(false, true);
          measures[j].reset();
      } 
      
      // Output parameter values for each trial
      sprintf(writefile, finaldvarsoutfile.c_str(), trial);
      douts[mvnames.size()].writevals(parmap, writefile, 'a');
 
}

//#############################################################
// Read values of variables in varmap from file.
// Format of file should be "variable name" tab "value"
//#############################################################

int Protocol::readpars(map<string, double*> varmap, string file)
{
    ifstream ifile;
    
    string name;
    double num;
    map<string, double*>::iterator p;
    
    if(!ifile.is_open())
        ifile.open(file);
    if(!ifile.is_open()){
        cout << "Error opening " << file << endl;
        return 1;
    }
    
    while(!ifile.eof()){
        ifile >> name;
        if(varmap.count(name)>0){
            ifile >> num;
            *varmap[name] = num;
        }
    }
    
    ifile.close();
    return 0;
  
};

//#############################################################
// Create vectors based on mixed list in file.  First row element is checked in varmap
// and gets stored in vector, rest of row used to create row in 2D vector.
// Writes paramiter names into cnames and paramiter values into twoDrnames.
//#############################################################
int Protocol::parsemixedmap(map<string,double*> varmap, string file, vector<string>* cnames, vector<vector<string>>* twoDrnames)
{
    ifstream ifile;
    string line, name;
    
    vector<string> rnames;
    
    ifile.open(file);
    if(!ifile.is_open()){
        cout << "Error opening " << file << endl;
        return 1;
    }
    
    int counter = 0;
    
    while(!ifile.eof()){
        getline(ifile,line);
        stringstream linestream(line);
        linestream >> name;
        if(varmap.count(name)>0){
            (*cnames).push_back(name);
            
            while (!linestream.eof()) {
                linestream >> name;
                rnames.push_back(name);
            }
            if (rnames.size()>0)
                (*twoDrnames).push_back(rnames);
            else
                (*cnames).pop_back();
            
            rnames.clear();
        }
        counter++;
        
    }
    ifile.close();
   
    return 0;
};

//#############################################################
// Create vectors based on 2D list in file.  First row element is checked in varmap
// and gets stored in vector, rest of row checked in varmap2 and used to create row in 2D vector.
// Returns vector with first column and rest of file in 2D vector.
//#############################################################
int Protocol::parse2Dmap(map<string,double*> varmap,set<string> vars2, string file, vector<string>* vnames, vector< vector<string> >* twoDmnames)
{
    ifstream ifile;
    string line, name;
    
    vector<string> mnames;
    
    ifile.open(file);
    if(!ifile.is_open()){
        cout << "Error opening " << file << endl;
        return 1;
    }
    
    int counter = 0;
    
    while(!ifile.eof()){
        getline(ifile,line);
        stringstream linestream(line);
        linestream >> name;
        
        if(varmap.count(name)>0){
            (*vnames).push_back(name);
            while (!linestream.eof()) {
                linestream >> name;
                if(vars2.count(name)>0){
                    mnames.push_back(name);
                }
            }
            if (mnames.size()>0)
                (*twoDmnames).push_back(mnames);
            else
               (* vnames).pop_back();
            
            mnames.clear();
        }
        counter++;
        
    }
    ifile.close();
    return 0;
};


//#############################################################
// Create vectors based on 2D list in file.  First row element is checked in varmap
// and gets stored in vector, rest of row checked in varmap2 and used to create row in 2D vector.
// Returns vector with first column and rest of file in 2D vector.
//#############################################################
int Protocol::parse2Dmap(map<string,double*> varmap,map<string,double*> varmap2, string file, vector<string>* vnames, vector< vector<string> >* twoDmnames)
{
    ifstream ifile;
    string line, name;
    
    vector<string> mnames;
    
    ifile.open(file);
    if(!ifile.is_open()){
        cout << "Error opening " << file << endl;
        return 1;
    }
    
    int counter = 0;
    
    while(!ifile.eof()){
        getline(ifile,line);
        stringstream linestream(line);
        linestream >> name;
        
        if(varmap.count(name)>0){
            (*vnames).push_back(name);
            while (!linestream.eof()) {
                linestream >> name;
                if(varmap2.count(name)>0){
                    mnames.push_back(name);
                }
            }
            if (mnames.size()>0)
                (*twoDmnames).push_back(mnames);
            else
               (* vnames).pop_back();
            
            mnames.clear();
        }
        counter++;
        
    }
    ifile.close();
    return 0;
};

//############################################################
// write 2Dmap to file
//############################################################
bool Protocol::write2Dmap(vector<string> vnames, vector< vector<string> > twoDmnames, string file) {
    int ret = 0;
    unsigned int i,j;
    ofstream out;

    out.open(file);
    if(!out.is_open()){
        cout << "Error opening " << file << endl;
        return 1;
    }

    for(i = 0; i < vnames.size(); i++) {
        out << vnames[i] << "\t";
        for(j = 0; j < twoDmnames[i].size(); j++){
            out << twoDmnames[i][j] << "\t";
        }
        out << endl;
    }

    return ret;
}

//#############################################################
// Resize varmap based on list in file.
//#############################################################
int Protocol::resizemap(map<string,double*> varmap, string file, map<string, double*>* vars)
{
    ifstream ifile;
    string name;
    
    ifile.open(file);
    if(!ifile.is_open()){
        cout << "Error opening " << file << endl;
        return 1;
    }
    
    while(!ifile.eof()){
        ifile >> name;
        if(varmap.count(name)>0)
            (*vars)[name] = varmap[name];
    }
    ifile.close();
    return 0;
};

//#############################################################
// Set params for reading/saving model params.
//#############################################################
int Protocol::readpvars(){

    int ret = 0;
    ret = parsemixedmap(cell->pars, pvarfile ,&pnames, &pvals);
    parmap = resizemap(cell->pars,pnames);
    return ret;
};

//############################################################
// Write the contents of varmap to a file
//############################################################
bool Protocol::writepars(map<string, double*> varmap, string file)
{
    ofstream ofile;
    string name;
    double num;
    map<string, double*>::iterator it;
    
    if(!ofile.is_open())
        ofile.open(file);
    if(!ofile.is_open()){
        cout << "Error opening " << file << endl;
        return 1;
    }
    
    for(it = varmap.begin(); it != varmap.end(); it++){
        ofile << it->first << "\t" << *(it->second) << endl;
    }
    
    ofile.close();
    return 0;
  
};
//############################################################
//Wirte the keys from varmap to a file
//############################################################
bool Protocol::writedvars(map<string, double*> varmap, string file)
{
    ofstream ofile;
    string name;
    double num;
    map<string, double*>::iterator it;
    
    if(!ofile.is_open())
        ofile.open(file);
    if(!ofile.is_open()){
        cout << "Error opening " << file << endl;
        return 1;
    }

    for(it = varmap.begin(); it != varmap.end(); it++){
        ofile << it->first << endl;
    }

    ofile.close();
    return 0;

}

void Protocol::setTrial(unsigned int current_trial) {
    trial = current_trial;
    assign_cell_pars(pnames,pvals,trial);   // Assign cell pars
}

unsigned int Protocol::getTrial() {
    return trial;
}


