#-------------------------------------------------
#
# Project created by QtCreator 2012-04-12T20:21:53
#
#-------------------------------------------------

QT       += core gui

TARGET = dcpu_developer
TEMPLATE = app


SOURCES += main.cpp\
        dcpudeveloper.cpp \
    assembler.cpp \
    emulator.cpp \
    phrases.cpp \
    highlighter.cpp \
    editor.cpp

HEADERS  += dcpudeveloper.h \
    emulator.h \
    phrases.h \
    highlighter.h \
    editor.h \
    include/phrases.h \
    include/highlighter.h \
    include/emulator.h \
    include/editor.h \
    include/dcpudeveloper.h \
    include/constants.h \
    include/assembler.h

FORMS    += dcpudeveloper.ui
