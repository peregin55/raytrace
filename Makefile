all: test

raytrace: raytrace_deps
	$(MAKE) -C src

raytrace_deps:
	$(MAKE) -C dependencies/jsoncpp-0.5.0
	$(MAKE) -C dependencies/lodepng-20141130

test: raytrace
	$(MAKE) -C test

clean:
	$(MAKE) -C dependencies/jsoncpp-0.5.0 clean
	$(MAKE) -C dependencies/lodepng-20141130 clean
	$(MAKE) -C src clean
	$(MAKE) -C test clean
