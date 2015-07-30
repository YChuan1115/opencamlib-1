Some definitions:
  1. C(R,r) is a cutter with shaft radius R and corner radius r
  1. CL point = Cutter location point. The position of the cutter.
  1. CC point = Cutter contact point. Where the cutter touches the model.

# Introduction #
The basic idea is to drop down a cutter along the z-axis until it touches the triangulated model. This is done by testing the cutter against all triangles and then choosing the highest found CL point.

# Basic tests #
Each triangle has once facet, three vertices and three edges. so there are three tests to be performed.
  1. vertex test for the three vertices
  1. facet test. this usually determines a CC point with the plane containing the triangle and then performs a point-in-triangle test to see if the CC point is within the facet
  1. edge test for the three edges. this also is usually done first for an infinite line and then a check is made to see if the CC point is within the edge.

# Optimizations #

The general consensus seems to be that using sin() cos() or tan() is really bad for performance. For the flat endmill and the ball-mill these can be avoided. The edge test for the toroidal mill requires sin() atan() etc. Julian Todd might have an algorithm for this using the offset-ellipse case which avoids trigonometric functions.

Chuang2002 lists the following steps for their DropCutter
  1. start with the tool position (xe,ye)
  1. find the triangles under the cutter
  1. vertex test (update ze)
    1. if vertex in inner region, se ze=zp
    1. if vertex in outer region, do proper vertex test
  1. facet test (update ze)
    1. if all vertices lower than current CL point, skip to next triangle
    1. compute CC point with infinite plane.
    1. if CC point is in triangle, compute ze
  1. edge test (update ze)
    1. extend to infinite lines, if all lines do not intersect cutter-region, skip to next triangle
    1. compute CC point with infinite line. if outside edge, skip to next edge
    1. if inside edge, compute CL point and update ze

## Finding triangles that overlap with the tool ##
to find the triangles under the tool it probably makes sense to bucket the triangles into a 'checker-board' pattern.

another option described in Yau2004 is to construct a k-d-tree of all the triangles. a search for those triangles that are under the cutter can then be performed rapidly. Yau uses rectangular bounding boxes for both the cutter and each triangle.

# DropCutter References #
papers
  1. Chuang2002 http://dx.doi.org/10.1007/PL00003965  describes DropCutter for flat, filleted and ball-nose cutters. The edge test requires use of trigonometric functions(slow?)
  1. Hwang1998 http://dx.doi.org/10.1016/S0010-4485(98)00021-9  DropCutter for flat, filleted and ball-nose cutters. Edge test with iterative solution to some equations
  1. Yau2004 http://dx.doi.org/10.1080/00207540410001671651 DropCutter for a general APT-style cutter. This includes cutters with pointed ends and tapered cutters. See also the freesteel blog on [tapered-cutters](http://www.freesteel.co.uk/wpblog/2007/02/one-tapered-tool-diagram-coming-up/)
  1. Chuang2005 http://dx.doi.org/10.1016/j.cad.2004.10.005  this paper presents a Push-cutter idea for calculating z-level (waterline) paths.

Links to blog-posts by Anders W:
  1. vertex test http://www.anderswallin.net/2007/06/drop-cutter-part-13-cutter-vs-vertex/
  1. Facet test http://www.anderswallin.net/2007/06/drop-cutter-part-23-cutter-vs-facet/
  1. point in triangle http://www.anderswallin.net/2007/06/point-in-triangle-test/ point-in-triangle
  1. edge test http://www.anderswallin.net/2007/07/drop-cutter-33-edge-test/
  1. drop-cutter test http://www.anderswallin.net/2007/07/drop-cutter-might-work/