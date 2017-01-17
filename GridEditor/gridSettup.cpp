#include "gridSettup.h"
#include "cellUtils.h"
#include "guiUtils.h"
#include "gridDelegate.h"

#include <QHBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QStandardPaths>
#include <QHeaderView>
//##############################
//gridSetupWidget
//##############################
gridSetupWidget::gridSetupWidget(QWidget* parent) : gridSetupWidget(new gridProtocol(), QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first(),parent) {}

gridSetupWidget::gridSetupWidget(gridProtocol* initial_proto, QDir workingDir, QWidget* parent) : QWidget(parent) {
    this->proto = initial_proto;
    this->workingDir = workingDir;
    this->parent = parent;
    this->grid = ((gridCell*)proto->cell)->getGrid();
	this->model = new GridModel(this->proto,this);

    this->createMenu();
}
void gridSetupWidget::createMenu() {
//initialize widgets
    rowInt = new QSpinBox();
    addRowButton = new QPushButton(tr("+ Row"));
    removeRowButton = new QPushButton(tr("- Row"));
    columnInt = new QSpinBox();
    addColumnButton = new QPushButton(tr("+ Column"));
    removeColumnButton = new QPushButton(tr("- Column"));
    toggleMeasure = new QPushButton(tr("Toggle Measure Selected"));
    toggleStim = new QPushButton(tr("Toggle Stimulate Selected"));
    QLabel* chooseTypeLabel = new QLabel(tr("Cell Type"));
    chooseType = new QComboBox();
    cellGrid = new QTableView();
//setup view
	this->cellGrid->setModel(this->model);
	this->cellGrid->setItemDelegate(new GridDelegate);
	this->cellGrid->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	this->cellGrid->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	this->cellGrid->setShowGrid(false);
	this->cellGrid->viewport()->setMouseTracking(true);
    auto cellMap = cellUtils().cellMap;
    cellMap["Inexcitable Cell"] = [] () {return new Cell;};
    for(auto it : cellMap) {
        chooseType->addItem(it.first.c_str());
    }
    rowInt->setRange(1,1000);
    columnInt->setRange(1,1000);
    connect(addColumnButton, &QPushButton::clicked, [this] () {
			this->model->insertColumns(this->model->columnCount(), this->columnInt->value());
			});
    connect(removeColumnButton, &QPushButton::clicked, [this] () {
			int num = this->columnInt->value();
			this->model->removeColumns(this->model->columnCount()-num,num);
			});
    connect(addRowButton, &QPushButton::clicked, [this] () {
			this->model->insertRows(this->model->rowCount(), this->rowInt->value());
			});
    connect(removeRowButton, &QPushButton::clicked, [this] () {
			int num = this->rowInt->value();
			this->model->removeRows(this->model->rowCount()-num,num);
			});
    connect(toggleStim, &QPushButton::clicked, this, &gridSetupWidget::toggleStimPressed);
    connect(toggleMeasure, &QPushButton::clicked, this, &gridSetupWidget::toggleMeasurePressed);
    connect(chooseType, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCellGroup(QString)));
    connect(cellGrid->selectionModel(),&QItemSelectionModel::selectionChanged, [this] (const QItemSelection& selected, const QItemSelection&) {
        if(!selected.isEmpty()) {
			bool oldState = this->chooseType->blockSignals(true);
            this->chooseType->setCurrentText(selected.first().topLeft().data().toString());
			this->chooseType->blockSignals(oldState);
        }
    });
	connect(this->model, &GridModel::cell_type_changed, this, &gridSetupWidget::cell_type_changed);
//setup layout
    QGridLayout* main_layout = new QGridLayout(this);
    main_layout->addWidget(rowInt,0,0);
    main_layout->addWidget(addRowButton,0,1);
    main_layout->addWidget(removeRowButton,0,2);
    main_layout->addWidget(columnInt,0,3);
    main_layout->addWidget(addColumnButton,0,4);
    main_layout->addWidget(removeColumnButton,0,5);
    main_layout->addWidget(toggleStim,1,0);
    main_layout->addWidget(toggleMeasure,1,1);
    main_layout->addWidget(chooseTypeLabel,1,2);
    main_layout->addWidget(chooseType,1,3);
    main_layout->addWidget(cellGrid,2,0,10,10);
    this->setLayout(main_layout);
}
void gridSetupWidget::setGrid(Grid* grid) {
    this->grid = grid;
//	delete this->model;
	this->model = new GridModel(this->proto);
	this->cellGrid->setModel(this->model);
}
Grid* gridSetupWidget::getGrid() {
    return this->grid;
}
gridProtocol* gridSetupWidget::getProtocol() {
	return this->proto;
}
void gridSetupWidget::changeCellGroup(QString type) {
    auto selected = this->cellGrid->selectionModel()->selectedIndexes();
    for(auto index : selected) {
		this->model->setData(index,type);
    }
}
void gridSetupWidget::toggleMeasurePressed() {
    auto selected = cellGrid->selectionModel()->selectedIndexes();
    for(auto index : selected) {
		this->model->setData(index.child(0,1),!index.child(0,1).data().toBool());
    }
}
void gridSetupWidget::toggleStimPressed() {
    auto selected = cellGrid->selectionModel()->selectedIndexes();
    for(auto index : selected) {
		this->model->setData(index.child(0,0),!index.child(0,0).data().toBool());
    }
}
QTableView* gridSetupWidget::view() {
	return this->cellGrid;
}
GridModel* gridSetupWidget::getModel() {
	return this->model;
}
void gridSetupWidget::reset() {
	this->model->reloadModel();
}
