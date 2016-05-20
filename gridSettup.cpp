#include "gridSettup.h"

#include <QHBoxLayout>
#include <QLabel>

//###########################
//gridNode
//###########################
gridNode::gridNode(Node* node, int X, int Y, gridCell* parentCell,  map<string, CellInitializer> cellMap) {
//class variabels
    this->node = node;
    this->cellMap = cellMap;
    this->info = new cellInfo;
    this->info->X = X;
    this->info->Y = Y;
    this->parentCell = parentCell;
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
    this->layout()->addWidget(new QLabel("Type:"));
    this->layout()->addWidget(cellType);
    connect(cellType, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCell(QString)));
    connect(stimNode, SIGNAL(stateChanged(int)), this, SIGNAL(stimNodeChanged(int)));
    connect(measNode, SIGNAL(stateChanged(int)), this, SIGNAL(measNodeChanged(int)));
}
gridNode::~gridNode() {}
Node* gridNode::getNode() {
    return node;
}
void gridNode::changeCell(QString type) {
    try {
        if(node->cell->type != type.toStdString()) {
            info->cell = cellMap.at(type.toStdString())();
            info->dx = *parentCell->vars["dx"];
            info->dy = *parentCell->vars["dy"];
            info->np = *parentCell->vars["np"];
            parentCell->getGrid()->setCellTypes(*info);
            emit cell_type_changed();
        }
    } catch(const std::out_of_range& oor) {
        return;
    }
}
void gridNode::update(bool stim, bool meas) {
    changeCell(node->cell->type);
    stimNode->setChecked(stim);
    measNode->setChecked(meas);
}
//##############################
//gridSetupWidget
//##############################
gridSetupWidget::gridSetupWidget(gridProtocol* initial_proto, QDir workingDir, QWidget* parent) {
    this->proto = initial_proto;
    this->workingDir = workingDir;
    this->parent = parent;
    this->grid = ((gridCell*)proto->cell)->getGrid();

    this->createMenu();
}
void gridSetupWidget::createMenu() {
//initialize widgets
    addRowButton = new QPushButton(tr("+ Row"));
    addColumnButton = new QPushButton(tr("+ Column"));
    cellGrid = new QTableWidget(grid->rowCount(),grid->columnCount());
//setup nodes already present in grid
    {int i = 0;
    for(auto it = grid->fiber.begin();it != grid->fiber.end();it++,i++) {
        int j = 0;
        for(auto iv = it->nodes.begin();iv != it->nodes.end(); iv++,j++) {
            gridNode* cellWidget = new gridNode(*iv,i,j,((gridCell*)proto->cell), proto->getCellMap());
            cellGrid->setCellWidget(i,j,cellWidget);
            connect(cellWidget, SIGNAL(cell_type_changed()), this, SIGNAL(cell_type_changed()));
            connect(cellWidget, &gridNode::stimNodeChanged, [this,cellWidget] (int status) {
                changeStimNodeList(status, cellWidget->getNode());
            });
            connect(cellWidget, &gridNode::measNodeChanged, [this,cellWidget] (int status) {
                changeMeasNodeList(status, cellWidget->getNode());
            });

        }
    }
    }
    connect(addColumnButton, &QPushButton::clicked, this, &gridSetupWidget::addColumn);
    connect(addRowButton, &QPushButton::clicked, this, &gridSetupWidget::addRow);
//setup layout
    QGridLayout* main_layout = new QGridLayout(this);
    main_layout->addWidget(addRowButton,0,0);
    main_layout->addWidget(addColumnButton,0,1);
    main_layout->addWidget(cellGrid,1,0,10,10);
    this->setLayout(main_layout);
    updateMenu();
}
void gridSetupWidget::updateMenu() {
    {int i = 0;
    for(auto it = grid->fiber.begin(); it!= grid->fiber.end();it++,i++) {
        int j = 0;
        for(auto iv = it->nodes.begin();iv != it->nodes.end(); iv++,j++) {
            gridNode* n = (gridNode*)cellGrid->cellWidget(i,j);
            bool stim = proto->getStimNodes().end() != proto->getStimNodes().find(n->getNode());
            bool meas = proto->getDataNodes().end() != proto->getDataNodes().find(n->getNode());
            n->update(stim,meas);
        }
    }
    }
    this->cellGrid->resizeRowsToContents();
    this->cellGrid->resizeColumnsToContents();
}
void gridSetupWidget::changeStimNodeList(int status, Node* node) {
    switch(status) {
    case 2:
        proto->getStimNodes().insert(node);
    break;
    case 0:
        proto->getStimNodes().erase(node);
    break;
    }
}
void gridSetupWidget::changeMeasNodeList(int status, Node* node) {
    switch(status) {
    case 2:
        proto->getDataNodes().insert(node);
    break;
    case 0:
        proto->getDataNodes().erase(node);
    break;
    }
}
void gridSetupWidget::addRow() {
    grid->addRow(grid->rowCount());
    cellGrid->setRowCount(cellGrid->rowCount()+1);
    int i = grid->fiber.size()-1;
    int j = 0;
    auto newRow = grid->fiber[i];
    for(auto iv = newRow.nodes.begin(); iv!= newRow.nodes.end(); iv++,j++) {
        gridNode* cellWidget = new gridNode(*iv, i,j,((gridCell*)proto->cell),proto->getCellMap());
        cellGrid->setCellWidget(i,j,cellWidget);
        connect(cellWidget, SIGNAL(cell_type_changed()), this, SIGNAL(cell_type_changed()));
        connect(cellWidget, &gridNode::stimNodeChanged, [this,cellWidget] (int status) {
            changeStimNodeList(status, cellWidget->getNode());
        });
        connect(cellWidget, &gridNode::measNodeChanged, [this,cellWidget] (int status) {
            changeMeasNodeList(status, cellWidget->getNode());
        });
    }
    updateMenu();
}
void gridSetupWidget::addColumn() {
    grid->addColumn(grid->columnCount());
    cellGrid->setColumnCount(cellGrid->columnCount()+1);
    int i = grid->fibery.size()-1;
    int j = 0;
    auto newRow = grid->fibery[i];
    for(auto iv = newRow.nodes.begin(); iv != newRow.nodes.end(); iv++,j++) {
        gridNode* cellWidget = new gridNode(*iv,j,i,((gridCell*)proto->cell), proto->getCellMap());
        cellGrid->setCellWidget(j,i,cellWidget);
        connect(cellWidget, SIGNAL(cell_type_changed()), this, SIGNAL(cell_type_changed()));
        connect(cellWidget, &gridNode::stimNodeChanged, [this,cellWidget] (int status) {
            changeStimNodeList(status, cellWidget->getNode());
        });
        connect(cellWidget, &gridNode::measNodeChanged, [this,cellWidget] (int status) {
            changeMeasNodeList(status, cellWidget->getNode());
        });
    }
    updateMenu();
}
void gridSetupWidget::setGrid(Grid* grid) {
    this->grid = grid;
}
Grid* gridSetupWidget::getGrid() {
    return this->grid;
}
