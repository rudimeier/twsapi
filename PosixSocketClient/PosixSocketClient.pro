TEMPLATE = lib

CONFIG -= \
 release \
 qt

CONFIG += \
 debug \
 staticlib

SOURCES += \
  src/EClientSocketBase.cpp \
  src/EPosixClientSocket.cpp

HEADERS += \
  src/EPosixClientSocket.h \
  src/EPosixClientSocketPlatform.h\
  ../Shared/EClientSocketBase.h \
  ../Shared/EClientSocketBaseImpl.h \
  ../Shared/TwsSocketClientErrors.h \
  ../Shared/EWrapper.h

DEFINES += \
  IB_USE_STD_STRING

INCLUDEPATH += \
  ../Shared

QMAKE_CXXFLAGS_WARN_ON += \
  -Wno-switch