/*
 * for large sims it can take a while to load
 * this allows for someone to see the progress and select which parts should be
 * graphed
 *
 */
#ifndef LOADINGPROGRESS_H
#define LOADINGPROGRESS_H

#include <QCheckBox>
#include <QDialog>
#include <QFileInfoList>
#include <QMap>
#include <QSet>

namespace Ui {
class LoadingProgressDialog;
}

class LoadingProgressDialog : public QDialog {
  Q_OBJECT

 public:
  explicit LoadingProgressDialog(QSet<int> trials, QWidget *parent = nullptr);
  ~LoadingProgressDialog();
  QSet<int> getExcludes() const;

 protected slots:
  virtual void closeEvent(QCloseEvent *e);

 private:
  Ui::LoadingProgressDialog *ui;
  QMap<int, QCheckBox *> trialCheckboxes;
  QSet<int> excludedTrials;
  QSet<int> trials;
  void createOptions();
 private slots:
  void on_skipButton_clicked();
  void on_loadAll_clicked();
  void on_loadFreqButton_clicked();
};

#endif  // LOADINGPROGRESSDIALOG_H
