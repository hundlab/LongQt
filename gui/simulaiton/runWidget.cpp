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

        connect(&watcher,SIGNAL(finished()),this,SLOT(finish()));
        connect(&watcher,SIGNAL(finished()),pdialog,SLOT(reset()));
        connect(&watcher,SIGNAL(progressRangeChanged(int,int)),pdialog,SLOT(setRange(int,int)));
        connect(&watcher,SIGNAL(progressValueChanged(int)),pdialog,SLOT(setValue(int)));
        connect(run_button, SIGNAL(clicked()),this,SLOT(run_sims()));
    }
void runWidget::setProto(Protocol* proto) {
    this->proto = proto;
}
void runWidget::run_sims() {
    int i = 0;
    run_button->setEnabled(false);
    working_dir.mkpath(working_dir.absolutePath());
    vector.clear();

    for( i = 0; i < proto->numtrials; i++) {
        proto->setTrial(i);
        proto->readfile = "r"+ to_string(i) + ".dat"; // File to read SV ICs
        proto->savefile = "s"+ to_string(i) + ".dat"; // File to save final SV
        proto->propertyoutfile = "dt%d_%s" + string(".dat");
        proto->dvarsoutfile = "dt%d_dvars" + string(".dat");
        proto->finalpropertyoutfile = "dss%d_%s" + string(".dat");
        proto->finaldvarsoutfile = "dss%d_pvars" + string(".dat");
        vector.append(proto->clone());
    }
   
    QFuture<void> next = QtConcurrent::map(vector,[] (Protocol* p) {
        if(p != NULL) {
            p->runTrial();
        }
    });  // pass vector of protocols to QtConcurrent

    watcher.setFuture(next);
    emit running();
}
void runWidget::setWorkingDir(QDir& dir) {
        working_dir = dir;
}
void runWidget::cancel() {
    watcher.cancel();
    run_button->setEnabled(true);
    emit canceled();
}
void runWidget::finish() {
    run_button->setEnabled(true);
    emit finished();
}
