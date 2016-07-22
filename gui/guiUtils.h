#ifndef GUIUTILS_H
#define GUIUTILS_H

class GuiUtils {
  public:
    QMap<QString, QString> parsDescriptions;
    QMap<QString, QString> dvarsDescriptions;
    QMap<QString, QString> measDescriptions;
    GuiUtils() {
        parsDescriptions.insert("bcl","Basic cycle length");
        parsDescriptions.insert("meastime", "Timepoint to begin measuring variables checked in the “Measure Variables” panel");
        parsDescriptions.insert("paceflag","Check if applying stimulus current to cell membrane");
        parsDescriptions.insert("stimdur","Duration of input stimulus current to cell membrane");
        parsDescriptions.insert("tMax","Duration of simulation");
        parsDescriptions.insert("numstims","Total number of input stimulus pulses to apply to the cell membrane");
        parsDescriptions.insert("stimt","Timepoint to begin inputting stimulus current to cell membrane");
        parsDescriptions.insert("writeflag", "1=write, 0=don't write");
        parsDescriptions.insert("numtrials","Total number of trial simulations to run");
        parsDescriptions.insert("stimval","Current value of input stimulus current to cell membrane");
        parsDescriptions.insert("writetime", "Timepoint to begin writing value of variables checked in the “Track Variables” panel");
        parsDescriptions.insert("readCellState", "Check if reading in initial conditions from a text file");
        parsDescriptions.insert("writeCellState", "Check if writing final conditions of the simulation to a text file");
        parsDescriptions.insert("dvarfile","Name of output text file denoting initial conditions/ values checked in “Track Cell Values”");
        parsDescriptions.insert("measfile","Name of output text file denoting initial conditions/ values checked in “Measure Cell Values”");
        parsDescriptions.insert("simvarfile","Name of output text file denoting initial conditions/ values checked in “Choose Variables”");
        parsDescriptions.insert("pvarfile","Name of output text file denoting initial conditions/ values checked in “Perform Sensitivity Analysis”");
        parsDescriptions.insert("cellStateDir","Folder containing output text files with data and initial conditions");
        parsDescriptions.insert("datadir","Folder that will contain output of simulation");



        dvarsDescriptions.insert("caI","Concentration of free and buffered intracellular Ca2+, mmol/L");
        dvarsDescriptions.insert("caJsr","Ca2+ concentration in junctional sarcoplasmic reticulum, mmol/L");
        dvarsDescriptions.insert("caNsr","Ca2+ concentration in network sarcoplasmic reticulum, mmol/L");
        dvarsDescriptions.insert("caR","");
        dvarsDescriptions.insert("Ca","Ca2+ current through the L-type Ca2+ channel, uA/uF");
        dvarsDescriptions.insert("iCab","Background Ca2+ current, uA/uF");
        dvarsDescriptions.insert("iCat","Ca2+ current through the T-type Ca2+ channel, uA/uF");
        dvarsDescriptions.insert("iClt","Total transmembrane Cl- current");
        dvarsDescriptions.insert("iDiff","Ca2+ transfer from SS to myoplasm, mmol/L per ms");
        dvarsDescriptions.insert("iK1","Time-independent K+ current, uA/uF");
        dvarsDescriptions.insert("iKp","Plateau K+ current, uA/uF");
        dvarsDescriptions.insert("iKr","Rapid delayed rectifier K+ current, uA/uF");
        dvarsDescriptions.insert("iKs","Slow delayed rectifier K+ current, uA/uF");
        dvarsDescriptions.insert("iKt","Total transmembrane K+ current");
        dvarsDescriptions.insert("iLeak","Ca2+ leak from JSR to myoplasm, mmol/L per ms");
        dvarsDescriptions.insert("iNa","Fast Na+ current, uA/uF");
        dvarsDescriptions.insert("iNaca","Na+-Ca2+ exchanger, uA/uF");
        dvarsDescriptions.insert("iNacar","");
        dvarsDescriptions.insert("iNak","Na+-K+ pump, uA/uF");
        dvarsDescriptions.insert("iNal","Slowly inactivating late Na+ current, uA/uF");
        dvarsDescriptions.insert("iNat","Total transmembrane Na+ current");
        dvarsDescriptions.insert("iPca","Sarcolemmal Ca2+ pump, uA/uF");
        dvarsDescriptions.insert("iRel","Ca2+ release from JSR to myoplasm, mmol/L per ms");
        dvarsDescriptions.insert("iTo","4AP-sensitive transient outward K+ current, uA/uF");
        dvarsDescriptions.insert("iTo2","Ca2+-dependent transient outward Cl- current, uA/uF");
        dvarsDescriptions.insert("iTot","Total transmembrane current");
        dvarsDescriptions.insert("iTr","Ca2+ transfer from network to junctional sarcoplasmic reticulum, mmol/L per ms");
        dvarsDescriptions.insert("iUp","Ca2+ uptake from myoplasm to NSR, mmol/L per ms");
		dvarsDescriptions.insert("Gate.a","Activation gate of Ito1");
		dvarsDescriptions.insert("Gate.aa","Ca2+-dependent inactivation gate of Ito2");
		dvarsDescriptions.insert("Gate.d","Activation gate of ICaL");
		dvarsDescriptions.insert("Gate.f","Fast voltage-dependent inactivation gate of ICaL");
		dvarsDescriptions.insert("Gate.fca","Fast Ca2+-dependent inactivation gate of ICaL");
		dvarsDescriptions.insert("Gate.fca2","Slow Ca2+-dependent inactivation gate of ICaL");
		dvarsDescriptions.insert("Gate.h","Fast inactivation gate of INa");
		dvarsDescriptions.insert("Gate.hl","Inactivation gate of INaL");
		dvarsDescriptions.insert("Gate.i","Fast inactivation gate of Ito1");
		dvarsDescriptions.insert("Gate.i2","Slow inactivation gate of Ito1");
		dvarsDescriptions.insert("Gate.j","Slow inactivation gate of INa");
		dvarsDescriptions.insert("Gate.m","Activation gate of INa");
		dvarsDescriptions.insert("Gate.ml","Activation gate of INaL");
		dvarsDescriptions.insert("Gate.xr","Activation gate of IKr");
		dvarsDescriptions.insert("Gate.xs","Fast activation gate of IKs");
		dvarsDescriptions.insert("Gate.xs2","Slow activation gate of IKs");
		dvarsDescriptions.insert("clI","");
		dvarsDescriptions.insert("cmdn","Calmodulin, Ca2+ buffer in myoplasm");
		dvarsDescriptions.insert("csqn","Calsequestrin, Ca2+ buffer in JSR");
		dvarsDescriptions.insert("dVdt","Change in voltage with respect fo time");
		dvarsDescriptions.insert("fBlock","");
		dvarsDescriptions.insert("fBound","");
		dvarsDescriptions.insert("fI","");
		dvarsDescriptions.insert("fOx","");
		dvarsDescriptions.insert("fOxP","");
		dvarsDescriptions.insert("fPhos","");
		dvarsDescriptions.insert("kI","Intracellular potassium concentration");
		dvarsDescriptions.insert("naI","Intracellular sodium concentration");
		dvarsDescriptions.insert("ryRopen","");
		dvarsDescriptions.insert("t","Time (ms)");
		dvarsDescriptions.insert("trpn","troponin");
		dvarsDescriptions.insert("vOld","Voltage (mV)");

		measDescriptions.insert("amp","Amplitude");
		measDescriptions.insert("cl","Cycle length");
		measDescriptions.insert("ddr","Diastolic depolarization rate");
		measDescriptions.insert("deriv2ndt","Second derivative with respect to time");
		measDescriptions.insert("derivt","Derivative with respect to time");
		measDescriptions.insert("dur","Duration");
		measDescriptions.insert("durtime1","Time at which duration is measured");
		measDescriptions.insert("maxderiv","Max derivative");
		measDescriptions.insert("min","Minimum");
		measDescriptions.insert("mint","Time at which minimum is measured");
		measDescriptions.insert("peak","Peak");
		measDescriptions.insert("vartakeoff","");
    } 
};
#endif
