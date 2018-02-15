CXX=g++
CXXFLAGS=-pthread -Wall -Wno-switch -std=c++11 -Wfatal-errors
LIBFLAGS=-L/usr/lib -lssl -lcrypto
ROOT_DIR=../../../source/cppclient
BASE_SRC_DIR=${ROOT_DIR}/client
BASE_SRC_SSL_DIR=${ROOT_DIR}/ssl
OPENSSL_DIR=/usr/include/openssl
INCLUDES=-I${BASE_SRC_DIR} -I${ROOT_DIR} -I{OPENSSL_DIR}
TARGET=TestCppClientSSL

$(TARGET):
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(BASE_SRC_DIR)/*.cpp $(BASE_SRC_SSL_DIR)/*.cpp ./*.cpp -o$(TARGET) $(LIBFLAGS)

clean:
	rm -f $(TARGET) *.o

