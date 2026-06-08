CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -O2

TARGET = RAII
SRC = RAII.cpp

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)