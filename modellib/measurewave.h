//################################################
// This header file
// declares Measure class used to track properties
// (e.g. peak, min, duration) of specified state variable.
//
// Copyright (C) 2015 Thomas J. Hund.
// Updated 07/2015
// Email thomas.hund@osumc.edu
//#################################################

#ifndef MEASUREWAVE_H
#define MEASUREWAVE_H

#include <string>
#include <map>
#include <set>
#include <limits>
#include "measure.h"

using namespace std;

class MeasureWave: public Measure
{
    public:
        MeasureWave(std::set<std::string> selected = {}, double percrepol = 50);
        MeasureWave(const MeasureWave& toCopy);
        MeasureWave(MeasureWave&& toCopy);
        virtual ~MeasureWave() = default;

        MeasureWave& operator=(const MeasureWave& toCopy) = delete;

        void reset();   //resets params to init vals

        //		string varname;

        set<string> variables();
        map<string,double> variablesMap();

        void percrepol(double val);
        double percrepol() const;

    protected:

        virtual void calcMeasure(double time,double var);  //measures props related to var; returns 1 when ready for output.
        double vartakeoff = NAN; //var value at point of max deflection.
        double durtime1 = NAN;
        double amp = NAN;
        double ddr = NAN;      //diastolic depol. rate
        //		double maxderiv1;
        //		double maxderiv2;
        double derivt1 = NAN;  // time of prev. cycle max deriv.
        double deriv2ndt = NAN;  // time of max 2nd deriv.
        double maxderiv2nd = 0;
        double cl = 0;
        double dur = NAN;   //duration
        double __percrepol = 50;   //specify percent repolarization
        double repol = -25;           // repol var val for duration measure.
        bool minflag = false;
        bool maxflag = false;
        bool durflag = false;    //1 while measuring duration.
        bool ampflag = false;
        bool ddrflag = false;

    private:
        void copy(const MeasureWave& toCopy);
};

#endif
