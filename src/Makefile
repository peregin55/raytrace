include ../Makefile.inc

# preprocessor (header) flags
CPPFLAGS      = -I/opt/X11/include -I../dependencies/jsoncpp-0.5.0 -I../dependencies/lodepng-20141130
# linker flags
LDFLAGS       = -L/opt/X11/lib -L../dependencies/jsoncpp-0.5.0 -L../dependencies/lodepng-20141130

# linker libraries
GL_LIBS       = -lglut -lGLU -lGL
LDLIBS        = $(GL_LIBS) -ljsoncpp -llodepng -lm
# source and build paths
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all:	raytrace

$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	# $@ is the name of this target (the location of the object file)
	# $< is the first prereq (the name of the cpp file to compile)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

raytrace: ${OBJECTS}
	$(CXX) $(LDFLAGS) ${OBJECTS} $(LDLIBS) -o $@

clean:
	rm -f *.o raytrace 
