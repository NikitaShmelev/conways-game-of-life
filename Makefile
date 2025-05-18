CXX      := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic

SRCS := main.cpp GameOfLife.cpp
OBJS := $(SRCS:.cpp=.o)

conway: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) conway
