#include "lqgrideditor.h"
#include "ui_lqgrideditor.h"
#include <QStatusBar>

LQGridEditor::LQGridEditor(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LQGridEditor)
{
	ui->setupUi(this);
	this->setStatusBar(new QStatusBar());
	this->gridView = this->ui->setup->view();
}

LQGridEditor::~LQGridEditor()
{
	delete ui;
}

void LQGridEditor::on_actionSet_Conductivities_triggered()
{
	if(this->condEdit == 0) {
		this->condEdit = new ConductivityEditor(this->gridView);
		this->condEdit->show();
		connect(this->condEdit, &QObject::destroyed, [this] () {
			this->condEdit = 0;
		});
	} else {
		this->condEdit->show();
		this->condEdit->raise();
	}
}
