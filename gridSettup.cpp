#include "gridSettup.h"

#include <QHBoxLayout>
#include <QLabel>

//###########################
//gridNode
//###########################
gridNode::gridNode(Node* node, map<string, CellInitializer> cellMap) {
//class variabels
    this->node = node;
    this->cellMap = cellMap;
    cellType = new QComboBox();
    stimNode = new QCheckBox("Stimulate");
    measNode = new QCheckBox("Measure");
//setup variables
    for(auto it = cellMap.begin(); it != cellMap.end(); it++) {
        cellType->addItem(it->first.c_str());
    }
//layout management
    this->setLayout(new QHBoxLayout(this));
    this->layout()->addWidget(stimNode);
    this->layout()->addWidget(measNode);
    this->layout()->addWidget(QLabel("Type:")):
    this->layout()->addWidget(cellType);
    connect(cellType, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCell(QString)));
    connect(stimNode, SIGNAL(stateChanged(int)), this, SIGNAL(stimNodeChanged(int)));
    connect(measNode, SIGNAL(stateChanged(int)), this, SIGNAL(measNodeChanged(int)));
}
void gridNode::changeCell(QString type) {
    try {
        node->cell = cellMap.at(type.toStdString())();
        emit cell_type_changed();
    } catch(const std::out_of_range& oor) {
        return;
    }
}
//##############################
//gridSetupWidget
//##############################
gridSetupWidget::gridSetupWidget(Protocol* initial_proto, QDir workingDir, QWidget* parent = 0) {
    this->proto = initial_proto;
    this->workingDir = workingDir;
    this->parent = parent;

    this->createMenu();
}
void gridSetupWidget::createMenu() {
//initialize widgets
    addRowButton = new QPushButton(tr("+ Row"));
    addColumnButton = new QPushButton(tr("+ Column"));
    cellGrid = new QTableWidget(grid->rowCount(),grid->columnCount());
//setup nodes already present in grid
    {int i = 0;
    for(auto it = grid->fiber.begin(); grid->fiber.end();it++,i++) {
        int j = 0;
        for(auto iv = it->nodes.begin(); it->nodes.end(); iv++,j++) {
            gridNode* cellWidget = new gridNode(*iv, proto->cellMap());
            cellGrid->setCellWidget(i,j,cellWidget)
            connect(cellWidget, SIGNAL(cell_type_changed()), this, SIGNAL(cell_type_changed()));
            connect(cellWidget, &gridNode::stimNodeChanged, [cellWidget] (int status) {
                changeStimNodeList(int status, cellWidget->node());
            });
            connect(cellWidget, &gridNode::measNodeChanged, [cellWidget] (int status) {
                changeMeasNodeList(int status, cellWidget->node());
            });

        }
        connect(addColumnButton, &QPushButton::clicked, &addColumn);
        connect(addRowButton, &QPushButton::clicked, &addRow);
    }
//setup layout
    this->setLayout(new QGridLayout(this));
    this->layout()->addWidget(addRowButton,0,0);
    this->layout()->addWidget(addColumnButton,0,1);
    this->layout()->addWidget(cellGrid,1,0,10,10);
//sync menu GUI with grid
    updateMenu();
}
void gridSetupWidget::updateMenu() {
    
}
void gridSetupWidget::addRow() {
    grid->addRow(grid->rowCount());
}
void gridSetupWidget::addColumn() {
    grid->addColumn(grid->columnCount());
}
void gridSetupWidget::setGrid(Grid* grid) {
    this->grid = grid;
}
Grid* gridSetupWidget::getGrid() {
    return this->grid;
}
