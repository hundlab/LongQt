#include "heart_cell_sim.h"
//#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Simulation window;
    window.show();
    return a.exec();
}
