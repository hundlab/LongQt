#include "conductivityeditor.h"
#include "ui_conductivityeditor.h"
ConductivityEditor::Edge ConductivityEditor::Edge::minimize(const Edge e) const {
	Edge n = e;
	switch(e.p) {
		case left:
			if(n.x >0) {
				n.x--;
				n.p = right;
			}
			break;
		case top:
			if(n.y >0) {
				n.y--;
				n.p = bottom;
			}
			break;
		default:
			break;
	}
	return n;
}
bool ConductivityEditor::Edge::operator <(Edge e) const {
	Edge a = this->minimize(*this);
	Edge b = this->minimize(e);
	if(b.x < a.x) {
		if(a.y==b.y&&a.p==left&&b.p==right&&a.x - b.x == 1) {
			return false;
		}
		return true;
	} else if(b.x > a.x) {
		return false;
	}
	if(b.y < a.y) {
		if(a.p==top&&b.p==bottom&&a.y - e.y == 1) {
			return false;
		}
		return true;
	} else if(b.y > a.y) {
		return false;
	}
	if(b.p < a.p) {
		return true;
	} else {
		return false;
	}
}
bool ConductivityEditor::Edge::operator ==(Edge e) const {
	if(e.x == this->x&&e.y == this->y&&e.p==this->p)
		return true;
	if(e.y == this->y) {
	if(e.x+1==this->x&&e.p==right&&this->p==left)
		return true;	
	if(e.x==this->x+1&&e.p==left&&this->p==right)
		return true;
	}
	if(e.x == this->x) {
	if(e.y+1==this->y&&e.p==bottom&&this->p==top)
		return true;
	if(e.y==this->y+1&&e.p==top&&this->p==bottom)
		return true;
	}
	return false;
}

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
				this->setConductivity(e, val);
			} else {
				this->setConductivity(e, maxVal);
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
	//check for out of bounds
	if(e.y<0||e.x<0||e.y>=this->model->rowCount()||e.x>=this->model->columnCount()) {
		return;
	}
	if(this->visited.count(e) == 0) {
		next.insert(e);
	}

}

void ConductivityEditor::setConductivity(Edge e, double val) {
	//check for edge cases
	if((e.y==0&&e.p==top)||(e.y==this->model->rowCount()-1&&e.p==bottom)||(e.x==0&&e.p==left)||(e.x==this->model->columnCount()-1&&e.p==right)) {
		return;
	}
	this->model->setData(this->model->index(e.y,e.x,QModelIndex()).child(1,e.p),val);
}
