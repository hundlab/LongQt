#include "pvarsgrid.h"
#include <QDebug>

PvarsGrid::PvarsGrid(Grid* grid): grid(grid) {}

CellPvars* PvarsGrid::clone() {
    return new PvarsGrid(*this);
}

void PvarsGrid::setGrid(Grid* grid) {
    this->grid = grid;
}

void PvarsGrid::setIonChanParams() {
    Cell* cell = 0;
    for(auto& pvar : *this->__pvars) {
        for(auto& oneCell : pvar.second->cells) {
            cell = this->grid->findNode({oneCell.first.second,oneCell.first.first})->cell;
            *cell->pars.at(pvar.first) = oneCell.second;
        }
    }
}
void PvarsGrid::writePvars(QXmlStreamWriter& xml) {
    xml.writeStartElement("pvars");
    for(auto& pvar : *this->__pvars) {
        xml.writeStartElement("pvar");
        xml.writeAttribute("name", pvar.first.c_str());
        xml.writeTextElement("distribution_type", QString::number(pvar.second->dist));
        xml.writeTextElement("value0", QString::number(pvar.second->val[0]));
        xml.writeTextElement("value1", QString::number(pvar.second->val[1]));
        xml.writeTextElement("maxDist",QString::number(pvar.second->maxDist));
        xml.writeTextElement("maxVal",QString::number(pvar.second->maxVal));
        xml.writeStartElement("cells");
        for(auto& cell : pvar.second->cells) {
            xml.writeStartElement("cell");
            xml.writeAttribute("x", QString::number(cell.first.first));
            xml.writeAttribute("y", QString::number(cell.first.second));
            if(pvar.second->startCells.count(cell.first) > 0) {
                xml.writeAttribute("start", "1");
            }
            xml.writeCharacters(QString::number(cell.second));
            xml.writeEndElement();
        }
        xml.writeEndElement();
        xml.writeEndElement();
    }
    xml.writeEndElement();
}

void PvarsGrid::readPvars(QXmlStreamReader& xml) {
    //	while(!xml.atEnd() && xml.name() != "file") {
    //		xml.readNext();
    //	}
    //	xml.readNext();
    while(!xml.atEnd() && xml.name() != "pvars") {
        xml.readNext();
    }
    this->handlePvars(xml);
}

void PvarsGrid::handlePvars(QXmlStreamReader& xml) {
    if(xml.atEnd()) return;
    while(xml.readNextStartElement() && xml.name()=="pvar"){
        this->handlePvar(xml);
    }
    xml.skipCurrentElement();
}

void PvarsGrid::handlePvar(QXmlStreamReader& xml) {
    if(xml.atEnd()) return;
    pair<string,MIonChanParam*> pvar;
    pvar.second = new MIonChanParam;
    pvar.first = xml.attributes().value("name").toString().toStdString();
    while(xml.readNextStartElement()) {
        if(xml.name()=="distribution_type") {
            xml.readNext();
            pvar.second->dist = static_cast<Distribution>(xml.text().toInt());
            xml.skipCurrentElement();
        } else if(xml.name()=="value0") {
            xml.readNext();
            pvar.second->val[0] = xml.text().toDouble();
            xml.skipCurrentElement();
        } else if(xml.name()=="value1") {
            xml.readNext();
            pvar.second->val[1] = xml.text().toDouble();
            xml.skipCurrentElement();
        }  else if(xml.name()=="maxDist") {
            xml.readNext();
            pvar.second->maxDist = xml.text().toInt();
            xml.skipCurrentElement();
        } else if(xml.name()=="maxVal") {
            xml.readNext();
            pvar.second->maxVal = xml.text().toDouble();
            xml.skipCurrentElement();
        } else if(xml.name()=="cells") {
            while(xml.readNextStartElement() && xml.name()=="cell"){
                bool start = false;
                pair<pair<int,int>,double> p = this->handleCell(xml,start);
                if(start) {
                    pvar.second->startCells.insert(p.first);
                }
                pvar.second->cells[p.first] = p.second;
            }
        }
        else {
            xml.skipCurrentElement();
        }
    }
    this->pvars[pvar.first] = pvar.second;
}

pair<pair<int,int>,double> PvarsGrid::handleCell(QXmlStreamReader& xml, bool& start) {
    pair<pair<int,int>,double> c;
    if(xml.atEnd()) return c;
    c.first.first = xml.attributes().value("x").toInt();
    c.first.second = xml.attributes().value("y").toInt();
    if(xml.attributes().value("start").toInt() == 1) {
        start = true;
    } else {
        start = false;
    }
    xml.readNext();
    c.second = xml.text().toDouble();
    xml.skipCurrentElement();
    return c;
}

string PvarsGrid::MIonChanParam::str(string name) {
    string text = "(X <column>, Y <row>)\n";
    text += name+"\n";
    for(auto& cell: this->cells) {
        text += "(" + std::to_string(cell.first.first) +","+
            std::to_string(cell.first.second) +") = "+std::to_string(cell.second)+"\n";
    }
    return text;
}
void PvarsGrid::insert(string name,IonChanParam param) {
    MIonChanParam* nparam = new MIonChanParam(const_cast<IonChanParam&>(param));
    this->calcIonChanParam(nparam);
    (*this->__pvars)[name] = nparam;
}
void PvarsGrid::setMaxDistAndVal(string varname, int maxDist, double maxVal) {
    try {
        MIonChanParam* param = this->__pvars->at(varname);
        param->maxDist = maxDist;
        param->maxVal = maxVal;
        this->calcIonChanParam(param);
    } catch(std::out_of_range) {
        qWarning("PvarsGrid: %s is not in pvars",varname.c_str());
    }
}
void PvarsGrid::setStartCells(string varname, set<pair<int,int>> startCells) {
    try {
        MIonChanParam* param = this->__pvars->at(varname);
        param->startCells = startCells;
        this->calcIonChanParam(param);
    } catch(std::out_of_range) {
        qWarning("PvarsGrid: %s is not in pvars",varname.c_str());
    }
}
void PvarsGrid::calcIonChanParams() {
    for(auto& pvar : *this->__pvars) {
        this->calcIonChanParam(pvar.second);
    }
}
void PvarsGrid::calcIonChanParam(MIonChanParam* param) {
    this->current = param->startCells;
    int i = 0;
    double val = 0;
    while(current.size() > 0 && i <= param->maxDist) {
        for(const pair<int,int>& e: this->current) {
            switch(param->dist) {
                case CellPvars::Distribution::none:
                    val = param->val[0]+param->val[1]*i;
                    break;
                case CellPvars::Distribution::normal: 
                    {
                        normal_distribution<double> distribution(param->val[0],param->val[1]);
                        val = distribution(generator);
                        break;
                    }
                case CellPvars::Distribution::lognormal:
                    {
                        lognormal_distribution<double> logdistribution(param->val[0], param->val[1]);
                        val = logdistribution(generator);
                        break;
                    }
            }
            if(val > param->maxVal) {
                val = param->maxVal;
            }
            param->cells[make_pair(e.first,e.second)] = val;
            this->visited.insert(e);
        }
        ++i;
        this->getNext();
    }
    this->current.clear();
    this->visited.clear();
}

void PvarsGrid::getNext() {
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
void PvarsGrid::add(pair<int,int> e, set<pair<int,int>>& next) {
    //check for out of bounds
    if(e.second<0||e.first<0||e.second>=this->grid->rowCount()||e.first>=this->grid->columnCount()) {
        return;
    }
    if(this->visited.count(e) == 0) {
        next.insert(e);
    }
}
