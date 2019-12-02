QT += sql

# Local
HEADERS += \
    $$PWD/src/local/xdao.hpp \
    $$PWD/src/local/xdatabase.hpp \
    $$PWD/src/local/xfield.hpp \
    $$PWD/src/local/xquerybuilder.hpp \
    $$PWD/src/local/xtablemodel.hpp

SOURCES += \
    $$PWD/src/local/xdatabase.cpp \
    $$PWD/src/local/xfield.cpp \
    $$PWD/src/local/xquerybuilder.cpp

INCLUDEPATH += $$PWD/src/local


# Remote
HEADERS += \
    $$PWD/src/remote/xjsonmodel.hpp \
    $$PWD/src/remote/xkey.hpp \
    $$PWD/src/remote/xrestclient.hpp

SOURCES += \
    $$PWD/src/remote/xjsonmodel.cpp \
    $$PWD/src/remote/xkey.cpp \
    $$PWD/src/remote/xrestclient.cpp

INCLUDEPATH += $$PWD/src/remote


# Shared
HEADERS += \
    $$PWD/src/shared/xstringtemplates.hpp

SOURCES +=

INCLUDEPATH += $$PWD/src/shared
