#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsWindow(class QMainWindow* parent = nullptr);
  ~SettingsWindow();

private slots:
  void on_pushButton_clicked();

private:
  Ui::SettingsWindow *ui;
  class MainWindow* mainWindow;
  class QVideoWidget* videoWidget;
  class QMediaPlayer* mediaPlayer;
};

#endif // SETTINGSWINDOW_H
