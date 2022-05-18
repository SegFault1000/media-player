#include "cslider.h"
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QGridLayout>
#include <chrono>
#include <QLabel>
#include <util.h>

CSlider::CSlider(QWidget* parent, QMediaPlayer* mediaPlayer) :
  QSlider(parent),
  mediaPlayer(mediaPlayer) {}

void CSlider::mouseMoveEvent(QMouseEvent* ev)
{    
  draggingSlider = true;
  mediaPlayer->setPosition(this->sliderPosition());
  timeLabelPreview->setText(this->positionAsTimeString());
  QSlider::mouseMoveEvent(ev);
}
void CSlider::mouseReleaseEvent(QMouseEvent* ev)
{ 
  draggingSlider = false;
  holdingSlider = false;
  if(this->sliderPosition() != mediaPlayer->position())
    mediaPlayer->setPosition(this->sliderPosition());
  timeLabelPreview->setVisible(false);
  QSlider::mouseReleaseEvent(ev);
}



void CSlider::mousePressEvent ( QMouseEvent * event )
{
  if (event->button() == Qt::LeftButton)
  {
   //   if (orientation() == Qt::Vertical)
   //       setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height() ) ;
   //   else
      setValue(minimum() + ((maximum()-minimum()) * event->x()) / width() );
      timeLabelPreview->setVisible(true);
      timeLabelPreview->setText(this->positionAsTimeString());
      event->accept();
  }
  QSlider::mousePressEvent(event);
}

bool CSlider::isDraggingSlider()
{
  return draggingSlider;
}

bool CSlider::isHoldingSlider()
{
  return holdingSlider;
}

void CSlider::setPreviewLabel(QLabel* timeLabelPreview)
{
  this->timeLabelPreview = timeLabelPreview;
}

QString CSlider::positionAsTimeString()
{
  return util::millisecondsToTimeString(this->sliderPosition());
}

//CSliderDebuggerClass
CSliderDebugger::CSliderDebugger(CSlider* target) : QWidget(target)
{
  this->target = target;
  LBIsHolding = new QLabel(this);
  LBIsDragging = new QLabel(this);
  gridLayout = new QGridLayout(this);
  gridLayout->addWidget(LBIsHolding, 0, 0);
  gridLayout->addWidget(LBIsDragging, 1, 0);
  this->setLayout(gridLayout);
  static auto update = [&](int delayMs = 32){
    while(threadIsRunning){
      LBIsHolding->setText(QStringLiteral("Holding: %1").arg(this->target->isHoldingSlider()));
      LBIsDragging->setText(QStringLiteral("Dragging: %1").arg(this->target->isDraggingSlider()));
      std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
  };
  thread = new std::thread(update, 16);
}

CSliderDebugger::~CSliderDebugger()
{
  threadIsRunning = false;
  thread->join();
  delete thread;
}
