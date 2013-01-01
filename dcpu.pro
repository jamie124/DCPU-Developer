QT       += core gui opengl

SOURCES += \
    phrases.cpp \
    parser.cpp \
    memoryviewer.cpp \
    main.cpp \
    highlighter.cpp \
    glhelper.cpp \
    emulator.cpp \
    editor.cpp \
    dcpudeveloper.cpp \
    utils.cpp \
    assembler.cpp

HEADERS += \
    include/utils.h \
    include/phrases.h \
    include/parser.h \
    include/memoryviewer.h \
    include/highlighter.h \
    include/glhelper.h \
    include/emulator.h \
    include/editor.h \
    include/dcpudeveloper.h \
    include/constants.h \
    include/assembler.h

FORMS    += dcpudeveloper.ui
