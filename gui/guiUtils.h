#ifndef GUIUTILS_H
#define GUIUTILS_H

class GuiUtils {
  public:
    QMap<QString, QString> parsDescriptions;
    QMap<QString, QString> dvarsDescriptions;
    QMap<QString, QString> measDescriptions;
    GuiUtils() {
        parsDescriptions.insert("bcl","");
        parsDescriptions.insert("meastime", "");
        parsDescriptions.insert("paceflag","");
        parsDescriptions.insert("stimdur","");
        parsDescriptions.insert("tMax","");
        parsDescriptions.insert("maxdoutsize","");
        parsDescriptions.insert("numstims","");
        parsDescriptions.insert("readflag","");
        parsDescriptions.insert("stimt","");
        parsDescriptions.insert("writeflag", "1=write, 0=don't write");
        parsDescriptions.insert("measflag", "");
        parsDescriptions.insert("numtrials","");
        parsDescriptions.insert("saveflag","");
        parsDescriptions.insert("stimval","");
        parsDescriptions.insert("writetime", "time during simulation that writing will begin");

        dvarsDescriptions.insert("Gate.d","");
        dvarsDescriptions.insert("Gate.paf","");
        dvarsDescriptions.insert("Gate.r","");
        dvarsDescriptions.insert("cmdnI","");
        dvarsDescriptions.insert("iCart","");
        dvarsDescriptions.insert("iHna","");
        dvarsDescriptions.insert("iTo","Indium Tin Oxide");
        dvarsDescriptions.insert("naI", "Sodium Iodide");
        dvarsDescriptions.insert("Gate.dt","");
        dvarsDescriptions.insert("Gate.pas","");
        dvarsDescriptions.insert("Gate.y","");
        dvarsDescriptions.insert("cmdnR","");
        dvarsDescriptions.insert("iCat","");
        dvarsDescriptions.insert("iKach","");
        dvarsDescriptions.insert("iNak","");
        dvarsDescriptions.insert("iTot", "");
        dvarsDescriptions.insert("t", "Time");
        dvarsDescriptions.insert("Gate.f","");
        dvarsDescriptions.insert("Gate.pi","");
        dvarsDescriptions.insert("caI","");
        dvarsDescriptions.insert("csqn","");
        dvarsDescriptions.insert("iCatt","");
        dvarsDescriptions.insert("iKr","");
        dvarsDescriptions.insert("iNat","");
        dvarsDescriptions.insert("iTr","");
        dvarsDescriptions.insert("trpnCa","");
        dvarsDescriptions.insert("Gate.fca","");
        dvarsDescriptions.insert("Gate.q","");
        dvarsDescriptions.insert("caJsr","");
        dvarsDescriptions.insert("dVdt","");
        dvarsDescriptions.insert("iDiff","");
        dvarsDescriptions.insert("iKs","");
        dvarsDescriptions.insert("iRel","");
        dvarsDescriptions.insert("iTrek","");
        dvarsDescriptions.insert("trpnMg","");
        dvarsDescriptions.insert("Gate.ft","");
        dvarsDescriptions.insert("Gate.qa","");
        dvarsDescriptions.insert("caNsr","");
        dvarsDescriptions.insert("iCait","");
        dvarsDescriptions.insert("iH","");
        dvarsDescriptions.insert("iKt","");
        dvarsDescriptions.insert("iSt","");
        dvarsDescriptions.insert("iUp","");
        dvarsDescriptions.insert("trpnMgmg","");
        dvarsDescriptions.insert("Gate.n","");
        dvarsDescriptions.insert("Gate.qi","");
        dvarsDescriptions.insert("caR","");
        dvarsDescriptions.insert("iCal","");
        dvarsDescriptions.insert("iHk","");
        dvarsDescriptions.insert("iNah","");
        dvarsDescriptions.insert("iSus","");
        dvarsDescriptions.insert("kI","");
        dvarsDescriptions.insert("vOld","");

        measDescriptions.insert("amp","");
        measDescriptions.insert("cl","");
        measDescriptions.insert("ddr","");
        measDescriptions.insert("deriv2ndt","");
        measDescriptions.insert("derivt","");
        measDescriptions.insert("dur","");
        measDescriptions.insert("durtime1","");
        measDescriptions.insert("maxduriv","");
 
    } 
};
#endif
