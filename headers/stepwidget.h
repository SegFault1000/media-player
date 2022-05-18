#ifndef STEPWIDGET_H
#define STEPWIDGET_H

#include <QObject>
#include <QWidget>

class StepWidget : public QWidget
{
  Q_OBJECT
private:
  double preciseStep = 16.6666667;
  int progressStep = 1;
  int volumeStep = 5;
  class QGridLayout* gridLayout;
  class QLabel* progressStepLabel;
  QLabel* preciseStepLabel;
  QLabel* volumeStepLabel;
  class QDoubleSpinBox* preciseStepSpinBox;
  class QSpinBox* progressStepSpinBox;
  QSpinBox* volumeStepSpinBox;
public:
  StepWidget(QWidget* parent);
  int getProgressStep();
  double getPreciseStep();
  int getVolumeStep();
public slots:
  void updateAllValues();
};

#endif // STEPWIDGET_H
