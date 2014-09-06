include ../../Makefile.inc

CPPFLAGS=-I .

# source and build paths
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all:	jsoncpp


$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
  # $@ is the name of this target (the location of the object file)
  # @< is the first prereq (the name of the cpp file to compile)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

jsoncpp: ${OBJECTS}
	$(AR) $(ARFLAGS) libjsoncpp.a ${OBJECTS}

clean:
	rm *.o libjsoncpp.a 
