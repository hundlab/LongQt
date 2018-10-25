/*
 * Allows users to modify the conductivites in a grid based on a rule
 */
#ifndef CONDUCTIVITYEDITOR_H
#define CONDUCTIVITYEDITOR_H

#include <QWidget>
#include "gridModel.h"
#include <QTableView>
namespace LQ = LongQt;

namespace Ui {
class ConductivityEditor;
}

class ConductivityEditor : public QWidget
{
    Q_OBJECT

	//position of the edge
/*	enum Pos {
		top = 0,
		right = 1,
		bottom = 2,
		left = 3
	};*/
	//an edge to a cell
/*	struct Edge {
		int x;
		int y;
		Pos p;
		//multiple edges can be the same eg a top of one cell is the bottom of annother
		//this function leaves only non duplicate edges
		Edge minimize(const Edge e) const;
		bool operator <(Edge e) const;
		bool operator ==(Edge e) const;
	};
*/
public:
    explicit ConductivityEditor(QTableView* view, QWidget *parent = 0);
    ~ConductivityEditor();

private:
    Ui::ConductivityEditor *ui;
	GridModel* model = 0;
	QTableView* view = 0;
    std::set<std::pair<int,int>> current;
    std::set<std::pair<int,int>> visited;

	void setConductivities(double startVal, double incAmount, int maxDist, double maxVal);
	void getInitial();
	void getNext();
    void add(std::pair<int,int> e, std::set<std::pair<int,int>>& next);

    void setConductivity(std::pair<int,int> e, double val);


private slots:
	void on_applyButton_clicked();
};

#endif // CONDUCTIVITYEDITOR_H
