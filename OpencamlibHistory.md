## 2011 January ##

Work with calculating voronoi-diagrams, and their duals Delaunay triangulations.

Voronoi-diagram in Cyan, and Delaunay triangulation in red:

<a href='http://www.youtube.com/watch?feature=player_embedded&v=dXyyccLi1X8' target='_blank'><img src='http://img.youtube.com/vi/dXyyccLi1X8/0.jpg' width='425' height=344 /></a>

Voronoi-diagram for point-generators:

<a href='http://www.youtube.com/watch?feature=player_embedded&v=1Ra08OoKWXU' target='_blank'><img src='http://img.youtube.com/vi/1Ra08OoKWXU/0.jpg' width='425' height=344 /></a>

## 2010 November ##

Fixed a lot of memory leaks, see [issue30](https://code.google.com/p/opencamlib/issues/detail?id=30).

New experimental operation: Adaptive Waterline, see: http://www.anderswallin.net/2010/11/better-adaptive-waterline/

## 2010 October ##

experimenting with adaptive sampling for drop-cutter: http://www.anderswallin.net/2010/10/adaptive-sampling-drop-cutter/

more re-factoring and cleanup of both drop-cutter and push-cutter.

Push cutter for BullCutter with the aligned offset-ellipse. (ConeCutter is still missing/ToDo). See: http://www.anderswallin.net/2010/10/waterline-with-bullcutter/

## 2010 September ##

re-factoring of drop-cutter and push-cutter methods to handle most work in the base-class and only encapsulate minimal variability in the sub-classes. Push-cutter for the toroid is still Todo (this requires an aligned offset-ellipse solver).

Experimenting with a pure c++ shared library target (libocl.so). This does not require linking against boost-python.

## 2010 August ##

BallCutter now has the push-cutter methods, and can be used with Waterline.

some work on cutsim: http://www.youtube.com/watch?v=1DAvgLCj_RQ and http://www.anderswallin.net/2010/08/octree-with-marching-cubes/

more progress on waterline toolpaths: http://www.anderswallin.net/2010/08/the-weave-point-order-problem/

<img src='http://www.anderswallin.net/wp-content/uploads/2010/08/tux_waterlines-625x364.png'>

<img src='http://www.anderswallin.net/wp-content/uploads/2010/08/tux_offsets-509x625.png'>

<h2>2010 July</h2>

First tests with producing Waterline paths: <a href='http://www.anderswallin.net/2010/07/waterline-toolpaths-part-2/'>http://www.anderswallin.net/2010/07/waterline-toolpaths-part-2/</a>
<img src='http://www.anderswallin.net/wp-content/uploads/2010/07/tux_waterlines-625x369.png' width='625></img'>


Experimenting with the octree-based cutting-simulation: <a href='http://www.youtube.com/watch?v=N7SU0l2b1j4'>http://www.youtube.com/watch?v=N7SU0l2b1j4</a> and <a href='http://www.youtube.com/watch?v=9sctWgd_5rs'>http://www.youtube.com/watch?v=9sctWgd_5rs</a>

<img src='http://www.anderswallin.net/wp-content/uploads/2010/07/depth_10.png' width='625></img'>


The CLPoint class was introduced. This changes how the low level drop-cutter methods are called.<br>
<br>
A line-filter was added which detects collinear (to within tolerance) points in a list of CLPoints and removes the redundant ones. See blog post: <a href='http://www.anderswallin.net/2010/07/line-filter-for-opencamlib/'>http://www.anderswallin.net/2010/07/line-filter-for-opencamlib/</a>

<h2>2010 May</h2>

STLSurf.getBounds() now returns the bounding-box values for an STL surface.<br>
<br>
ocl.revision() now returns the SVN revision<br>
<br>
cutter.offsetCutter(double d) method which returns a new cutter offset outward by the amount d. This is useful for creating semi-finish paths with a stock-to-leave value of d.<br>
<br>
Composite cutters that consist of two or more of the basic cutter shapes. See blog: <a href='http://www.anderswallin.net/2010/05/composite-cutters-for-ocl/'>http://www.anderswallin.net/2010/05/composite-cutters-for-ocl/</a>

ConeCutter added.<br>
<br>
<h2>2010 April</h2>

Experimental BatchDropCutter now makes use of OpenMP for multi-threading.<br>
<br>
First test-code of octree cutting simulation: <a href='http://www.youtube.com/watch?v=38LdfiMTVw0'>http://www.youtube.com/watch?v=38LdfiMTVw0</a>

First implementation of drop-cutter for Toroidal tools. <a href='http://www.youtube.com/watch?v=T0_lUYq2m-g'>http://www.youtube.com/watch?v=T0_lUYq2m-g</a>

Fixed some obvious bugs in drop-cutter for Cylindrical and Spherical tools.<br>
<br>
<h2>2010 March</h2>

Preliminary octree for cutting simulation <a href='http://www.youtube.com/watch?v=38LdfiMTVw0'>http://www.youtube.com/watch?v=38LdfiMTVw0</a>

Offset-ellipse solver for toroidal cutter edge-test <a href='http://www.youtube.com/watch?v=rD1h8S0lG7g'>http://www.youtube.com/watch?v=rD1h8S0lG7g</a>

DropCutter for Cylindrical and Spherical endmills now works.<br>
<br>
Seb and Dan join the project as committers!<br>
<br>
A "fixed" kd-tree search: <a href='http://www.youtube.com/watch?v=5eci8wSmeGM'>http://www.youtube.com/watch?v=5eci8wSmeGM</a>

Another drop-cutter demonstration: <a href='http://vimeo.com/10215501'>http://vimeo.com/10215501</a>   the kd-tree build or search function is broken somehow... video also here: <a href='http://www.youtube.com/watch?v=-0MPPY49Nwk'>http://www.youtube.com/watch?v=-0MPPY49Nwk</a>

Drop-cutter code with kd-tree search for triangles. <a href='http://vimeo.com/10215501'>http://vimeo.com/10215501</a>

<h2>2010 February</h2>

Loading STL-files and visualizing them works. Basic drop-cutter algorithm for cylindrical tools.<br>
<br>
<img src='http://www.anderswallin.net/wp-content/uploads/2010/04/tux.png' width='640></img'>