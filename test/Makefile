include ../Makefile.inc

# preprocessor (header) flags
CPPFLAGS      = -I../dependencies/jsoncpp-0.5.0 -I../dependencies/catch-1.0 -I../src
# linker flags
LDFLAGS       = -L../dependencies/jsoncpp-0.5.0
# linker libraries
LDLIBS        = -ljsoncpp -lm
# source and build paths
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
RAYTRACE_OBJS = $(filter-out ../src/raytrace.o, $(wildcard ../src/*.o))

all: test	

$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	# $@ is the name of this target (the location of the object file)
	# @< is the first prereq (the name of the cpp file to compile)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

raytrace_test: ${OBJECTS} ${RAYTRACE_OBJS}
	$(CXX) $(LDFLAGS) ${OBJECTS} ${RAYTRACE_OBJS} $(LDLIBS) -o $@

test: raytrace_test
	./raytrace_test || (echo "test failed $$?"; exit 1)

clean:
	rm -f *.o raytrace_test
