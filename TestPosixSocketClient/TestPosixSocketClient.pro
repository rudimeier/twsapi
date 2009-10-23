SOURCES += \
  ../PosixSocketClient/src/EClientSocketBase.cpp \
  ../PosixSocketClient/src/EPosixClientSocket.cpp \
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
  -Wno-switch \
  -Wno-unused-parameter