#ifndef TIMEMONITORLAYOUT_H
#define TIMEMONITORLAYOUT_H

#include <QObject>
#include <QGridLayout>


class TimeMonitorLayout : public QGridLayout{

  TimeMonitorLayout(class QWidget* parent);
public:
  class QLabel* currentTimeLabel;
  QLabel* previewTimeLabel;
  QLabel* totalTimeLabel;

  QLabel* currentTimeLabelMilliseconds;
  QLabel* previewTimeLabelMilliseconds;
  QLabel* totalTimeLabelMilliseconds;
  static class QWidget* GetTimeMonitorWidgetInstance();
  static void BindLabels(QLabel*&, QLabel*&, QLabel*&);
  static inline TimeMonitorLayout* GetLayout();

};

#endif // TIMEMONITORLAYOUT_H
