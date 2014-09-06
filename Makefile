all:	raytrace


raytrace:
	$(MAKE) -C dependencies/jsoncpp-0.5.0
	$(MAKE) -C src

clean:
	$(MAKE) -C dependencies/jsoncpp-0.5.0 clean
	$(MAKE) -C src clean
