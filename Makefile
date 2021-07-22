CXX=clang++
CXXFLAGS=-std=c++17
SERVER_SRC=$(wildcard Server/*.cpp)
SERVER_OBJ=$(SERVER_SRC:.cpp=.o)
CLIENT_SRC=$(wildcard Client/*.cpp)
CLIENT_OBJ=$(CLIENT_SRC:.cpp=.o)

all: dirs server client

.PHONY: server
server: $(SERVER_OBJ)
	$(CXX) -o bin/server $^

.PHONY: client
client: $(CLIENT_OBJ)
	$(CXX) -o bin/client $^

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf bin
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ)

dirs:
	@mkdir -p bin