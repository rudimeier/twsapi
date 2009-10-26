TEMPLATE = app

CONFIG -= \
  release \
  qt

CONFIG += \
  debug

SOURCES += \
  PosixTestClient.cpp \
  Main.cpp

HEADERS += \
  PosixTestClient.h

DEFINES += \
  IB_USE_STD_STRING

INCLUDEPATH += \
  ../PosixSocketClient/src \
  ../Shared

QMAKE_CXXFLAGS_WARN_ON += \
  -Wno-unused-parameter

LIBS += \
  -L../PosixSocketClient \
  -libtws
