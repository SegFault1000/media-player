#ifndef CSLIDER_H
#define CSLIDER_H

#include <QObject>
#include <QSlider>
#include <thread>


class CSlider : public QSlider
{
  Q_OBJECT
public:
  CSlider(QWidget*, class QMediaPlayer*);
  void mouseMoveEvent(QMouseEvent *ev) override;
  void mouseReleaseEvent(QMouseEvent *ev) override;
  void mousePressEvent(QMouseEvent *ev) override;
  bool isDraggingSlider();
  bool isHoldingSlider();
  void setPreviewLabel(class QLabel*);
  inline QString positionAsTimeString();

private:
  class QMediaPlayer* mediaPlayer = nullptr;
  QLabel* timeLabelPreview = nullptr;
  bool* userPaused = nullptr;  
  bool draggingSlider = false;
  bool holdingSlider = false;
};



struct CSliderDebugger : public QWidget{
  class QLabel* LBIsDragging;
  QLabel* LBIsHolding;
  //QLabel* LBHasReleased;
  CSlider* target = nullptr;
  class QGridLayout* gridLayout;
  class std::thread* thread = nullptr;
  bool threadIsRunning = true;
  CSliderDebugger(CSlider*);
  ~CSliderDebugger() override;

};
#endif // CSLIDER_H
