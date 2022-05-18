#include "bookmarkwidget.h"
#include <QApplication>
#include <QMainWindow>
#include <Qmenu>
#include <QMediaPlayer>
#include <QInputDialog>
#include <QListWidget>
#include <QLabel>
#include <QGridLayout>
BookmarkWidget::BookmarkWidget(QMainWindow* parent, QMediaPlayer* mediaPlayer) : QWidget(parent), mediaPlayer(mediaPlayer), mainWindow(parent)
{

  this->setFocusPolicy(Qt::NoFocus);
  listWidget = new QListWidget(this);
  listWidget->setSelectionMode(QListWidget::MultiSelection);
  distanceLabel = new QLabel("0", this);
  QGridLayout* grid = new QGridLayout(this);


  grid->addWidget(distanceLabel, 0, 0);
  grid->addWidget(listWidget, 1, 0);

  connect(listWidget, &QListWidget::itemDoubleClicked, this, &BookmarkWidget::setMediaPlayerProgressToBookmark);

  connect(listWidget, &QListWidget::itemSelectionChanged, this, &BookmarkWidget::deselectTooManySelections);
  connect(listWidget, &QListWidget::itemSelectionChanged, this, &BookmarkWidget::checkDistance);

  listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(listWidget, &QListWidget::customContextMenuRequested, this, &BookmarkWidget::showContextMenu);
}

int BookmarkWidget::distanceBetweenSelectedBookmarks()
{
  QList<QListWidgetItem*> items = listWidget->selectedItems();
  if(items.length() != 2)
    return -1;
  auto item1 = static_cast<AlteredQListWidgetItem*>(items[0]);
  auto item2 = static_cast<AlteredQListWidgetItem*>(items[1]);
  return util::abs(item1->position() - item2->position());
}

const QListWidget* BookmarkWidget::getListWidget()
{
  return this->listWidget;
}



void BookmarkWidget::checkDistance()
{
  distanceLabel->setText(QString::number(this->distanceBetweenSelectedBookmarks()));
}


void BookmarkWidget::setMediaPlayerProgressToBookmark(QListWidgetItem* item)
{
  this->mediaPlayer->setPosition(static_cast<AlteredQListWidgetItem*>(item)->position());
}

void BookmarkWidget::removeSelectedBookmarks()
{
  for( auto item : listWidget->selectedItems())
    listWidget->removeItemWidget(item);
}

void BookmarkWidget::addBookmarkAtPosition()
{
  auto pos = mediaPlayer->position();
  for(int i = 0; i < listWidget->count(); i++)
    if(static_cast<AlteredQListWidgetItem*>(listWidget->item(i))->position() == pos)
      return;


  listWidget->addItem(new AlteredQListWidgetItem(pos));
}

void BookmarkWidget::showContextMenu(const QPoint& point)
{
  QPoint globalPos = listWidget->mapToGlobal(point);
  auto itemBeforeCast = listWidget->itemAt(point);
  if(!itemBeforeCast){
    util::SendMessage("It was null.");
    return;
  }
  auto item = static_cast<AlteredQListWidgetItem*>(itemBeforeCast);
  QMenu menu;
  menu.addAction("Rename", [&]{
    item->changeName();
  });
  menu.addAction("Delete", [&]{
    this->listWidget->removeItemWidget(item);
    delete item;
  });
  menu.addAction("Go to", [&]{
    mediaPlayer->setPosition(item->position());
  });
  menu.exec(globalPos);
}

void BookmarkWidget::deselectTooManySelections()
{
  QList<QListWidgetItem*> items = this->listWidget->selectedItems();
  if(items.length() > 2)
    for(QListWidgetItem* item : items)
      item->setSelected(false);
}



void AlteredQListWidgetItem::changeName()
{
  static QWidget w;
  QString newName = QInputDialog::getText(&w, "Change bookmark name", QStringLiteral("What do you want to change\n[%0]\nto?")
                                          .arg(this->name));
  if(newName.isEmpty())
    return;
  this->name = newName;
  this->setText(QStringLiteral("%0: [%1]")
                .arg(util::millisecondsToTimeString(this->position()))
                .arg(this->name));

}
int AlteredQListWidgetItem::position() const
{
  return _position;
}
