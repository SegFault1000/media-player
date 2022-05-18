#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include <QObject>
#include <QWidget>
#include <QListWidgetItem>
#include <util.h>
class AlteredQListWidgetItem : public QListWidgetItem{
  int _position = 0;
  QString name = "";
public:
  template<class... Ts>
  AlteredQListWidgetItem(int _position, const QString& name = "", Ts&&... args) :
    QListWidgetItem(std::forward<Ts>(args)...),
    _position(_position)
  {
    static auto getNewName = [&]{
      static int uid = 0;
      return QStringLiteral("bookmark%0").arg(++uid);
    };
    this->name = name.isEmpty() ? getNewName() : name;
    this->setText(QStringLiteral("%0: [%1]")
                  .arg(util::millisecondsToTimeString(this->position()))
                  .arg(this->name));
  }
  int position() const;
  //bool operator<(const QListWidgetItem& item) const;
  void changeName();
};

class BookmarkWidget : public QWidget
{
  Q_OBJECT
private:
  class QListWidget* listWidget = nullptr;
  class QMediaPlayer* mediaPlayer = nullptr;
  class QLabel* distanceLabel = nullptr;
  class QMainWindow* mainWindow = nullptr;
public:
  BookmarkWidget(QMainWindow*, class QMediaPlayer*);
  int distanceBetweenSelectedBookmarks();
  QListWidget const* getListWidget();


public slots:
  void checkDistance();
  void setMediaPlayerProgressToBookmark(QListWidgetItem*);
  void removeSelectedBookmarks();
  void addBookmarkAtPosition();
  void showContextMenu(const QPoint&);
  void deselectTooManySelections();

};

#endif // BOOKMARKWIDGET_H
