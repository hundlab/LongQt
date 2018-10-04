#ifndef SETSECONDSTIM_H
#define SETSECONDSTIM_H

#include <QWidget>

namespace Ui {
class SetSecondStim;
}

class SetSecondStim : public QWidget
{
    Q_OBJECT

public:
    explicit SetSecondStim(QWidget *parent = 0);
    ~SetSecondStim();

private:
    Ui::SetSecondStim *ui;
};

#endif // SETSECONDSTIM_H
