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
        note_box = new QTextEdit;
        note_box->setAcceptRichText(false);
        note_box_name = new QLineEdit("notes");
        run_button_container_layout->addWidget(new QLabel("Notes on the simulation will be stored in the simulation folder upon running the simulation"), 0,0,1,2);
        run_button_container_layout->addWidget(new QLabel("Filename"), 1,0,1,1);
        run_button_container_layout->addWidget(note_box_name, 1,1,1,1);
        run_button_container_layout->addWidget(note_box,2,0,1,2);
        run_button_container_layout->addWidget(run_button, 3,0,1,2);
        run_button_container_layout->addWidget(new QLabel("Progress"), 4,0);
        run_button_container_layout->addWidget(pdialog, 4,1);
        this->setLayout(run_button_container_layout);
        connect(run_button, SIGNAL(clicked()),this,SLOT(run_sims()));
    }
void runWidget::setProto(Protocol* proto) {
    this->proto = proto;
}
void runWidget::run_sims() {
    run_button->setEnabled(false);
    working_dir.mkpath(working_dir.absolutePath());
    QFile* note_file = new QFile(working_dir.absolutePath()+ "/" + note_box_name->text() + ".txt");
    note_file->open(QIODevice::WriteOnly|QIODevice::Text);
    if(note_file->isOpen()) {
        QTextStream out(note_file);
        out << note_box->toPlainText();
    } else {
        cerr << "note file could not be opened";
    }
    this->manager = new SimManager(this->proto);
    connect(manager,&SimManager::finished,this,&runWidget::finish);
    connect(manager,SIGNAL(numTrialsChanged(int,int)),pdialog,SLOT(setRange(int,int)));
    connect(manager,SIGNAL(currentTrialChanged(int)),pdialog,SLOT(setValue(int)));
    connect(manager, &SimManager::started, this, &runWidget::running);
 
    this->manager->runSims();
}
void runWidget::setWorkingDir(QDir& dir) {
        working_dir = dir;
}
void runWidget::cancel() {
    this->manager->quit();
    this->manager->deleteLater();
    run_button->setEnabled(true);
    pdialog->reset();
    emit canceled();
}
void runWidget::finish() {
    this->manager->deleteLater();
    run_button->setEnabled(true);
    pdialog->reset();
    emit finished();
}
