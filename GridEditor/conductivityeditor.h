#ifndef CONDUCTIVITYEDITOR_H
#define CONDUCTIVITYEDITOR_H

#include <QWidget>
#include "gridModel.h"
#include <QTableView>

namespace Ui {
class ConductivityEditor;
}

class ConductivityEditor : public QWidget
{
    Q_OBJECT

	enum Pos {
		top = 0,
		right = 1,
		bottom = 2,
		left = 3
	};
	struct Edge {
		int x;
		int y;
		Pos p;
		Edge minimize(const Edge e) const {
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
		bool operator <(Edge e) const {
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
		bool operator ==(Edge e) const {
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
	};

public:
    explicit ConductivityEditor(QTableView* view, QWidget *parent = 0);
    ~ConductivityEditor();

private:
    Ui::ConductivityEditor *ui;
	GridModel* model;
	QTableView* view;
	set<Edge> current;
	set<Edge> visited;

	void setConductivities(double startVal, double incAmount, int maxDist, double maxVal);
	void getInitial();
	void getNext();
	void add(Edge e, set<Edge>& next);
	void setConductivity(Edge e, double val);


private slots:
	void on_applyButton_clicked();
};

#endif // CONDUCTIVITYEDITOR_H
