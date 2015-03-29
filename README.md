raytrace
=========
![trio.png](https://github.com/peregin55/raytrace/raw/master/images/trio.png) ![refract.png](https://github.com/peregin55/raytrace/raw/master/images/refract.png)
![planet.png](https://github.com/peregin55/raytrace/raw/master/images/planet.png)

## build 
After downloading the source, navigate to the root of the project and use the following command to build and test the ray tracer:

    make clean && make

In order to build the ray tracer you will need a C++ compiler with C++0x/C++11 support and a valid GLUT implementation installed.  Library paths can be configured under ```src/Makefile``` and global compiler options can be edited in the ```Makefile.inc``` located at the top of the project.

## execute
To execute the ray tracer:

    src/raytrace <scene-file.json>

Sample scene files can be found under ```src/scenes``` directory.

    src/raytrace src/scenes/trio.json
