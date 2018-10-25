#include "conductivityeditor.h"
#include "ui_conductivityeditor.h"
#include "cellutils.h"
using namespace std;
using namespace LongQt;

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
	this->model->setPercent(ui->percValuesCheckBox->isChecked());
	this->setConductivities(ui->startingValueDoubleSpinBox->value(), ui->incrementAmountDoubleSpinBox->value(), ui->distanceSpinBox->value(), ui->valueMaximumDoubleSpinBox->value());
}
void ConductivityEditor::setConductivities(double startVal, double incAmount, int maxDist, double maxVal) {
	this->getInitial(); 
	int i = 0;
	while(current.size() > 0 && i <= maxDist) {
		for(const pair<int,int>& e: this->current) {
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
		this->add({toAdd.column(),toAdd.row()},this->current);
	}
}


void ConductivityEditor::getNext() {
	set<pair<int,int>> next;
	for(const pair<int,int>& e: this->current) {
		this->add({e.first,e.second+1}, next);
		this->add({e.first,e.second-1}, next);
		this->add({e.first+1,e.second}, next);
		this->add({e.first-1,e.second}, next);
	}
	this->current = next;
}
//need to check for duplicates
void ConductivityEditor::add(pair<int,int> e, set<pair<int,int>>& next) {
	//check for out of bounds
	if(e.second<0||e.first<0||e.second>=this->model->rowCount()||e.first>=this->model->columnCount()) {
		return;
	}
	if(this->visited.count(e) == 0) {
		next.insert(e);
	}
}

void ConductivityEditor::setConductivity(pair<int,int> e, double val) {
	for(int i = 0; i < 4; i++) {
	//check for edge cases
		if((e.second==0&&i==CellUtils::top)||
				(e.second==this->model->rowCount()-1&&i==CellUtils::bottom)||
				(e.first==0&&i==CellUtils::left)||
				(e.first==this->model->columnCount()-1&&i==CellUtils::right)) {
			continue;
		}
		//check for overlaping edges in current
/*		if((current.count(make_pair(e.first -1, e.second)) == 1 && i == CellUtils::left) ||
				(current.count(make_pair(e.first +1, e.second)) == 1 && i == CellUtils::right) ||
				(current.count(make_pair(e.first, e.second +1)) == 1 && i == CellUtils::bottom) ||
				(current.count(make_pair(e.first, e.second -1)) == 1 && i == CellUtils::top)) 
		{
			continue;
        }*/
		//check for overlaping edges in visited
		if((visited.count(make_pair(e.first -1, e.second)) == 1 && i == CellUtils::left) ||
				(visited.count(make_pair(e.first +1, e.second)) == 1 && i == CellUtils::right) ||
				(visited.count(make_pair(e.first, e.second +1)) == 1 && i == CellUtils::bottom) ||
				(visited.count(make_pair(e.first, e.second -1)) == 1 && i == CellUtils::top)) 
		{
			continue;
		}
		this->model->setData(this->model->index(e.second,e.first,QModelIndex()).child(1,i),val);
	}
}
