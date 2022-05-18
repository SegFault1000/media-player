#include "TimeMonitorLayout.h"
#include <QLabel>
#include <QWidget>

static QLabel* createUnderlinedAndBoldLabel(const QString& text = "", QWidget* parent = nullptr){
  QLabel* label = new QLabel(text, parent);
  auto font = label->font();
  font.setUnderline(true);
  font.setBold(true);
  label->setFont(font);
  return label;
}
static QLabel* createUnderlinedLabel(const QString& text = "", QWidget* parent = nullptr){
  QLabel* label = new QLabel(text, parent);
  auto font = label->font();
  font.setUnderline(true);
  label->setFont(font);
  return label;
}

static QLabel* createColoredLabel(const QString& text, const QString& color, QWidget* parent = nullptr){
  QLabel* label = new QLabel(text, parent);
  label->setStyleSheet(QStringLiteral("QLabel{ color: %1; }").arg(color));
  return label;
}
TimeMonitorLayout::TimeMonitorLayout(QWidget* parent) : QGridLayout(parent)
{
  currentTimeLabel = new QLabel("00:00:00",parent);

  previewTimeLabel = createColoredLabel("00:00:00", "red", parent);
  previewTimeLabel->setHidden(true);

  totalTimeLabel = new QLabel("00:00:00", parent);

  this->addWidget(createUnderlinedAndBoldLabel("current: ", parent), 0,0,1,2);
  this->addWidget(currentTimeLabel, 1,0);
  this->addWidget(previewTimeLabel, 1,1);

  this->addWidget(createUnderlinedAndBoldLabel("Total: ", parent), 2,0,1,2);
  this->addWidget(totalTimeLabel, 3,0);
/*
  currentTimeLabelMilliseconds = createColoredLabel("0", "red", parent);
  previewTimeLabelMilliseconds = createColoredLabel("0", "red", parent);
  totalTimeLabelMilliseconds = createColoredLabel("0", "red", parent);

  this->addWidget(createUnderlinedAndBoldLabel("Seconds:", parent),0,0,1,2);
  this->addWidget(new QLabel("Current:", parent),1,0);
  this->addWidget(currentTimeLabel,2,0);
  this->addWidget(previewTimeLabel,2,1);
  this->addWidget(totalTimeLabel, 3, 0);

  this->addWidget(createUnderlinedAndBoldLabel("Milliseconds: ", parent),4,0,1,2);
  this->addWidget(new QLabel("current:", parent), 5,0);
  this->addWidget(currentTimeLabelMilliseconds, 6,0);
  this->addWidget(previewTimeLabelMilliseconds, 6,1);
  this->addWidget(totalTimeLabelMilliseconds, 7,0);
  */
}

QWidget* TimeMonitorLayout::GetTimeMonitorWidgetInstance()
{
  static QWidget* widget = []{
    static QWidget w;
    w.setLayout(new TimeMonitorLayout(&w));
    return &w;
  }();

  return widget;
}

void TimeMonitorLayout::BindLabels(QLabel*& current, QLabel*& preview, QLabel*& total)
{
  auto layout = TimeMonitorLayout::GetLayout();
  current = layout->currentTimeLabel;
  preview = layout->previewTimeLabel;
  total = layout->totalTimeLabel;
}

inline TimeMonitorLayout* TimeMonitorLayout::GetLayout()
{
  return static_cast<TimeMonitorLayout*>(TimeMonitorLayout::GetTimeMonitorWidgetInstance()->layout());
}
