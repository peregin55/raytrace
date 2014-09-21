all: test

raytrace: raytrace_deps
	$(MAKE) -C src

raytrace_deps:
	$(MAKE) -C dependencies/jsoncpp-0.5.0

test: raytrace
	$(MAKE) -C test

clean:
	$(MAKE) -C dependencies/jsoncpp-0.5.0 clean
	$(MAKE) -C src clean
	$(MAKE) -C test clean
