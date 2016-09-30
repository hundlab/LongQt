#include "gridSettup.h"
#include "cellUtils.h"
#include "guiUtils.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPen>

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
    this->setToolTip("Inexcitable Cell");
//    this->setAutoFillBackground(true);
    this->setMinimumSize(30,30);
    this->stimStatus = false;
    this->measStatus = false;
//layout management
    this->setLayout(new QHBoxLayout(this));
//    this->layout()->addWidget(stimNode);
//    this->layout()->addWidget(measNode);
//    this->layout()->addWidget(new QLabel("Type:"));
//    this->layout()->addWidget(cellType);
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
bool gridNode::getStimStatus() {
    return this->stimStatus;
}
bool gridNode::getMeasStatus() {
    return this->measStatus;
}
void gridNode::changeCell(QString type) {
    try {
        if(node->cell->type != type.toStdString()) {
            info->cell = cellMap.at(type.toStdString())();
            info->dx = *parentCell->pars["dx"];
            info->dy = *parentCell->pars["dy"];
            info->np = *parentCell->pars["np"];
            if(*info->cell->pars["dtmin"] < *parentCell->pars["dtmin"]) {
                *parentCell->pars["dtmin"] = *info->cell->pars["dtmin"];
            }
            if(*info->cell->pars["dtmed"] < *parentCell->pars["dtmed"]) {
                *parentCell->pars["dtmed"] = *info->cell->pars["dtmed"];
            }
            if(*info->cell->pars["dtmax"] < *parentCell->pars["dtmax"]) {
                *parentCell->pars["dtmax"] = *info->cell->pars["dtmax"];
            }
            this->setToolTip(type);
            parentCell->getGrid()->setCellTypes(*info);
            emit cell_type_changed(type);
        }
        cellType->setCurrentText(type);
        QPalette palet(this->palette());
        palet.setColor(QPalette::Base, GuiUtils().genColor(this->cellType->currentIndex()));
        palet.setColor(QPalette::Highlight, GuiUtils().genColor(this->cellType->currentIndex(),20));
        palet.setColor(QPalette::HighlightedText, GuiUtils().genColor(this->cellType->currentIndex(),20));
        this->setPalette(palet);
    } catch(const std::out_of_range&) {
        return;
    }
}
void gridNode::update(bool stim, bool meas) {
    changeCell(node->cell->type);
    this->stimStatus = stim;
    this->measStatus = meas;
    ((QWidget*)this)->update();
}
void gridNode::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPen pen(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.fillRect(QRect(0,0,this->size().width(),this->size().height()),this->palette().base());
//    if(this->parent()->property("selected").isValid()) {
  //      painter.fillRect(QRect(0,0,this->size().width(),this->size().height()),this->palette().highlight());
    //}
    if(this->measStatus) {
        QRect rectangle(2, 2, this->size().width()-2, this->size().height()-2);
        if(!rectangle.isValid()) {
            return;
        }
        int startAngle = 0;
        int spanAngle = 360 * 16;
        painter.drawArc(rectangle, startAngle, spanAngle);
    }
    if(this->stimStatus) {
        QLine line1(2,2,this->size().width()-2, this->size().height()-2);
        QLine line2(this->size().width()-2, 2,2,this->size().height()-2);
        painter.drawLine(line1);
        painter.drawLine(line2);
    }
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
    cellGrid = new QTableWidget(grid->rowCount(),grid->columnCount());
//setup nodes already present in grid
    {int i = 0;
    for(auto it = grid->fiber.begin();it != grid->fiber.end();it++,i++) {
        int j = 0;
        for(auto iv = it->nodes.begin();iv != it->nodes.end(); iv++,j++) {
            gridNode* cellWidget = new gridNode(*iv,i,j,((gridCell*)proto->cell));
            cellGrid->setCellWidget(i,j,cellWidget);
//            connect(cellWidget, SIGNAL(cell_type_changed(QString)), this, SLOT(changeCellGroup(QString)));
            connect(cellWidget, &gridNode::stimNodeChanged, [this,cellWidget] (int status) {
                changeStimNodeList(status, cellWidget->getNodePair());
            });
            connect(cellWidget, &gridNode::measNodeChanged, [this,cellWidget] (int status) {
                changeMeasNodeList(status, cellWidget->getNodePair());
            });
        }
    }
    }
    auto cellMap = cellUtils().cellMap;
    cellMap["Inexcitable Cell"] = [] () {return new Cell;};
    for(auto it : cellMap) {
        chooseType->addItem(it.first.c_str());
    }
    rowInt->setRange(1,1000);
    columnInt->setRange(1,1000);
    connect(addColumnButton, &QPushButton::clicked, [this] () { this->addColumns(this->columnInt->value());});
    connect(removeColumnButton, &QPushButton::clicked, [this] () {this->removeColumns(this->columnInt->value());});
    connect(addRowButton, &QPushButton::clicked, [this] () { this->addRows(this->rowInt->value());});
    connect(removeRowButton, &QPushButton::clicked, [this] () {this->removeRows(this->rowInt->value());});
    connect(toggleStim, &QPushButton::clicked, this, &gridSetupWidget::toggleStimPressed);
    connect(toggleMeasure, &QPushButton::clicked, this, &gridSetupWidget::toggleMeasurePressed);
    connect(chooseType, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCellGroup(QString)));
    connect(cellGrid,&QTableWidget::itemSelectionChanged, [this] () {
        QList<QTableWidgetSelectionRange> selected = cellGrid->selectedRanges();
        if(!selected.isEmpty()) {
            this->chooseType->setCurrentText(((gridNode*)cellGrid->cellWidget(selected.first().topRow(),selected.first().leftColumn()))->getNode()->cell->type);
        }
    });
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
void gridSetupWidget::addRows(int num) {
    grid->addRows(num,-1);
    int rowCount = cellGrid->rowCount();
    cellGrid->setRowCount(rowCount+num);
//    for(auto it = )
/*    for(auto iv = newRow.nodes.begin(); iv!= newRow.nodes.end(); iv++,j++) {
        gridNode* cellWidget = new gridNode(*iv, i,j,((gridCell*)proto->cell));
        cellGrid->setCellWidget(i,j,cellWidget);
        connect(cellWidget, SIGNAL(cell_type_changed(QString)), this, SLOT(changeCellGroup(QString)));
        connect(cellWidget, &gridNode::stimNodeChanged, [this,cellWidget] (int status) {
            changeStimNodeList(status, cellWidget->getNodePair());
        });
        connect(cellWidget, &gridNode::measNodeChanged, [this,cellWidget] (int status) {
            changeMeasNodeList(status, cellWidget->getNodePair());
        });
    }*/
    updateMenu();
}
void gridSetupWidget::addColumns(int num) {
    grid->addColumns(num,-1);
    cellGrid->setColumnCount(cellGrid->columnCount()+1);
    int i = static_cast<int>(grid->fibery.size())-1;
    int j = 0;
    auto newRow = grid->fibery[i];
/*    for(auto iv = newRow.nodes.begin(); iv != newRow.nodes.end(); iv++,j++) {
        gridNode* cellWidget = new gridNode(*iv,j,i,((gridCell*)proto->cell));
        cellGrid->setCellWidget(j,i,cellWidget);
        connect(cellWidget, SIGNAL(cell_type_changed(QString)), this, SLOT(changeCellGroup(QString)));
        connect(cellWidget, &gridNode::stimNodeChanged, [this,cellWidget] (int status) {
            changeStimNodeList(status, cellWidget->getNodePair());
        });
        connect(cellWidget, &gridNode::measNodeChanged, [this,cellWidget] (int status) {
            changeMeasNodeList(status, cellWidget->getNodePair());
        });
    }*/
    updateMenu();
}
void gridSetupWidget::removeRows(int num) {
    if(grid->rowCount() <= num-1) {return;}
    grid->removeRows(num,-1);
    cellGrid->setRowCount(cellGrid->rowCount()-num);
    updateMenu();  
}
void gridSetupWidget::removeColumns(int num) {
    if(grid->columnCount() <= num-1) {return;}
    grid->removeColumns(num,-1);
    cellGrid->setColumnCount(cellGrid->columnCount()-num);
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
void gridSetupWidget::toggleMeasurePressed() {
    QList<QTableWidgetSelectionRange> selected = cellGrid->selectedRanges();
    set<pair<int,int>> data = this->proto->getDataNodes();
    for(auto it : selected) {
        for(int i = it.topRow(); i <= it.bottomRow(); i++) {
            for(int j = it.leftColumn(); j <= it.rightColumn(); j++) {
                if(data.find(make_pair(i,j)) != data.cend()) {
                    this->changeMeasNodeList(0, make_pair(i,j));
                } else {
                    this->changeMeasNodeList(2, make_pair(i,j));
                }
            }
        }
    }
    updateMenu();
}
void gridSetupWidget::toggleStimPressed() {
    QList<QTableWidgetSelectionRange> selected = cellGrid->selectedRanges();
    for(auto it : selected) {
        for(int i = it.topRow(); i <= it.bottomRow(); i++) {
            for(int j = it.leftColumn(); j <= it.rightColumn(); j++) {
                if(this->proto->getStimNodes().find(make_pair(i,j)) != this->proto->getStimNodes().end()) {
                    this->changeStimNodeList(0, make_pair(i,j));
                } else {
                    this->changeStimNodeList(2, make_pair(i,j));
                }
            }
        }
    }
    updateMenu();
}
