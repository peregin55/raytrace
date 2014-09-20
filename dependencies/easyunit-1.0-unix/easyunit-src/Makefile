include ../../../Makefile.inc

CPPFLAGS=-I .

# source and build paths
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all:	easyunit


$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
  # $@ is the name of this target (the location of the object file)
  # @< is the first prereq (the name of the cpp file to compile)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

easyunit: ${OBJECTS}
	$(AR) $(ARFLAGS) libeasyunit.a ${OBJECTS}
	mv libeasyunit.a ..

clean:
	rm -f *.o ../libeasyunit.a 
