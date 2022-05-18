#include "mainwindow.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QRegularExpressionMatch>
//argc - 1


//argc - 2
//videoplayer null volume


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w(nullptr, QCoreApplication::arguments());
  w.show();
  return a.exec();
}
