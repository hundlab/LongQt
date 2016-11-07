#ifndef CONDUCTIVITYEDITOR_H
#define CONDUCTIVITYEDITOR_H

#include <QWidget>

namespace Ui {
class ConductivityEditor;
}

class ConductivityEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ConductivityEditor(QWidget *parent = 0);
    ~ConductivityEditor();

private:
    Ui::ConductivityEditor *ui;
};

#endif // CONDUCTIVITYEDITOR_H
