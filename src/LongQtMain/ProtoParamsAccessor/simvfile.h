#ifndef SIMVFILE_H
#define SIMVFILE_H

#include "simvar.h"

#include <QLineEdit>
namespace LQ = LongQt;

class SimvFile : public Simvar {
 public:
  explicit SimvFile(std::shared_ptr<LQ::Protocol> proto, std::string name,
                    QLineEdit* parent);
  ~SimvFile();

  void update_ui() override;
  void update_model(QString value);
};

#endif  // SimvFile_H
