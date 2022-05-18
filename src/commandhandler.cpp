#include "commandhandler.h"
#include "mainwindow.h"
#include "util.h"
#include <QTextStream>

CommandHandler::CommandHandler(MainWindow* window, QStringList& consoleArgs)
{
  InitCommands();

  QString str = "";
  str.reserve(100);
  for(int i = 1; i < consoleArgs.size(); ++i)
  {
    str += consoleArgs[i];
    str += " ";
  }
  QTextStream stream{&str};
  while(stream.status() == QTextStream::Ok)
  {
    QString arg;
    stream >> arg;
    if(stream.status() != QTextStream::Ok)
      break;
    auto it = map.find(arg);
    if(it != map.end())
    {
      auto func = it.value();
      func(window, stream);
    }
  }
}

void CommandHandler::InitCommands(){
  map["-path"] = [](MainWindow* w, QTextStream& stream){
    QString arg;
    stream >> arg;
    if(!arg.isEmpty())
      w->getMediaPlayer()->setMedia(QUrl::fromLocalFile(arg));
  };
  map["-volume"] = [](MainWindow* w, QTextStream& stream){
    QString arg;
    stream >> arg;
    if(!arg.isEmpty())
      w->getMediaPlayer()->setVolume(util::clamp(arg.toInt(), 0, 100));
  };
  map["-step"] = [](MainWindow* w, QTextStream& stream){
    QString arg;
    stream >> arg;
    w->setProgressionStepAmount(arg.toInt());
  };
  map["-precisestep"] = [](MainWindow* w, QTextStream& stream){
    QString arg;
    stream >> arg;
    w->SetPrecisionStepAmount(arg.toFloat());
  };
}


