######################################################################
# Automatically generated by qmake (3.0) Sun Mar 27 16:04:50 2016
######################################################################
QT += widgets concurrent gui
TEMPLATE = app
TARGET = SmartImageComparison
INCLUDEPATH += .
INCLUDEPATH += ./include




# Input
HEADERS += mainwindow.h gpg.h \
    include/blockhash.h \
    include/const.h \
    include/gpg.h \
    include/log.h

FORMS += \
         mainwindow.ui

SOURCES += main.cpp mainwindow.cpp \
    src/blockhash.cpp \
    src/gpg.cpp

CONFIG   += c++11

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += Magick++
