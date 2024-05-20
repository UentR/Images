.SUFFIXES: .o .cpp

CXX = g++
CXXFLAGS = -std=c++17 -Wall -pedantic -O3

OBJDIR = object

PROGS = Main

all: $(PROGS)

clean:
	rm -f $(PROGS) $(OBJDIR)/*.o

Main: object/main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@