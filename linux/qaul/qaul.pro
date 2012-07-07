#-------------------------------------------------
#
# Project created by QtCreator 2012-05-13T23:44:54
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = qaul
TEMPLATE = app


SOURCES += main.cpp\
        qaul.cpp

HEADERS  += qaul.h

FORMS    += qaul.ui

LIBS     += -L ../../libqaul -l qaul

OTHER_FILES += \
    olsrd_linux.conf \
    portfwd.conf \
    copy_files.sh

copyfiles.commands  = ../qaul/copy_files.sh
QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
