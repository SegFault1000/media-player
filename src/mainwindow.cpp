#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QVideoWidget>
#include <QGridLayout>
#include <QProgressBar>
#include <cslider.h>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QStyle>
#include <QThread>
#include <QTimer>
#include <util.h>
#include <QFileDialog>
#include <QMessageBox>
#include <bookmarkwidget.h>
#include <timemonitorlayout.h>
#include <QInputDialog>
#include <QShortcut>
#include <stepwidget.h>
#include <settingswindow.h>
#include <QFileInfo>
#include "commandhandler.h"
#define VIDEO_WIDTH 7
#define VIDEO_HEIGHT 4
MainWindow::MainWindow(QWidget *parent, QStringList&& consoleArgs)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow),
  consoleArgs(std::move(consoleArgs))
{
  ui->setupUi(this);  
  argc = this->consoleArgs.length();

  this->setFocusPolicy(Qt::StrongFocus);
  //Prepare media player and video widget
  mediaPlayer = new QMediaPlayer(this);
  videoWidget = new QVideoWidget(this);
  TimeMonitorLayout::BindLabels(timeLabelCurrent, timeLabelPreview, timeLabelMax);
  SetUpVideoWidgets();
  widget = new QWidget(this);
  upperGridLayout = new QGridLayout(widget);

  widget->setLayout(upperGridLayout);
  this->setCentralWidget(widget);

  //TimeMonitorLayout::BindLabels(timeLabelCurrent, timeLabelPreview, timeLabelMax);
  //volumeBar = new QProgressBar(this);
  volumeSlider = new QSlider(this);
  volumeLabel = new QLabel(QStringLiteral("%0%").arg(QString::number(this->volume)), this);
  progressSlider = new CSlider(this, mediaPlayer);
  progressSlider->setPreviewLabel(timeLabelPreview);

  SetUpVolumeControl();
  SetUpProgressControl();

  upperGridLayout->addWidget(videoWidget, 0,0, VIDEO_HEIGHT, VIDEO_WIDTH);
  upperGridLayout->addWidget(volumeSlider, 8,0, 1, 1);
  upperGridLayout->addWidget(volumeLabel, 8,1, 1, 1);
  upperGridLayout->addWidget(progressSlider, 9,0);

  timeMonitorWidget = TimeMonitorLayout::GetTimeMonitorWidgetInstance();
  bookmarkMonitorWidget = new BookmarkWidget(this, this->mediaPlayer);
  stepWidget = new StepWidget(this);
  upperGridLayout->addWidget(timeMonitorWidget, 10,0);
  upperGridLayout->addWidget(bookmarkMonitorWidget,10,1);
  upperGridLayout->addWidget(stepWidget, 10, 2);
  SetUpShortcuts();
  if(argc > 2){
    CommandHandler(this, consoleArgs);
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setProgressionStepAmount(int num){
  this->progressStepAmount = num;
}

void MainWindow::SetPrecisionStepAmount(double num){
  this->frameStep = num;
}

void MainWindow::SetUpVideoWidgets()
{      
  connect(this->mediaPlayer, &QMediaPlayer::mediaStatusChanged, this->videoWidget, &QVideoWidget::show);
  this->mediaPlayer->setVideoOutput(this->videoWidget);  
  this->mediaPlayer->setVolume(0);
  this->videoWidget->setGeometry(100,100,300,400);
  if(argc == 2){
    ChangeVideo(consoleArgs.at(1), true);
  }

}


void MainWindow::SetUpVolumeControl()
{
  this->volumeSlider->setOrientation(Qt::Orientation::Horizontal);
  this->volumeSlider->setRange(0,100);
  QObject::connect(this->mediaPlayer, &QMediaPlayer::volumeChanged, this->volumeSlider, &QSlider::setValue);
  QObject::connect(this->volumeSlider, &QSlider::valueChanged, this->mediaPlayer, &QMediaPlayer::setVolume);
  QObject::connect(this->volumeSlider, &QSlider::valueChanged, [&]{
    this->volumeLabel->setText(QStringLiteral("%0%").arg(QString::number(this->volumeSlider->sliderPosition())));
  });
}

void MainWindow::SetUpProgressControl()
{
  this->progressSlider->setOrientation(Qt::Orientation::Horizontal);
  this->progressSlider->setFocusPolicy(Qt::FocusPolicy::NoFocus);

  static bool shouldTrackPosition = false;

  //QObject::connect(mediaPlayer, &QMediaPlayer::positionChanged)
  QObject::connect(this->mediaPlayer, &QMediaPlayer::positionChanged,
  [&]{
    if(shouldTrackPosition && !progressSlider->isHoldingSlider()){
      int pos = this->mediaPlayer->position();
      this->progressSlider->setValue(pos);
      this->timeLabelCurrent->setText(util::millisecondsToTimeString(pos));
      if(shouldDisplayTimeInTitle){
        TimeMonitorLayout* timeLayout = static_cast<TimeMonitorLayout*>(TimeMonitorLayout::GetTimeMonitorWidgetInstance()->layout());
        if(shouldDisplayTimeInTitle){
          this->setWindowTitle(QStringLiteral("%0 / %1 (%2 / %3) %4")
           .arg(timeLayout->currentTimeLabel->text())
           .arg(timeLayout->totalTimeLabel->text())
           .arg(timeLayout->currentTimeLabelMilliseconds->text())
           .arg(timeLayout->totalTimeLabelMilliseconds->text())
           .arg(videoTitle)
          );
        }
      }
    }
  });
  QObject::connect(this->mediaPlayer, &QMediaPlayer::durationChanged,
  [&]{
    shouldTrackPosition = true;
    this->progressSlider->setRange(0, this->mediaPlayer->duration());
    if(!this->progressSlider->isHoldingSlider())
      this->progressSlider->setValue(this->mediaPlayer->position());
    this->timeLabelMax->setText(util::millisecondsToTimeString(this->mediaPlayer->duration()));
  });
}

void MainWindow::SetUpTimeLabels(){
  const char* baseStyle = "QLabel{font-weight: bold;}";
  this->timeLabelCurrent->setStyleSheet(baseStyle);
  this->timeLabelPreview->setStyleSheet("QLabel{font-weight: bold; color: red;}");
  this->timeLabelMax->setStyleSheet(baseStyle);
}


bool MainWindow::ChangeVideo(const QString& filename, bool playAfterChange)
{  
  if(!QFileInfo::exists(filename))
    return false;
  mediaPlayer->setMedia(QUrl::fromLocalFile(filename));
  this->refreshVideoWidgetVisibility();
  if(playAfterChange && mediaPlayer->state() == QMediaPlayer::PausedState)
    mediaPlayer->play();

  return true;
}

void MainWindow::SetUpShortcuts()
{
  AddShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), &MainWindow::jumpToTimeFromUserInput);
  AddShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), &MainWindow::on_actionOpen_triggered);

  AddShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left), &MainWindow::findClosestBookmarkBefore);
  AddShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right), &MainWindow::findClosestBookmarkAfter);

  AddShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), &MainWindow::toggleFullScreen);
  //non-member
  AddShortcut(QKeySequence(Qt::Key_F), &MainWindow::HideLowerWidgets);
  AddShortcut(QKeySequence(Qt::Key_V), &MainWindow::ToggleVolumeSliderVisibility);
  AddShortcut(QKeySequence(Qt::Key_B), &MainWindow::ToggleProgressSliderVisibility);

  /*
  AddShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), [&]{

  });
*/

  /*
   KeyEvent is better for handling ALT key
   AddShortcut(QKeySequence(Qt::Key_Right), &MainWindow::mediaPlayerStepForward);
   AddShortcut(QKeySequence(Qt::Key_Left), &MainWindow::mediaPlayerStepBack);

   AddShortcut(QKeySequence(Qt::Key_Alt + Qt::Key_Right), &MainWindow::mediaPlayerStepForwardPrecise);
   AddShortcut(QKeySequence(Qt::Key_Alt + Qt::Key_Left), &MainWindow::mediaPlayerStepBackPrecise);
  */
}


void MainWindow::AddShortcut(QKeySequence&& sequence, void(MainWindow::*func)())
{
    auto shortcut = new QShortcut(std::move(sequence), this);
    QObject::connect(shortcut, &QShortcut::activated, this, func);
}

void MainWindow::setFullScreen(bool b)
{

}



QVideoWidget* MainWindow::getVideoWidget()
{
  return this->videoWidget;
}

QMediaPlayer* MainWindow::getMediaPlayer()
{
  return this->mediaPlayer;
}
void MainWindow::SetFPS(double _fps)
{
  frameStep = _fps / 1000.0;
}

int MainWindow::GetFPS()
{
  return 1000.0 / frameStep;
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent)
{
  int key = keyEvent->key();  
  static bool alt{};
  static int amount{};
  switch(key){
  case Qt::Key::Key_Right:    
    if(keyEvent->modifiers() == Qt::CTRL)
      goto keyPressDefaultLabel;

      alt = keyEvent->modifiers() == Qt::AltModifier;
      amount = alt ? stepWidget->getPreciseStep() : stepWidget->getProgressStep() * 1000;
      this->mediaPlayer->setPosition(this->mediaPlayer->position() + amount);
   break;
  case Qt::Key::Key_Left:
    if(keyEvent->modifiers() == Qt::CTRL)
      goto keyPressDefaultLabel;

    alt = keyEvent->modifiers() == Qt::AltModifier;
    amount = alt ? stepWidget->getPreciseStep() : stepWidget->getProgressStep() * 1000;
    this->mediaPlayer->setPosition(this->mediaPlayer->position() - amount);
  break;

  case Qt::Key::Key_Space:
    mediaPlayer->state() == QMediaPlayer::PausedState ?
          mediaPlayer->play() : mediaPlayer->pause();
  break;
  case Qt::Key::Key_M:
    bookmarkMonitorWidget->addBookmarkAtPosition();
  break;  
  default:
      keyPressDefaultLabel:
    QMainWindow::keyPressEvent(keyEvent);
  break;
  }


  /*
  case Qt::Key::Key_Up:
  mediaPlayer->setVolume(mediaPlayer->volume() + volumeStepAmount);
  break;
  case Qt::Key::Key_Down:
    mediaPlayer->setVolume(mediaPlayer->volume() - volumeStepAmount);
  break;
  */
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
  auto point = event->angleDelta();
  if(point.y() > 0)
    mediaPlayer->setPosition(mediaPlayer->position() + stepWidget->getPreciseStep());
  else if(point.y() < 0)
    mediaPlayer->setPosition(mediaPlayer->position() - stepWidget->getPreciseStep());
}

void MainWindow::jumpToTimeFromUserInput()
{
 QString time = QInputDialog::getText(this, "Insert time", "What time do you want to jump to?");
 if(time.isEmpty())
   return;
 int result = util::timeStringToMilliseconds(time);
 if(result == -1)
   return;
 mediaPlayer->setPosition(result);
}


void MainWindow::on_actionOpen_triggered()
{
  QString filename = QFileDialog::getOpenFileName(this, "Get video file", "", "Media Files (*.mp4 *.mpeg, *.wav, *.mp3, *.avi)");
  if(filename != ""){
    ChangeVideo(filename, true);
    videoTitle = std::move(filename);
  }
  else{
    util::SendMessage("Failed to select a file.");
  }
}

//SLOTS:
void MainWindow::mediaPlayerStepForward()
{
  mediaPlayer->setPosition(mediaPlayer->position() + stepWidget->getProgressStep() * 1000);
  stepWidget->updateAllValues();
}

void MainWindow::mediaPlayerStepBack()
{
  mediaPlayer->setPosition(mediaPlayer->position() - stepWidget->getProgressStep() * 1000);
  stepWidget->updateAllValues();
}

void MainWindow::mediaPlayerStepForwardPrecise()
{
  mediaPlayer->setPosition(mediaPlayer->position() + stepWidget->getPreciseStep());
  stepWidget->updateAllValues();
}

void MainWindow::mediaPlayerStepBackPrecise()
{
  mediaPlayer->setPosition(mediaPlayer->position() - stepWidget->getPreciseStep());
  stepWidget->updateAllValues();
}

void MainWindow::findClosestBookmarkBefore()
{  
  const QListWidget* listWidget = bookmarkMonitorWidget->getListWidget();
  int count = listWidget->count();
  if(count == 0)
    return;
  using Pair = QPair<AlteredQListWidgetItem*, qint64>;
  QVector<Pair> items;
  items.reserve(count);
  for(int i = 0; i < count; i++){
    auto item = static_cast<AlteredQListWidgetItem*>(listWidget->item(i));
    if(item->position() < this->mediaPlayer->position())
      items.push_back({item, util::abs(this->mediaPlayer->position() - item->position())});
  }  
  if(items.count() == 0)
    return;
  static auto comp = [](const Pair& a, const Pair& b){
    return a.second < b.second;
  };
  auto ptr = util::min_element(items.begin(), items.end(), comp)->first;
  this->mediaPlayer->setPosition(ptr->position());
}

void MainWindow::findClosestBookmarkAfter()
{
  const auto listWidget = bookmarkMonitorWidget->getListWidget();
  auto count = listWidget->count();
  if(count == 0)
    return;
  using Pair = QPair<AlteredQListWidgetItem*, qint64>;
  QVector<Pair> items;
  items.reserve(count);
  for(int i = 0; i < count; i++){
    auto item = static_cast<AlteredQListWidgetItem*>(listWidget->item(i));
    if(item->position() > this->mediaPlayer->position())
      items.push_back({item, util::abs(item->position() - this->mediaPlayer->position())});
  }
  if(items.count() == 0)
    return;
  static auto comp = [](const Pair& a, const Pair& b){
    return a.second < b.second;
  };
  auto ptr = util::min_element(items.begin(), items.end(), comp)->first;
  this->mediaPlayer->setPosition(ptr->position());
}

void MainWindow::refreshVideoWidgetVisibility()
{
  this->videoWidget->setVisible(false);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  this->videoWidget->setVisible(true);
}

void MainWindow::toggleFullScreen()
{
  this->setFullScreen(!this->fullScreen);
}

void MainWindow::HideLowerWidgets()
{
  lowerWidgetshidden = !lowerWidgetshidden;
  this->stepWidget->setVisible(lowerWidgetshidden);
  this->bookmarkMonitorWidget->setVisible(lowerWidgetshidden);
  this->timeMonitorWidget->setVisible(lowerWidgetshidden);
}

void MainWindow::ToggleVolumeSliderVisibility()
{
  this->volumeSlider->setVisible(!this->volumeSlider->isVisible());
  this->volumeLabel->setVisible(volumeSlider->isVisible());
}

void MainWindow::ToggleProgressSliderVisibility()
{
  this->progressSlider->setVisible(!this->progressSlider->isVisible());
}

void MainWindow::closeEvent(QCloseEvent*)
{
  QCoreApplication::exit(0);
}

void MainWindow::on_actionOpen_Settings_Window_triggered()
{
  static SettingsWindow settingsWindow(this);
  if(settingsWindow.isHidden()){
    settingsWindow.show();
  }
  settingsWindow.raise();
}
