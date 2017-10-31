CXX=g++
LIBRARY=-L/usr/local/lib
INCLUDE=-I/usr/local/include/nlohmann 
CXXFLAGS=-Wall -g -std=c++11 -O3 -lcurl -ggdb -lm -lpthread $(INCLUDE) $(LIBRARY)


OBJS=obj/main.o obj/curlhttp.o obj/agent.o obj/user.o obj/administrator.o obj/volunteer.o obj/factory.o obj/utils.o obj/emergency.o obj/mission.o
TARGET=simulador

$(TARGET): $(OBJS) 
	        $(CXX) -o $@ $^ $(CXXFLAGS) 

obj/main.o:src/main.cc
obj/curlhttp.o:src/curlhttp.cc include/curlhttp.h
obj/agent.o:src/agent.cc include/agent.h
obj/user.o:src/user.cc include/user.h
obj/administrator.o:src/administrator.cc include/administrator.h
obj/volunteer.o:src/volunteer.cc include/volunteer.h
obj/factory.o:src/factory.cc include/factory.h
obj/utils.o:src/utils.cc include/utils.h
obj/emergency.o:src/emergency.cc include/emergency.h
obj/mission.o:src/mission.cc include/mission.h

obj/%.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS) 
clean:
	${RM} $(TARGET)
	${RM} obj/*.o
	${RM} *~

all:
	make clean
	make
