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
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::doTask(Protocol &proto) {
    proto.runSim();
}

void Dialog::on_pushButton_clicked() {
    int i = 0;
    QVector<Protocol> vector;
    std::vector<Protocol> protos(ui->spinBox->value()); // create vector of Protocols for QtConcurrent

    
    for( i = 0; i < ui->spinBox->value(); i++) {
        setupSim(&protos[i] ,i+1);
        vector.append(protos[i]);
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

