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
  ../Shared/CommonDefs.h \
  ../Shared/Contract.h \
  ../Shared/EClient.h \
  ../Shared/EClientSocketBase.h \
  ../Shared/EClientSocketBaseImpl.h \
  ../Shared/EClientSocket.h \
  ../Shared/EWrapper.h \
  ../Shared/Execution.h \
  ../Shared/IBString.h \
  ../Shared/MySocket.h \
  ../Shared/Order.h \
  ../Shared/OrderState.h \
  ../Shared/ScannerSubscription.h \
  ../Shared/shared_ptr.h \
  ../Shared/TwsSocketClientErrors.h

DEFINES += \
  IB_USE_STD_STRING

INCLUDEPATH += \
  ../Shared

QMAKE_CXXFLAGS_WARN_ON += \
  -Wno-switch


TARGET=ibtws

# install stuff
INSTALLBASE = $$(HOME)/usr

target.path = $${INSTALLBASE}/lib64
headers.path = $${INSTALLBASE}/include/twsapi
headers.files = $${HEADERS}
headers.files -= \
  ../Shared/EClientSocket.h \
  ../Shared/EClientSocketBaseImpl.h \
  ../Shared/MySocket.h \
  ../Shared/TwsSocketClientErrors.h

INSTALLS = target headers
