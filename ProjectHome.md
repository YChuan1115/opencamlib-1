Since 2011 July the latest code is now on GitHub: https://github.com/aewallin/opencamlib
<b>This SVN repository is out of date!</b>

OpenCAMLib ("ocl") is a [GPL](http://www.gnu.org/copyleft/gpl.html) licensed C++ library with [Python](http://python.org/) bindings for creating toolpaths for cnc-machines. Current features fall into roughly three categories:
  * Cutter projection algorithms are used to project milling cutters axially ("drop-cutter") or radially ("push-cutter") in contact with a polygonal surface. Cylindrical, spherical, toroidal, and conical cutter shapes are supported. (ToDo: push-cutter for cone)
  * A Voronoi diagram algorithm is being developed for generation of 2D offsets. Currently only point generators work. Line-segment and arc-segments are on the ToDo list.
  * A Cutting simulation engine is developed to simulate milling and turning. This experimental code requires Qt 4.7.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=1DAvgLCj_RQ' target='_blank'><img src='http://img.youtube.com/vi/1DAvgLCj_RQ/0.jpg' width='425' height=344 /></a>

Join us on IRC channel #cam on irc.freenode.net

Opencamlib is written in C++ using the Boost libraries (http://www.boost.org/), and OpenMP (http://openmp.org/wp/) for multi-threading. Development is done on Ubuntu (10.04LTS, 10.10, and 11.04), but ocl has been built on Windows/Mac/Haiku/etc too. If you use/test/develop ocl on its own, it's nice to use VTK (http://vtk.org/) for visualization (VTK is not strictly required to build or use ocl). Opencamlib can be built as a pure c++ library as well as a python-extension using boost-python.

The first CAD-application or GUI to make use of ocl is HeeksCAD/HeeksCNC (http://heeks.net/). There are other GPL toolpath generating libraries: **libarea** http://code.google.com/p/libarea/ for producing 2D offset paths, and **libactp** http://code.google.com/p/libactp/ for adaptive clearing. See also http://os-cam.org/


The **drop-cutter** algorithm takes an (x,y) position of a cutter and drops down the cutter against an STL model. The **push-cutter** algorithm pushes the cutter, at constant Z-height, into contact with an STL model along the X or Y axis and can be used for 2D offsetting as well as waterline paths. The **cutsim** is based on an adaptive octree volume representation and can simulate arbitrary 5-axis cutting with an arbitrary cutter on a mill or lathe.

The basic cutter shapes available are cylindrical, spherical, toroidal, and conical. The basic shapes can be combined to make a general APT-type tool, as well as many more shapes.

Features:
  * Drop-cutter algorithm for finish and semi-finish operations on STL surfaces
    * Video: finish-pass on Tux http://www.youtube.com/watch?v=5eci8wSmeGM
    * Cylindrical, Spherical, Toroidal, and Cone cutters, as well as Composite-cutters consisting of two or more of the basic shapes. see http://www.anderswallin.net/2010/05/composite-cutters-for-ocl/
    * Uses a kd-tree to search for overlapping triangles, and OpenMP for ca, 2.5x speedup on a quad-core machine
    * offsetCutter() method for producing stock-to-leave semi-finish CL-points
  * Push-cutter algorithm for producing waterline paths
    * see: http://www.anderswallin.net/2010/07/waterline-toolpaths-part-2/
    * see: http://www.anderswallin.net/2010/08/the-weave-point-order-problem/
    * ToDo: ConeCutter
    * ToDo; use the algorithms and ideas from 3D waterlines to do 2D offsets
  * Octree cutting simulation
    * ToDo: build C/C++ shared library and test/develop cutsim further
    * ToDo: improve performance.
    * color triangles according to tool, variable simulation speed, start/stop/pause, material removal rate, tolerance checking/coloring
    * swept volumes for lathe operations

For a time-line history of development see OpencamlibHistory.