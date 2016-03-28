#include "runWidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QtConcurrent>

runWidget::runWidget(Protocol* proto, QDir working_dir, QWidget* parent) {
        this->parent = parent;
        this->proto = proto;
        this->working_dir = working_dir;
        QGridLayout* run_button_container_layout = new QGridLayout();
        run_button = new QPushButton("Run Simulations");
        pdialog = new QProgressBar();
        run_button_container_layout->addWidget(run_button, 0,0,1,2);
        run_button_container_layout->addWidget(new QLabel("Progress"), 1,0);
        run_button_container_layout->addWidget(pdialog, 1,1);
        this->setLayout(run_button_container_layout);

        connect(run_button, SIGNAL(clicked()),this,SLOT(run_sims()));
    }
void runWidget::run_sims() {
    int i = 0;
    Protocol* temp;
    run_button->setEnabled(false);
    working_dir.mkpath(working_dir.absolutePath());

    for( i = 0; i < proto->numtrials; i++) {
        proto->setTrial(i);
        temp = new Protocol(*proto);
        temp->readfile = "/r"+ to_string(i) + ".dat"; // File to read SV ICs
        temp->savefile = "/s"+ to_string(i) + ".dat"; // File to save final SV
        temp->propertyoutfile = "/dt%d_%s" + string(".dat");
        temp->dvarsoutfile = "/dt%d_dvars" + string(".dat");
        temp->finalpropertyoutfile = "/dss%d_%s" + string(".dat");
        temp->finaldvarsoutfile = "/dss%d_pvars" + string(".dat");
        vector.append(*temp);
    }
    connect(&watcher,SIGNAL(finished()),this,SLOT(finish()));
    connect(&watcher,SIGNAL(finished()),pdialog,SLOT(reset()));
    connect(&watcher,SIGNAL(progressRangeChanged(int,int)),pdialog,SLOT(setRange(int,int)));
    connect(&watcher,SIGNAL(progressValueChanged(int)),pdialog,SLOT(setValue(int)));
    
    QFuture<void> next = QtConcurrent::map(vector,[] (Protocol& p) {p.runTrial();});  // pass vector of protocols to QtConcurrent

    watcher.setFuture(next);
    emit running();
}
void runWidget::setWorkingDir(QDir dir) {
        working_dir = dir;
}
void runWidget::cancel() {
    watcher.cancel();
    vector.clear();
    run_button->setEnabled(true);
    emit canceled();
}
void runWidget::finish() {
    run_button->setEnabled(true);
    emit finished();
}
