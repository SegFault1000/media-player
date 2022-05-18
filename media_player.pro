QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/CMediaPlayer.cpp \
    src/bookmarkwidget.cpp \
    src/commandhandler.cpp \
    src/cslider.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingswindow.cpp \
    src/stepwidget.cpp \
    src/timemonitorlayout.cpp \
    src/timemonitorwidget.cpp \
    src/util.cpp

HEADERS += \
    headers/CMediaPlayer.h \
    headers/bookmarkwidget.h \
    headers/commandhandler.h \
    headers/cslider.h \
    headers/mainwindow.h \
    headers/settingswindow.h \
    headers/stepwidget.h \
    headers/timemonitorlayout.h \
    headers/timemonitorwidget.h \
    headers/util.h

FORMS += \
    mainwindow.ui \
    settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
