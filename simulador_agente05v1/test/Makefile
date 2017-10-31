CXX=g++
LIBRARY=-L/usr/local/lib 
INCLUDE=-I/usr/local/include/nlohmann  
CXXFLAGS=-Wall -g -std=c++17 -O3 -lcurl -ggdb -lm -pthread $(INCLUDE) -g $(LIBRARY)


OBJS=../obj/pthread_http_test.o  
TARGET=sim

$(TARGET): $(OBJS) 
	        $(CXX) -o $@ $^ $(CXXFLAGS) 

../obj/pthread_http_test.o:pthread_http_test.cc

../obj/%.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS) 
clean:
	${RM} $(TARGET)
	${RM} obj/*.o
	${RM} *~

all:
	make clean
	make

			
