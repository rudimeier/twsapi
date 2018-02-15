CXX=g++
CXXFLAGS=-pthread -Wall -Wno-switch -std=c++11
ROOT_DIR=../../../source/cppclient
BASE_SRC_DIR=${ROOT_DIR}/client
INCLUDES=-I${BASE_SRC_DIR} -I${ROOT_DIR}
TARGET=TestCppClient

$(TARGET):
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(BASE_SRC_DIR)/*.cpp ./*.cpp -o$(TARGET)

clean:
	rm -f $(TARGET) *.o

