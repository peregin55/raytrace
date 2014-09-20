all:	test

raytrace: raytrace_dependencies
	$(MAKE) -C src

raytrace_dependencies:
	$(MAKE) -C dependencies/jsoncpp-0.5.0

test: raytrace test_dependencies
	$(MAKE) -C test

test_dependencies:
	$(MAKE) -C dependencies/easyunit-1.0-unix/easyunit-src

clean:
	$(MAKE) -C src clean
	$(MAKE) -C dependencies/jsoncpp-0.5.0 clean
	$(MAKE) -C test clean
	$(MAKE) -C dependencies/easyunit-1.0-unix/easyunit-src clean
