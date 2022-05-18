#include <util.h>
#include <QString>
#include <QMessageBox>
#include <QLocale>
#include <cmath>
#include <QStringRef>

QString util::millisecondsToTimeString(long num){
  if(!num)
    return "00:00:00";

  int durationInSeconds = num / 1000;

  int seconds = durationInSeconds % 60;
  int minutes = (durationInSeconds / 60) % 60;

  if(!minutes)
    return QStringLiteral("00:00:%0 (%1)")
        .arg(QString::number(seconds).rightJustified(2, '0'))
        .arg(util::numberWithCommas(num));

  int hours = ((durationInSeconds / 60) / 60);
  if(!hours)
    return QStringLiteral("0:%0:%1 (%2)")
        .arg(QString::number(minutes).rightJustified(2, '0'))
        .arg(QString::number(seconds).rightJustified(2, '0'))
        .arg(util::numberWithCommas(num));

  return QStringLiteral("%0:%1:%2 (%3)")
      .arg(QString::number(hours).rightJustified(2, '0'))
      .arg(QString::number(minutes).rightJustified(2, '0'))
      .arg(QString::number(seconds).rightJustified(2, '0'))
      .arg(util::numberWithCommas(num));

}


void util::SendMessage(const QString& msg)
{
  static QMessageBox box;
  box.setText(msg);
  box.exec();
}

QString util::numberWithCommas(long num)
{
  static QLocale locale(QLocale::English);
  return locale.toString(num);

  //QString str;
  //int digits = countDigits(num);
  /*
  int digits = countDigits(num);
  int commaCount = digits / 3;
  int totalLength = digits + commaCount;
  int periodCounter = 0;
  QString str(totalLength, '0');
  for(int i = totalLength - 1; i >= 0; ++i, ++periodCounter){
    if(periodCounter == 3){
      str[i] = ',';
      periodCounter = 0;
      continue;
    }
    if(!num)
      break;
    str[i] = QChar(((int)num % 10) + '0');
    num /= 10;
  }
  return str;
  */
}

constexpr int util::countDigits(long n)
{
    return n == 0 ? 1 : log10(n) + 1;
}
int util::timeStringToMilliseconds(const QString& target){
  int first = target.indexOf(':');
  int last = target.indexOf(':', first + 1);
  int hours = QStringRef(&target, 0, first).toInt() * 3600;
  int minutes = QStringRef(&target, first + 1, last - first - 1).toInt() * 60;
  int seconds = QStringRef(&target, last + 1, target.size() - last - 1).toInt();
  return hours + minutes + seconds;
}
/*
int util::timeStringToMilliseconds(const QString& str)
{
  static int multipliers[] = {3600000,60000,1000};
  if(str.length() >= 7 && str.count(':') == 2){
    QString strArr[3];
    int offset = str.indexOf(':');
    strArr[0] = QString(str.data(), offset);
    int offset2 = str.lastIndexOf(':');
    strArr[1] = QString(str.data() + offset + 1, offset2 - offset - 1);
    strArr[2] = QString(str.data() + offset2 + 1);
    int total = 0;
    for(int i = 0; i < 3; i++)
      total += strArr[i].toInt() * multipliers[i];
    return total;
  }
  else{
    bool result;
    auto num = str.toInt(&result);
    return result ? num : -1;
  }
}*/


void util::SendMessage(int msg)
{
  static QMessageBox box;
  box.setText(QString::number(msg));
  box.exec();
}

void util::SendMessage(double msg)
{
  static QMessageBox box;
  box.setText(QString::number(msg));
  box.exec();
}

void util::SendMessage(bool b)
{
  static QMessageBox box;
  box.setText(b ? "true" : "false");
  box.exec();
}
