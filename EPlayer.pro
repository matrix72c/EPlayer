QT       += core gui
QT       += sql
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    addmusicdialog.cpp \
    addtolist.cpp \
    editlistdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    mtable.cpp \
    newdialog.cpp \
    searchdialog.cpp

HEADERS += \
    addmusicdialog.h \
    addtolist.h \
    editlistdialog.h \
    mainwindow.h \
    mtable.h \
    newdialog.h \
    searchdialog.h

FORMS += \
    addmusicdialog.ui \
    addtolist.ui \
    editlistdialog.ui \
    mainwindow.ui \
    newdialog.ui \
    searchdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

DISTFILES += \
    defaultQss.qss

INCLUDEPATH+=D:/FFmpeg/include

LIBS+=D:/FFmpeg/lib/libavcodec.dll.a\
      D:/FFmpeg/lib/libavdevice.dll.a\
      D:/FFmpeg/lib/libavfilter.dll.a\
      D:/FFmpeg/lib/libavformat.dll.a\
      D:/FFmpeg/lib/libavutil.dll.a\
      D:/FFmpeg/lib/libswresample.dll.a\
      D:/FFmpeg/lib/libswscale.dll.a\
      D:/FFmpeg/lib/libpostproc.dll.a
