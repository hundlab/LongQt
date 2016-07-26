#include "gridSettup.h"
#include "cellUtils.h"

#include <QHBoxLayout>
#include <QLabel>

//###########################
//gridNode
//###########################
gridNode::gridNode(Node* node, int X, int Y, gridCell* parentCell) {
//class variabels
    this->node = node;
    this->info = new cellInfo;
    this->info->X = X;
    this->info->Y = Y;
    this->parentCell = parentCell;
    cellType = new QComboBox();
    stimNode = new QCheckBox("Stimulate");
    measNode = new QCheckBox("Measure");
    cellMap = cellUtils().cellMap;
    cellMap["Inexcitable Cell"] = [] () {return new Cell;};
//setup variables
    for(auto it : cellMap) {
        cellType->addItem(it.first.c_str());
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
pair<int,int> gridNode::getNodePair() {
    return make_pair(this->info->X,this->info->Y);
}
void gridNode::changeCell(QString type) {
    try {
        if(node->cell->type != type.toStdString()) {
            info->cell = cellMap.at(type.toStdString())();
            info->dx = parentCell->par("dx");
            info->dy = parentCell->par("dy");
            info->np = parentCell->par("np");
            if(info->cell->par("dtmin") < parentCell->par("dtmin")) {
                parentCell->setPar("dtmin", info->cell->par("dtmin"));
            }
            if(info->cell->par("dtmed") < parentCell->par("dtmed")) {
                parentCell->setPar("dtmed", info->cell->par("dtmed"));
            }
            if(info->cell->par("dtmax") < parentCell->par("dtmax")) {
                parentCell->setPar("dtmax", info->cell->par("dtmax"));
            }
            parentCell->getGrid()->setCellTypes(*info);
            emit cell_type_changed(type);
        }
        cellType->setCurrentText(type);
    } catch(const std::out_of_range&) {
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
    removeRowButton = new QPushButton(tr("- Row"));
    addColumnButton = new QPushButton(tr("+ Column"));
    removeColumnButton = new QPushButton(tr("- Column"));
    cellGrid = new QTableWidget(grid->rowCount(),grid->columnCount());
//setup nodes already present in grid
    {int i = 0;
    for(auto it = grid->fiber.begin();it != grid->fiber.end();it++,i++) {
        int j = 0;
        for(auto iv = it->nodes.begin();iv != it->nodes.end(); iv++,j++) {
            gridNode* cellWidget = new gridNode(*iv,i,j,((gridCell*)proto->cell));
            cellGrid->setCellWidget(j,i,cellWidget);
            connect(cellWidget, SIGNAL(cell_type_changed(QString)), this, SLOT(changeCellGroup(QString)));
            connect(cellWidget, &gridNode::stimNodeChanged, [this,cellWidget] (int status) {
                changeStimNodeList(status, cellWidget->getNodePair());
            });
            connect(cellWidget, &gridNode::measNodeChanged, [this,cellWidget] (int status) {
                changeMeasNodeList(status, cellWidget->getNodePair());
            });
        }
    }
    }
    connect(addColumnButton, &QPushButton::clicked, this, &gridSetupWidget::addColumn);
    connect(removeColumnButton, &QPushButton::clicked, this, &gridSetupWidget::removeColumn);
    connect(addRowButton, &QPushButton::clicked, this, &gridSetupWidget::addRow);
    connect(removeRowButton, &QPushButton::clicked, this, &gridSetupWidget::removeRow);
//setup layout
    QGridLayout* main_layout = new QGridLayout(this);
    main_layout->addWidget(addRowButton,0,0);
    main_layout->addWidget(removeRowButton,0,1);
    main_layout->addWidget(addColumnButton,0,2);
    main_layout->addWidget(removeColumnButton,0,3);
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
            if(n == NULL) {
                qDeleteAll(this->children());
                this->createMenu();
                return;
            }
            bool stim = proto->getStimNodes().end() != proto->getStimNodes().find(n->getNodePair());
            bool meas = proto->getDataNodes().end() != proto->getDataNodes().find(n->getNodePair());
            n->update(stim,meas);
        }
    }
    }
    this->cellGrid->resizeRowsToContents();
    this->cellGrid->resizeColumnsToContents();
}
void gridSetupWidget::changeStimNodeList(int status, pair<int,int> node) {
    switch(status) {
    case 2:
        proto->getStimNodes().insert(node);
    break;
    case 0:
        proto->getStimNodes().erase(node);
    break;
    }
}
void gridSetupWidget::changeMeasNodeList(int status, pair<int,int> node) {
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
    int i = static_cast<int>(grid->fiber.size())-1;
    int j = 0;
    auto newRow = grid->fiber[i];
    for(auto iv = newRow.nodes.begin(); iv!= newRow.nodes.end(); iv++,j++) {
        gridNode* cellWidget = new gridNode(*iv, i,j,((gridCell*)proto->cell));
        cellGrid->setCellWidget(i,j,cellWidget);
        connect(cellWidget, SIGNAL(cell_type_changed(QString)), this, SLOT(changeCellGroup(QString)));
        connect(cellWidget, &gridNode::stimNodeChanged, [this,cellWidget] (int status) {
            changeStimNodeList(status, cellWidget->getNodePair());
        });
        connect(cellWidget, &gridNode::measNodeChanged, [this,cellWidget] (int status) {
            changeMeasNodeList(status, cellWidget->getNodePair());
        });
    }
    updateMenu();
}
void gridSetupWidget::addColumn() {
    grid->addColumn(grid->columnCount());
    cellGrid->setColumnCount(cellGrid->columnCount()+1);
    int i = static_cast<int>(grid->fibery.size())-1;
    int j = 0;
    auto newRow = grid->fibery[i];
    for(auto iv = newRow.nodes.begin(); iv != newRow.nodes.end(); iv++,j++) {
        gridNode* cellWidget = new gridNode(*iv,j,i,((gridCell*)proto->cell));
        cellGrid->setCellWidget(j,i,cellWidget);
        connect(cellWidget, SIGNAL(cell_type_changed(QString)), this, SLOT(changeCellGroup(QString)));
        connect(cellWidget, &gridNode::stimNodeChanged, [this,cellWidget] (int status) {
            changeStimNodeList(status, cellWidget->getNodePair());
        });
        connect(cellWidget, &gridNode::measNodeChanged, [this,cellWidget] (int status) {
            changeMeasNodeList(status, cellWidget->getNodePair());
        });
    }
    updateMenu();
}
void gridSetupWidget::removeRow() {
    if(grid->rowCount() <= 0) {return;}
    grid->removeRow(grid->rowCount()-1);
    cellGrid->setRowCount(cellGrid->rowCount()-1);
    updateMenu();  
}
void gridSetupWidget::removeColumn() {
    if(grid->columnCount() <= 0) {return;}
    grid->removeColumn(grid->columnCount()-1);
    cellGrid->setColumnCount(cellGrid->columnCount()-1);
    updateMenu();
}
void gridSetupWidget::setGrid(Grid* grid) {
    this->grid = grid;
}
Grid* gridSetupWidget::getGrid() {
    return this->grid;
}
void gridSetupWidget::changeCellGroup(QString type) {
    QList<QTableWidgetSelectionRange> selected = cellGrid->selectedRanges();
    for(auto it : selected) {
        for(int i = it.topRow(); i <= it.bottomRow(); i++) {
            for(int j = it.leftColumn(); j <= it.rightColumn(); j++) {
                ((gridNode*)cellGrid->cellWidget(i,j))->changeCell(type);
            }
        }
    }
    emit cell_type_changed();
    updateMenu();
}
