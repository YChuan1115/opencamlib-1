# Papers #

  * Sugihara and Iri, (1992) "construction of the voronoi diagram for one million generators in single-precision arithmetic" http://dx.doi.org/10.1109/5.163412
  * Imai (1996) "A Topology-Oriented Algorithm for the Voronoi Diagram of Polygons" http://www.cccg.ca/proceedings/1996/cccg1996_0019.pdf
  * Sugihara, Iri, Inagaki, Imai, (2000) "topology oriented implementation - an approach to robust geometric algorithms" http://dx.doi.org/10.1007/s004530010002
  * Held, (2001) "VRONI: an engineering approach to the reliable and efficient computation of Voronoi diagrams of points and line segments" http://dx.doi.org/10.1016/S0925-7721(01)00003-7



# Algorithm #

The Sugihara&Iri papers from 1992 and 1994 describe the topology-based approach for Voronoi diagrams of points.


Incremental algorithm for generating the VD G(l) when adding new vertex pl from the previous VD G(l-1)

  * **A1** select a subset T of vertices in G(l-1)
  * **A2** for every edge connecting a vertex in T to a vertex not in T, generate a new vertex on it.
  * **A3** Generate new edges connecting the new vertices (A2) so that the new edges form a cycle that encloses vertices T (and no other vertices)
  * **A4** remove vertices T and edges incident on them (the interior of the cycle is R(pl)

The set T fulfils:
  * **C1** T is nonempty
  * **C2** the sub-graph G(l-1, T) is a tree
  * **C3** for a primary cycle C the T(IN)-vertices are connected

To find the set T:

  * **B1** find the generator pi that is closest to pl and among the voronoi-points Qijk on the boundary of the voronoi region R(pi) find the one with the smallest H(pi,pj,pk,pl). Add this voronoi point to T
  * **B2** repeat until no more points can be added to T:
    * **B2.1** for each Qijk connected to by a voronoi-edge to T, add Qijk to T if H<0 and if the resulting T satisfies C2 and C3
    * to satisfy (C2) and (C3) check that for the new vertex v:
      * (C4) v is not adjacent to two or more IN vertices (would create cycle)
      * (C5) for an incident face containing v: v is adjacent to an IN vertex on this face

# the in-circle predicate #

Qijk is inside the new voronoi region R(pl) if Qijk is inside the circumscribed circle of pi, pj, pk.

This is true when the determinant H(pi,pj,pk,pl) < 0.



# the VD graph #

start with G(3) which has three vertices "far" away from the vertices to be added.

The initial VD has three edges towards the center (0,0) and three circular segments at 'infinity'.

  * voronoi-point state: "in" "out" "undecided"
    * "in" points are in T
    * "out" points are not in T
  * voronoi-region state: "incident" "nonincident"
    * incident regions have a boundary whose vertex is in T

T is a tree if
  * **C4** Qijk is not adjacent to two or more "in" voronoi points
T is connected if
  * **C5** for any "incident" voronoi region having Qijk on its boundary, Qijk is adjacent to an "in" voronoi point on this boundary
# Generators #
t is the offset parameter. k=+1 for enlarging circle(offset left of line), k=-1 for shrinking circle(offset right of line).
  * Line: a1 x + b1 y + c1 + k1 t = 0  and  a\*a + b\*b = 1
  * Circle: square(x(t) - xc1) + square(y(t)-yc1) = square([r1](https://code.google.com/p/opencamlib/source/detail?r=1)+k1\*t)
# Bisectors #
With point, line, and arc generators all voronoi-edges, aka. bisectors, are quadratic curves: lines(line/line), parabolas(circle/line), arcs, hyperbolas(circle/circle), ellipses(circle/circle).
From Held's book, a way of representing bisectors:

  * x = x1 - x2 - x3\*t +/- x4  sqrt( square(x5+x6\*t) - square(x7+x8\*t) )
  * y = y1 - y2 - y3\*t +/- y4  sqrt( square(y5+y6\*t) - square(y7+y8\*t) )
this requires storing 2\*eight = sixteen values for each bisector.

A more compact representation is possible by storing only four parameters (alfa1, alfa2, alfa3, alfa4), which are calculated from the parameters of the bisector generators.
## line/line ##
  * alfa1= (b1\*d2-b2\*d1)/delta
  * alfa2= (a2\*d1-a1\*d2)/delta
  * alfa3= b2-b1
  * alfa4= a1-a2
From these stored values the x1-x8 and y1-y8 are recovered:
  * x1 = alfa1, x3 = -alfa3, x2 = x4 = x5 = x6 = x7 = x8 = 0
  * y1 = alfa2, y3 = -alfa4, y2=y4=y5=y6=y7=y8 = 0
## circle/line ##
  * alfa1= a2
  * alfa2= b2
  * alfa3= a2\*xc1 + b2\*yc1+d2
  * alfa4= [r1](https://code.google.com/p/opencamlib/source/detail?r=1)
From these we recover:
  * x1 = xc1, x2 = alfa1\*alfa3, x3 = -alfa1, x3 = alfa2, x5 = alfa4, x6 = lambda1, x7 = alfa3, x8 = -1
  * y1 = yc1, y2 = alfa2\*alfa3, y3 = -alfa2, y4 = alfa1, y5 = alfa4, y6 = lambda1, y7 = alfa3, y8 = -1
## circle/circle ##
use definition: d= sqrt( square(xc1-xc2) + square(yc1-yc2) ) and lambda=-1 for CCW arc and +1 otherwise
  * alfa1= (xc2-xc1)/d
  * alfa2= (yc2-yc1)/d
  * alfa3= ([r2](https://code.google.com/p/opencamlib/source/detail?r=2)\*[r2](https://code.google.com/p/opencamlib/source/detail?r=2)-[r1](https://code.google.com/p/opencamlib/source/detail?r=1)\*[r1](https://code.google.com/p/opencamlib/source/detail?r=1)-d\*d)/2d
  * alfa4= (lambda2\*[r2](https://code.google.com/p/opencamlib/source/detail?r=2)-lambda1\*[r1](https://code.google.com/p/opencamlib/source/detail?r=1))/d
and we recover:
  * x1 = xc1, x2 = alfa1\*alfa3, x3 = alfa1\*alfa4, x4 = alfa2, x5 = [r1](https://code.google.com/p/opencamlib/source/detail?r=1), x6 = lambda1, x7 = alfa3, x8 = alfa4
  * y1 = yc1, y2 = alfa2\*alfa3, y3 = alfa2\*alfa4, y4 = alfa1, y5 = [r1](https://code.google.com/p/opencamlib/source/detail?r=1), y6 = lambda1, y7 = alfa3, y8 = alfa4