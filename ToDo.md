If you're interested in CAM algorithms and would like to help with this project, here is a list of algorithms and ideas that need implementing!
Please join us on the discussion list or on IRC to explore what we do and where you can help: http://groups.google.com/group/opencamlib

# 1D algorithms #
Drilling tool paths, or a similar point-to-point based toolpath/process.
  * Travelling salesman problem (TSP) solver
    * 2-TSP BGL code metric\_tsp\_approx is now wrapped in opencamlib as TSPSolver. See http://www.anderswallin.net/2011/05/tsp/
    * Christofides http://en.wikipedia.org/wiki/Christofides_algorithm
      * The Christofides algorithm guarantees 1.5x optimal solutions. Requires a minimum cost perfect matching algorithm (Edmonds, or "blossom V" or similar).
    * Genetic algorithm based?
    * simulated annealing based?
  * modified TSP: asymmetric graph. This could be used as a general algorithm for optimizing 2D paths. You come into a 'city' at the beginning of a cut and exit the 'city' after the cut (at another point in space). Or maybe you could implement this cutting idea with two cities per cut. As you enter the first city there is a zero-cost edge in the graph which you must travel along.

# 2D algorithms #
  * Voronoi diagram: Line-segment generators, Arc generators
    * Sugihara Iri http://dx.doi.org/10.1109/5.163412
    * Sugihara Iri 1994: http://dx.doi.org/10.1142/S0218195994000124
    * Topology based approach: http://dx.doi.org/10.1007/3-540-46632-0_36 and http://dx.doi.org/10.1007/s004530010002
    * Imai (line generators) http://www.cccg.ca/proceedings/1996/cccg1996_0019.pdf
    * Held 2001: http://dx.doi.org/10.1016/S0925-7721(01)00003-7
    * Held 2009: http://dx.doi.org/10.1016/j.cad.2008.08.004
  * Arc-filter (find arcs with given tolerance from short line segments)
  * **done** Line-filter

## 2D pocket/clearing algorithms ##
once we have a pocket or contour (from the offset algorithm) we need to clear(rough) it.
  * zigzag (use a graph-based approach, see paper by Arkin)
  * spiral (simple circular spiral?)
  * offset
  * adaptive (see separate page AdaptiveRoughing)

# 3D algorithms #
  * Drop-Cutter
    * **done** Cyl, Ball, Bull, Cone cutters work.
    * Composite cutters. (do they work ok currently?)
    * The triangle-search interface could be made abstract and other search methods explored. We now use a kd-tree, but geometric hashing/binning may be faster.
  * Push-cutter/Waterline
    * most cutters work (Cyl, Ball, Bull)
    * work needed on ConeCutter
    * work needed on CompositeCutter
  * Cutter Location surfaces
    * 2D triangulating an arbitrary pocket (with islands)
    * adaptive refinement of triangulation/mesh
    * slicing/traversing the mesh to generate toolpaths
    * geodesics aka. constant step-over, see Kobbelt paper http://www-i8.informatik.rwth-aachen.de/uploads/media/bommes_07_VMV_01.pdf
    * classification of areas into steep/shallow based on normal direction. Apply a drop-cutter strategy on shallow areas, a waterline approach to steep areas.
    * const-scallop (requires estimating curvature of surface?)

# Cutting simulation #
  * Idea: Store stock model in an octree, use isosurface extraction to get and render surface triangles. Cutting moves modify the stock octree.
  * The general idea is explained in papers by Yau: http://www.cadanda.com/V2Nos1to4_11.pdf   and   http://dx.doi.org/10.1115/1.3130231
  * Uses OpenGL VBO for rendering. As explained in the "secret sauce" paper.
  * Add all the cutter types.
  * Color triangles based on what cutter is used.
  * display/log material removal rate (verify that an adaptive pocketing algorithm actually works)
  * fixtures and other no-go areas
  * integrate with a g-code parser (EMC2 ?)
  * Isosurface extraction:
    * Marching cubes (classic Lorensen Cline paper)
    * Extended Marching Cubes (Kobbelt http://portal.acm.org/citation.cfm?id=383265)
    * Dual contouring http://portal.acm.org/citation.cfm?id=566586

# Multi-threading + Benchmarking #
We are now using OpenMP, but if someone is keen on MPI or OpenCL, then those are interesting directions too.

A set of standard benchmarking tests + timing mechanisms would be useful for measuring improvements in algorithm speed. Compare benchmarks to other libraries (PyCAM, libarea, other 2D offset libraries (clipper?) etc)

Tests. Unit tests and larger machining tests are needed. Run these periodically to track improvement and discover new/old bugs.