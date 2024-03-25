#Thabang  sambo
#23/03/2024
#make to handle the main compilation file and unitest compilation file
 

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

MAIN_SRCS = ConnectedComponent.cpp PGMimageProcessor.cpp findcomp.cpp
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
MAIN_EXEC = findcomp.exe

TEST_SRCS = ConnectedComponent.cpp PGMimageProcessor.cpp unitest.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_EXEC = unitest

.PHONY: all main tests clean

all: main tests

main: $(MAIN_EXEC)

tests: $(TEST_EXEC)

$(MAIN_EXEC): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJS) -o $(MAIN_EXEC)

$(TEST_EXEC): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $(TEST_EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(MAIN_OBJS) $(MAIN_EXEC) $(TEST_OBJS) $(TEST_EXEC) *.pgm

