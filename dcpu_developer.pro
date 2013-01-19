#-------------------------------------------------
#
# Project created by QtCreator 2012-04-12T20:21:53
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = dcpu_developer
TEMPLATE = app


SOURCES += main.cpp\
        dcpudeveloper.cpp \
    assembler.cpp \
    emulator.cpp \
    phrases.cpp \
    highlighter.cpp \
    editor.cpp \
    memoryviewer.cpp \
    glhelper.cpp \
    utils.cpp \
    hexspinbox.cpp \
    devices/lemviewer.cpp \
    devices/lem.cpp \
    devices/device.cpp \
    devices/clock.cpp \
    dialogs/findreplaceform.cpp \
    dialogs/findreplacedialog.cpp \
    dialogs/findform.cpp \
    dialogs/finddialog.cpp

HEADERS  += include/dcpudeveloper.h \
    include/emulator.h \
    include/phrases.h \
    include/highlighter.h \
    include/editor.h \
    include/phrases.h \
    include/highlighter.h \
    include/emulator.h \
    include/editor.h \
    include/dcpudeveloper.h \
    include/constants.h \
    include/assembler.h \
    include/memoryviewer.h \
    include/glhelper.h \
    include/utils.h \
    include/constants.h \
    include/lemviewer.h \
    include/lem.h \
    include/hexspinbox.h \
    include/device.h \
    include/parser.h \
    include/findreplaceform.h \
    include/findreplacedialog.h \
    include/findreplace_global.h \
    include/findform.h \
    include/finddialog.h \
    include/clock.h

INCLUDEPATH += include/

FORMS    += dcpudeveloper.ui \
    lem_monitor.ui \
    findreplaceform.ui \
    findreplacedialog.ui
