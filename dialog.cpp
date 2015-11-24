#include "dialog.h"
#include "ui_dialog.h"
#include "myoutput.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::doTask(Protocol &proto)
{
    // work goes here for thread
    QPointer<MyOutput> finalSVvals = new MyOutput;
    char filename[200];   // file for saving final state var vals

    qDebug() << "Starting thread...V = " << proto.cell->vOld;
 //   proto.assign_cell_pars(pnames,pvals,i);   // Assign cell pars before each trial.
    proto.time = proto.cell->t = 0.0;      // reset time
    proto.writeflag = 1;
    proto.writetime = 9000;
    proto.writeint = 500;
    proto.doneflag=1;     // reset doneflag

    //QPointer<Output> dOut = new Output;

    //###############################################################
    // Every time step, currents, concentrations, and Vm are calculated.
    //###############################################################
    int pCount = 0;

    while(int(proto.doneflag)&&(proto.time<proto.tMax)){

        proto.time = proto.cell->tstep(proto.stimt);    // Update time
        proto.cell->updateCurr();    // Update membrane currents

        if(int(proto.paceflag)==1)  // Apply stimulus
            proto.stim();

        proto.cell->updateConc();   // Update ion concentrations
        proto.vM=proto.cell->updateV();     // Update transmembrane potential

        if (int(proto.writeflag)==1&&proto.time>proto.writetime&&pCount%int(proto.writeint)==0) {
            qDebug() << "V = " << proto.cell->vOld << ", at t = " << proto.time;

        }
        proto.cell->setV(proto.vM); //Overwrite vOld with new value
        pCount++;
    }

    if (int(proto.saveflag)==1)
    {
        std::sprintf(filename,"/Users/thomashund/Documents/projects/QtDocs/MyConcurrentModel/data/s%d.dat",proto.myId);   // introduce my id?
        try{
            if (finalSVvals->writevals(proto.cell->vars,filename)==0)
                throw QString("Error saving final state variable values");
        }
        catch(QString sErr)
        {
            qDebug() << sErr;
        }
    }

    qDebug() << "Stopped task";
}

void Dialog::on_pushButton_clicked()
{
    QVector<Protocol> vector;
    std::vector<Protocol> protos(ui->spinBox->value()); // create vector of Protocols for QtConcurrent
    char filename[200];   // file for saving final state var vals

    QPointer<MyOutput> initialSVvals = new MyOutput;

    for(int i = 0; i < ui->spinBox->value(); i++)
    {
       protos[i].cell = new ControlSa;          // Declare specific cell type for each protocol
       protos[i].myId = i+1;

       protos[i].saveflag = 1;                  // 1 = save final state var vals
       protos[i].readflag = 1;                  // 1 = save final state var vals

       if (int(protos[i].readflag)==1){
           try
           {
                std::sprintf(filename,"/Users/thomashund/Documents/projects/QtDocs/MyConcurrentModel/data/r%d.dat",protos[i].myId);   // introduce my id?
                if (initialSVvals->readvals(protos[i].cell->vars,filename)==0)
                    throw QString("Error reading initial state variable values...simulation terminated");
                vector.append(protos[i]);                // Initialize vector for QtConcurrent...move into try statement?
            }
            catch(QString sErr)
            {
                qDebug() << sErr;
            }
       }
       else
           vector.append(protos[i]);                // Initialize vector for QtConcurrent...move into try statement?
    }

    QProgressDialog pdialog;
    pdialog.setLabelText("Processing");

    QFutureWatcher<void> watcher;

    connect(&pdialog,SIGNAL(canceled()),&watcher,SLOT(cancel()));  // connect signals and slots for watcher and dialog
    connect(&watcher,SIGNAL(finished()),&pdialog,SLOT(reset()));
    connect(&watcher,SIGNAL(progressRangeChanged(int,int)),&pdialog,SLOT(setRange(int,int)));
    connect(&watcher,SIGNAL(progressValueChanged(int)),&pdialog,SLOT(setValue(int)));

    //QFuture = QtConcurrent::map(vector,&Dialog::doTask);
    watcher.setFuture(QtConcurrent::map(vector,&Dialog::doTask));  // pass vector of protocols to QtConcurrent

    pdialog.exec();
    watcher.waitForFinished();       // watch for QtConcurrent to finish

    if(watcher.isCanceled())
    {
       qDebug()<<"canceled!";
       QMessageBox::critical(this,"Cancel","Simulation canceled!");
    }
    else
    {
       qDebug()<<"finished!";
       QMessageBox::information(this,"Finish","Simulation finished!");
    }

    for(int i = 0; i < ui->spinBox->value(); i++)
    {
        delete protos[i].cell;
    }

}
