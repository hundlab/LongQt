#include "proto.h"
#include "dialog.h"
#include "ui_dialog.h"
#include "myoutput.h"

void setupSim(Protocol* proto, int simNum);

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
<<<<<<< HEAD
    ui->comboBox->addItem("Default Cell");
    ui->comboBox_2->addItem("Default Species");
=======
    ui->comboBox->addItem("Choose Cell");
    ui->comboBox->addItem("Ventricular");
    ui->comboBox->addItem("Sinoatrial Node");
    ui->comboBox->addItem("Atrial");
    ui->comboBox->addItem("Kurata");
    ui->comboBox_2->addItem("Choose Species");
    ui->comboBox_2->addItem("Mouse");
    ui->comboBox_2->addItem("Rabbit");
    ui->comboBox_2->addItem("Human");
>>>>>>> danielle-daniel-merge
}

Dialog::~Dialog()
{
    delete ui;
}

<<<<<<< HEAD
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
        std::sprintf(filename,"/Applications/Qt/MyConcurrentModel/data/s%d.dat", proto.myId);
       // std::sprintf(filename,"/Users/thomashund/Documents/projects/QtDocs/MyConcurrentModel/data/s%d.dat",proto.myId);   // introduce my id?
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
=======
void Dialog::doTask(Protocol &proto) {
    proto.runSim();
>>>>>>> danielle-daniel-merge
}

void Dialog::on_pushButton_clicked() {
    int i = 0;
    QVector<Protocol> vector;
    std::vector<Protocol> protos(ui->spinBox->value()); // create vector of Protocols for QtConcurrent

<<<<<<< HEAD
       protos[i].saveflag = 1;                  // 1 = save final state var vals
       protos[i].readflag = 1;                  // 1 = save final state var vals

       if (int(protos[i].readflag)==1){
           try
           {
                //std::sprintf(filename,"/Users/thomashund/Documents/projects/QtDocs/MyConcurrentModel/data/r%d.dat",protos[i].myId);   // introduce my id?
                std::sprintf(filename,"/Applications/Qt/MyConcurrentModel/data/r%d.dat", protos[i].myId);
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
=======
    for( i = 0; i < ui->spinBox->value(); i++) {
        setupSim(&protos[i] ,i+1);
        vector.append(protos[i]);
>>>>>>> danielle-daniel-merge
    }
    //very brittle way of chosing cells!! //Dani
    if(ui->comboBox->currentText() == "Kurata"){

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
       else{
        QMessageBox msgBox;
        msgBox.setText("Code Currently Being Modified\n Please choose a different cell.\n (Kurata)");
        msgBox.exec();
    }
}

void setupSim(Protocol* proto, int simNum) {
    proto->cell = new ControlSa;
    proto->readfile = "./data/r"+ to_string(simNum) + ".dat"; // File to read SV ICs
    proto->savefile = "./data/s"+ to_string(simNum) + ".dat"; // File to save final SV
    proto->savefile = proto->savefile + ".dat";
    proto->dvarfile = "dvars.txt";  // File with SV to write.
    proto->pvarfile = "pvars.txt"; 
    proto->simvarfile = "simvars.txt";  // File to specify sim params
    proto->measfile = "mvars.txt"; // File containing property names to track
    proto->propertyoutfile = "./data/dt%d_%s" + to_string(simNum) + string(".dat");
    proto->dvarsoutfile = "./data/dt%d_dvars"+ to_string(simNum) + string(".dat");
    proto->finalpropertyoutfile = "./data/dss_%s"+ to_string(simNum) + string(".dat");
    proto->finaldvarsoutfile = "./data/dss_pvars"+ to_string(simNum) + string(".dat");

    proto->readpars(proto->pars, proto->simvarfile);  // Read in sim vars
    proto->readpvars(); 

    
    proto->resizemap(proto->cell->vars, proto->dvarfile, &(proto->datamap));  // use names in dvars.txt to resize datamap
    
    
    proto->initializeMeasure(int(proto->maxmeassize));   //initializeMeasures from measfile
    
    proto->douts = new Output[proto->getNeededDOutputSize()];   // Array of i/o data streams
   
}

