#include "myoutput.h"
MyOutput::MyOutput(QObject *parent) : QObject(parent)
{
    counter = 0;
    interval = 1;

}
//#############################################################
// Read values of variables in varmap from file.
// Format of file should be "variable name" tab "value"
//#############################################################
int MyOutput::readvals(std::map<QString, double*> varmap, QString filename)
{
   QString name;
    double num;

    std::map<QString, double*>::iterator p;

    if(!ifile.isOpen()){
        ifile.setFileName(filename);
        if(!ifile.open(QFile::ReadOnly | QFile::Text)){
            qDebug() << "Error opening " << filename;
            return 0;
        }
    }
QTextStream in(&ifile);
    while(!in.atEnd()){
        in >> name;
        if(varmap.count(name)>0){
            in >> num;
            *varmap[name] = num;
        }
    }

    ifile.close();
    return 1;

}
int MyOutput::writevals(std::map<QString, double *> varmap, QString filename, char type)
{
    std::map<QString, double*>::iterator p;

    if(counter==0){
         if(!ofile.isOpen()){
                ofile.setFileName(filename);
                if(!ofile.open(QFile::WriteOnly | QFile::Text)){
                    qDebug() << "Error opening " << filename;
                    return 0;
                }
        }
        QTextStream out(&ofile);
        for(p=varmap.begin(); p!=varmap.end(); ++p)
             out << p->first << '\t';
        out << endl;
    }
    QTextStream out(&ofile);
    for(p=varmap.begin(); p!=varmap.end(); ++p)
        out << *p->second << '\t';

    out << endl;

    counter++;
    return 1;

}
int MyOutput::writevals(std::map<QString, double *> varmap, QString filename)
{
    std::map<QString, double*>::iterator p;
    QTextStream out(&ofile);

    if(!ofile.isOpen()){
       ofile.setFileName(filename);
       if(!ofile.open(QFile::WriteOnly | QFile::Text)){
           qDebug() << "Error opening " << filename;
           return 0;
       }
    }

    for(p=varmap.begin(); p!=varmap.end(); ++p)
        out << p->first << '\t' << *p->second << endl;

    ofile.close();
    return 1;
}
int MyOutput::writevals(std::map<QString, double> varmap, QString filename, char type)
{
    std::map<QString, double>::iterator p;
    QTextStream out(&ofile);

    if(counter==0){
        if(!ofile.isOpen()){
            ofile.setFileName(filename);
            if(!ofile.open(QFile::WriteOnly | QFile::Text)){
                qDebug() << "Error opening " << filename;
                return 0;
            }
        }

        for(p=varmap.begin(); p!=varmap.end(); ++p)
            out << p->first << '\t';
        out << endl;
    }

    for(p=varmap.begin(); p!=varmap.end(); ++p)
        out << p->second << '\t';

    out << endl;

    counter++;
    return 1;
}
int MyOutput::writevals(std::map<QString, double> varmap, QString filename)
{
    std::map<QString, double>::iterator p;
    QTextStream out(&ofile);

    if(!ofile.isOpen()){
       ofile.setFileName(filename);
       if(!ofile.open(QFile::WriteOnly | QFile::Text)){
           qDebug() << "Error opening " << filename;
           return 0;
       }
    }

    for(p=varmap.begin(); p!=varmap.end(); ++p)
        out << p->first << '\t' << p->second << endl;

    ofile.close();
    return 1;
}
