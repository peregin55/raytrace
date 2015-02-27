include ../../Makefile.inc

CPPFLAGS=-I .

# source and build paths
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all:	lodepng


$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
  # $@ is the name of this target (the location of the object file)
  # $< is the first prereq (the name of the cpp file to compile)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

lodepng: ${OBJECTS}
	$(AR) $(ARFLAGS) liblodepng.a ${OBJECTS}

clean:
	rm -f *.o liblodepng.a
