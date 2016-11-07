#ifndef LQGRIDEDITOR_H
#define LQGRIDEDITOR_H

#include <QMainWindow>

namespace Ui {
class LQGridEditor;
}

class LQGridEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit LQGridEditor(QWidget *parent = 0);
    ~LQGridEditor();

private:
    Ui::LQGridEditor *ui;
};

#endif // LQGRIDEDITOR_H
