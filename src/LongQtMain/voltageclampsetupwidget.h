#ifndef VOLTAGECLAMPSETUPWIDGET_H
#define VOLTAGECLAMPSETUPWIDGET_H

#include <QWidget>

namespace Ui {
class VoltageClampSetupWidget;
}

class VoltageClampSetupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VoltageClampSetupWidget(QWidget *parent = 0);
    ~VoltageClampSetupWidget();

private:
    Ui::VoltageClampSetupWidget *ui;
};

#endif // VOLTAGECLAMPSETUPWIDGET_H
