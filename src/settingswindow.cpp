#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <mainwindow.h>
#include <QVideoWidget>
#include <QMediaPlayer>

SettingsWindow::SettingsWindow(QMainWindow* parent) :
  ui(new Ui::SettingsWindow)
{
  ui->setupUi(this);
  mainWindow = static_cast<MainWindow*>(parent);
  videoWidget = mainWindow->getVideoWidget();
  mediaPlayer = mainWindow->getMediaPlayer();
  ui->spinBoxVideoWidth->setValue(videoWidget->width());
  ui->spinBoxVideoHeight->setValue(videoWidget->height());
}

SettingsWindow::~SettingsWindow()
{
  delete ui;
}

void SettingsWindow::on_pushButton_clicked()
{
  auto geo = videoWidget->geometry();
  geo.setWidth(ui->spinBoxVideoWidth->value());
  geo.setHeight(ui->spinBoxVideoHeight->value());
  videoWidget->setGeometry(geo);
}
