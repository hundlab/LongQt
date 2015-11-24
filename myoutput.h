#ifndef MYOUTPUT_H
#define MYOUTPUT_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

class MyOutput : public QObject
{
    Q_OBJECT
public:
    explicit MyOutput(QObject *parent = 0);

    int counter;
    int interval;   //write to file when counter%interval==0

    virtual int writevals(std::map<QString, double*> varmap, QString filename, char type);
    virtual int writevals(std::map<QString, double*> varmap, QString filename);
    virtual int writevals(std::map<QString, double> varmap, QString filename, char type);
    virtual int writevals(std::map<QString, double> varmap, QString filename);
    virtual int readvals(std::map<QString, double*> varmap, QString file);

private:
    QFile ofile;
    QFile ifile;

signals:

public slots:
};

#endif // MYOUTPUT_H
