#include "stepwidget.h"
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
StepWidget::StepWidget(QWidget* parent) :
  QWidget(parent)
{
  progressStepLabel = new QLabel(QString::number(progressStep), this);
  preciseStepLabel = new QLabel(QString::number(preciseStep), this);
  volumeStepLabel = new QLabel(QString::number(volumeStep), this);

  progressStepSpinBox = new QSpinBox(this);
  preciseStepSpinBox = new QDoubleSpinBox(this);
  volumeStepSpinBox = new QSpinBox(this);
  updateAllValues();

  gridLayout = new QGridLayout(this);
  gridLayout->addWidget(new QLabel("Step amount:", this),0,0);
  gridLayout->addWidget(progressStepSpinBox, 1, 0);

  gridLayout->addWidget(new QLabel("Step amount(precise):", this),2,0);
  gridLayout->addWidget(preciseStepSpinBox, 3, 0);

  gridLayout->addWidget(new QLabel("Volume step amount: ", this), 4,0);
  gridLayout->addWidget(volumeStepSpinBox, 5, 0);


  QObject::connect(this->progressStepSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [&]{
    this->progressStep = this->progressStepSpinBox->value();
  });
  QObject::connect(this->preciseStepSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&]{
    this->preciseStep = this->preciseStepSpinBox->value();
  });
  QObject::connect(this->volumeStepSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [&]{
    this->volumeStep = this->volumeStepSpinBox->value();
  });
}

int StepWidget::getProgressStep()
{
  return this->progressStep;
}

double StepWidget::getPreciseStep()
{
  return this->preciseStep;
}

int StepWidget::getVolumeStep()
{
  return this->volumeStep;
}

void StepWidget::updateAllValues()
{
  progressStepSpinBox->setValue(progressStep);
  preciseStepSpinBox->setValue(preciseStep);
  progressStepSpinBox->setValue(progressStep);
  volumeStepSpinBox->setValue(volumeStep);
}
