include ../Makefile.inc

# preprocessor (header) flags
CPPFLAGS      = -I/opt/X11/include -I../dependencies/jsoncpp-0.5.0 -I../dependencies/easyunit-1.0-unix -I../src
# linker flags
LDFLAGS       = -L/opt/X11/lib -L../dependencies/jsoncpp-0.5.0 -L../dependencies/easyunit-1.0-unix
# linker libraries
LDLIBS        = $(GL_LIBS) $(XLIBS) -lm
GL_LIBS       = -lglut -lGLU -lGL
XLIBS         = -lXmu -lXext -lX11 -lXi -ljsoncpp -leasyunit
# source and build paths
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
OTHER_OBJS = $(filter-out ../src/raytrace.o, $(wildcard ../src/*.o))

all: test	

$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
  # $@ is the name of this target (the location of the object file)
  # @< is the first prereq (the name of the cpp file to compile)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

raytrace_test: ${OBJECTS} ${OTHER_OBJS}
	$(CXX) $(LDFLAGS) ${OBJECTS} ${OTHER_OBJS} $(LDLIBS) -o $@

test: raytrace_test
	./raytrace_test || (echo "test failed $$?"; exit 1)

clean:
	rm -f *.o raytrace_test
