#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include <QString>
#include <QMap>
class CommandHandler{
private:
  using Command = void(*)(class MainWindow*, class QTextStream&);
  QMap<QString, Command> map;
public:
  CommandHandler(MainWindow* window, QStringList&);
  void InitCommands();
  Command GetCommandFromArg(const QString& arg, const QStringList&);
};
#endif // COMMANDHANDLER_H
