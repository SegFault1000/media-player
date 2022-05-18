#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMap>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE







//QString millisecondsToTimeString(long);

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr, QStringList&& args = QStringList());
  ~MainWindow();

  void setProgressionStepAmount(int num);
  void SetPrecisionStepAmount(double num);

private slots:
  void on_actionOpen_triggered();
  void on_actionOpen_Settings_Window_triggered();  

private:
  Ui::MainWindow* ui;  
  int argc;
  QStringList consoleArgs;
//private variables:
  //video stuff:
  QMediaPlayer* mediaPlayer;
  QVideoWidget* videoWidget;
  //layout stuff:
  class QLabel* volumeLabel;
  QWidget* widget;
  QDockWidget* bottomDockWidget;
  class QGridLayout* gridLayout;
  class QGridLayout* upperGridLayout;
  //volume control  
  class QSlider* volumeSlider;
  int volumeStepAmount = 3;
  int volume = 15;  

  //progress control
  class CSlider* progressSlider;
  class QLabel* timeLabelCurrent;
  QLabel* timeLabelMax;
  QLabel* timeLabelPreview;

  int progressStepAmount = 1000;
  double frameStep = 16.6666667;


  bool sliderTickIsPressed = false;
  bool userPaused = false;
  bool isDraggingSlider = false;
  //timeMonitor
  QWidget* timeMonitorWidget;
  //Bookmarkmonitor
  class BookmarkWidget* bookmarkMonitorWidget;
  //StepWidget
  class StepWidget* stepWidget;

  //Fullscreen control
  bool fullScreen = false;
  bool lowerWidgetshidden = false;
  bool shouldDisplayTimeInTitle = false;
  QString videoTitle = "";



//private functions:
  inline void SetUpVideoWidgets();
  inline void SetUpVolumeControl();
  inline void SetUpProgressControl();
  void SetUpTimeLabels();

  bool ChangeVideo(const QString& filename, bool playAfterChange);
  void SetUpShortcuts();

  void AddShortcut(QKeySequence&& sequence, void(MainWindow::*)());
  template<class Func>
  void AddShortcut(QKeySequence&& sequence, Func func)
  {
    auto shortcut = new QShortcut(std::move(sequence), this);
    QObject::connect(shortcut, &QShortcut::activated, func);
  }

  void setFullScreen(bool);



  //extra:
  void SetFPS(double);
  int GetFPS();

//protected functions:
  void keyPressEvent(QKeyEvent*) override;
  void wheelEvent(QWheelEvent*) override;
public:
  //getters
  QVideoWidget* getVideoWidget();
  QMediaPlayer* getMediaPlayer();
 //public slots
public slots:
  void jumpToTimeFromUserInput();
  void mediaPlayerStepForward();
  void mediaPlayerStepBack();
  void mediaPlayerStepForwardPrecise();
  void mediaPlayerStepBackPrecise();
  void findClosestBookmarkBefore();
  void findClosestBookmarkAfter();
  void refreshVideoWidgetVisibility();
  void toggleFullScreen();
  void HideLowerWidgets();
  void ToggleVolumeSliderVisibility();
  void ToggleProgressSliderVisibility();
  void closeEvent(QCloseEvent*) override;
};
#endif // MAINWINDOW_H
