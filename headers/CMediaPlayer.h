#ifndef CVIDEOWIDGET_H
#define CVIDEOWIDGET_H

#include <QObject>
#include <QMediaPlayer>

class CMediaPlayer : public QMediaPlayer
{
  Q_OBJECT
public:
  CMediaPlayer();
  qint64 positionInSeconds();

};

#endif // CVIDEOWIDGET_H
