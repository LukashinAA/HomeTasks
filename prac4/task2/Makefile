CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
TARGET = matrix_test
SRCS = main.cpp matrix.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: matrix.h
matrix.o: matrix.h

test: all

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all test clean
