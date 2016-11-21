#include "conductivityeditor.h"
#include "ui_conductivityeditor.h"

ConductivityEditor::ConductivityEditor(QTableView* view, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConductivityEditor)
{
    ui->setupUi(this);
		this->view = view;
		this->model = (GridModel*)view->model();
}

ConductivityEditor::~ConductivityEditor()
{
    delete ui;
}
void ConductivityEditor::on_applyButton_clicked()
{
	this->setConductivities(ui->startingValueDoubleSpinBox->value(), ui->incrementAmountDoubleSpinBox->value(), ui->distanceSpinBox->value(), ui->valueMaximumDoubleSpinBox->value());

}
void ConductivityEditor::setConductivities(double startVal, double incAmount, int maxDist, double maxVal) {
	this->getInitial(); 
	int i = 0;
	while(current.size() > 0 && i <= maxDist) {
		for(const Edge& e: this->current) {
			//val is not correct
			double val = startVal+incAmount*i;
			if(val < maxVal) {
				this->model->setData(this->model->index(e.y,e.x,QModelIndex()).child(1,e.p),val);
			} else {
				this->model->setData(this->model->index(e.y,e.x,QModelIndex()).child(1,e.p),maxVal);
			}
			this->visited.insert(e);
		}
		++i;
		this->getNext();	
	}
	this->current.clear();
	this->visited.clear();
}

void ConductivityEditor::getInitial() {
	auto selected = this->view->selectionModel()->selectedIndexes();
	for(auto toAdd : selected) {
		this->add({toAdd.column(),toAdd.row(),top},this->current);
		this->add({toAdd.column(),toAdd.row(),right},this->current);
		this->add({toAdd.column(),toAdd.row(),bottom},this->current);
		this->add({toAdd.column(),toAdd.row(),left},this->current);
	}
}

void ConductivityEditor::getNext() {
	set<Edge> next;
	for(const Edge& e: this->current) {
		switch(e.p) {
			case top:
				this->add({e.x,e.y+1,left}, next);
				this->add({e.x,e.y+1,right}, next);
				this->add({e.x,e.y,left}, next);
				this->add({e.x,e.y,right}, next);
				this->add({e.x+1,e.y,top}, next);
				this->add({e.x-1,e.y,top}, next);
				break;
			case bottom:
				this->add({e.x,e.y-1,left}, next);
				this->add({e.x,e.y-1,right}, next);
				this->add({e.x,e.y,left}, next);
				this->add({e.x,e.y,right}, next);
				this->add({e.x+1,e.y,bottom}, next);
				this->add({e.x-1,e.y,bottom}, next);
				break;
			case right:
				this->add({e.x+1,e.y,top}, next);
				this->add({e.x+1,e.y,bottom}, next);
				this->add({e.x,e.y,top}, next);
				this->add({e.x,e.y,bottom}, next);
				this->add({e.x,e.y-1,right}, next);
				this->add({e.x,e.y+1,right}, next);
				break;
			case left:
				this->add({e.x-1,e.y,top}, next);
				this->add({e.x-1,e.y,bottom}, next);
				this->add({e.x,e.y,top}, next);
				this->add({e.x,e.y,bottom}, next);
				this->add({e.x,e.y-1,left}, next);
				this->add({e.x,e.y+1,left}, next);
				break;
		}
	}
	this->current = next;
}
//need to check for duplicates
void ConductivityEditor::add(Edge e, set<Edge>& next) {
	//check for edge cases
	if((e.y==0&&e.p==top)||(e.y==this->model->rowCount()-1&&e.p==bottom)||(e.x==0&&e.p==left)||(e.x==this->model->columnCount()-1&&e.p==right)) {
		return;
	}
	//check for out of bounds
	if(e.y<0||e.x<0||e.y>=this->model->rowCount()||e.x>=this->model->columnCount()) {
		return;
	}
	if(this->visited.count(e) == 0) {
		next.insert(e);
	}

}

