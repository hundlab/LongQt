#ifndef SIMVFILE_H
#define SIMVFILE_H

#include "simvar.h"

#include <QLineEdit>
namespace LQ = LongQt;

class SimvFile : public Simvar {
 public:
  explicit SimvFile(std::shared_ptr<LQ::Protocol> proto, std::string name,
                    QWidget* parent = 0);
  ~SimvFile();

 private:
  virtual void update_ui();
  void update_model(QString value);

  QLineEdit* widg;
};

#endif  // SimvFile_H
