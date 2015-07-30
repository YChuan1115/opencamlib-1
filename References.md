# Introduction #

This page will list academic papers related to CAM. Please add any papers you have to this list. The pdf papers are usually available by email if you ask kindly on the mailing-list.

Papers should be organized roughly by category below. It doesn't make sense to list the whole abstract here, it's usually available free on the web, but do provide some comments on what the paper contains or the main ideas.

When only the DOI of a paper is give you can reach it through http://dx.doi.org

This is a list of books related to computational geometry and CAM. Please add your description or comments for each book to this page!

# Websites #
  * http://www.personal.kent.edu/~rmuhamma/Compgeometry/compgeom.html
# Books. #
  * Okabe, Boots, Sugihara: "Spatial Tessellations - concepts and applications of voronoi diagrams", Wiley series in probability and statistics.
  * O'Rourke: Computational Geometry in C
  * deBerg: Computational geometry, algorithms and applications
  * Farin: [Curves and Surfaces for CAGD](http://www.farinhansford.com/books/cagd/)
  * Faux & Pratt: "Computational Geometry for Design and Manufacture"
  * Held, Martin, "On the Computational Geometry of Pocket Machining", Lecture Notes in Computer Science nr500 ( available as pdf through sciencedirect.com )
    * This describes the Voronoi-diagram based approach to 2D offsetting of line segments. Held has since published papers which extends these ideas to cover circular arcs. See: http://www.cosy.sbg.ac.at/~held/projects/pocket_off/pocket.html
  * Samet: "The Design and Analysis of Spatial Data Structures"
    * describes quadtrees and octrees
  * Sethian: "Level Set Methods and Fast Marching Methods"
  * Ghali: "Introduction to Geometric Computing"
  * Schneider, Eberly: "Geometric tools for computer graphics"
  * Vince: "Geometric Algebra: an algebraic system for computer games and animation"

# Reviews #
  * Dragomatz and Mann, "A Classified Bibliography of Literture on NC Tool Path Generation", CAD 29(3), 1997
    * http://www.cgl.uwaterloo.ca/~smann/Papers/survey2.pdf
    * A review of tool-path algorithms with around 200 references to pre-1997 work
# Drop-Cutter #
  * Yau, Chuang, Lee (2004) "Numerical control machining of triangulated sculptured surfaces in a stereo lithography format with a generalized cutter", International Journal of Production Research 42:13 2573-2598
    * link http://dx.doi.org/10.1080/00207540410001671651
    * summary: Describes the drop-cutter algorithm for a generalized 'APT-type' toroidal/tapered cutter. Also discusses a kd-tree for finding triangles under the cutter.
  * Hwang, Chang (1998) "Three-axis machining of compound surfaces using flat and filleted endmills", Computer Aided Design 30(8)  1998
    * link: http://dx.doi.org/10.1016/S0010-4485(98)00021-9
    * summary: describes the drop-cutter algorithm for cylindrical, spherical, and toroidal endmills.
  * Chuang, Chen, Yau (2002) "A Reverse Engineering Approach to Generating Interference-Free Tool Paths in Three-Axis Machining from Scanned Data of Physical Models", Int J Adv Manuf Technol (2002) 19:23–31
    * link: http://dx.doi.org/10.1007/PL00003965
    * summary: meshing of point-cloud data, drop-cutter math for toroidal endmills

# Z-slice or Waterline #
  * Chuang, Yau (2005) "A new approach to z-level contour machining of triangulated surface models using fillet endmills", Computer-Aided Design 37 (2005) 1039–1051
    * link  doi:10.1016/j.cad.2004.10.005
    * summary: z-slice algorithm for creating waterline paths

# Cutting Simulation #
A z-map is an array of vectors in the z-direction which are cut to length by the moving cutter. Vertical walls are difficult to represent, and undercuts are impossible to simulate.

An octree is an adaptive space division. The marching-cubes algorithm can be used to produce trianges from the octree if a distance-value is stored at each octree vertex.

## Octree based methods ##
  * Hong-Tzong Yau and Lee-Sen Tsou, "Efficient NC Simulation for Multi-Axis Solid Machining With a Universal APT Cutter" http://dx.doi.org/10.1115/1.3130231
  * Hong T. Yau ,  Lee S. Tsou ,  Yu C. Tong , "Adaptive NC Simulation for Multi-axis Solid Machining "
    * pdf link: http://www.cadanda.com/V2Nos1to4_11.pdf
  * Karunakaran
    * DOI 10.1007/s00170-009-2107-7

## Z-map based methods ##
  * Maeng et al. (2003) "A Z-map update method for linearly moving tools" Computer-Aided Design 35 (2003) 995–1009
    * link doi:10.1016/S0010-4485(02)00161-6
    * summary: math+algorithm for updating a z-map model when a toroidal tool is moved with linear moves
  * Maeng et al. (2004) "A fast NC simulation method for circularly moving tools in the Z-map environment", IEEE Proceedings of the Geometric Modeling and Processing 2004 (GMP’04)
    * summary: z-map update algorithm for circular/arc moves
  * Lee et al. 2002 : “Development of simulation system for machining process using enhanced Z-map model”
    * doi:10.1016/S0924-0136(02)00761-6
  * Jerard et al. 1990 (doi:?): “The use of surface points sets for generation, simulation, verification and automatic correction of NC machining programs”
  * Jerard et al. 1989 (doi:10.1007/BF01999101): “Approximate methods for simulation and verification of numerically controlled machining programs”
  * Drysdale et al. 1989 (doi:10.1007/BF01553878): “Discrete Simulation of NC Machining”
  * Chung et al. 1998 (doi:10.1016/S0010-4485(97)00033-X): “Modeling the surface swept by a generalized cutter for NC verification”

# Curve Offset and Pocketing #
  * Held, "VRONI: An engineering approach to the reliable and efficient computation of Voronoi diagrams of points and line segments" Computation Geometry 18 (2001)
    * http://dx.doi.org/10.1016/S0925-7721(01)00003-7
  * Kim, Lee, Yang "A new offset algorithm for closed 2D lines with Islands" Int J Adv Manuf Technol (2006) (DOI:10.1007/s00170-005-0013-1)
  * Park and Choi "Uncut free pocketing tool-paths generation using pair-wise offset algorithm" CAD 33 (2001)  http://dx.doi.org/10.1016/S0010-4485(00)00109-3
  * Park and Choi "Free-form die-cavity pocketing" Int J adv manuf technol 22 (2003) DOI:10.1007/s00170-003-1703-1
  * Liu et al "An offset algorithm for polyline curves" Computers in Industry (2007)
    * http://dx.doi.org/10.1016/j.compind.2006.06.002
  * Hansen and Arbab "An algorithm for generating NC tool paths for arbitrary shaped pockets with islands" ACM transactions on graphics (1992)
    * a classic paper which many others reference
  * Lai, Faddis, Sorem, "Incremental algorithms for ®nding the offset distance and minimum passage width in a pocket machining toolpath using the Voronoi technique", J Materials processing technology 100 (2000)
    * http://dx.doi.org/10.1016/S0924-0136(99)00425-2
# Adaptive Machining #
  * Flutter and Todd (2001) "A machining strategy for toolmaking", Computer Aided Design
    * link http://dx.doi.org/10.1016/S0010-4485(00)00136-6
    * summary: adaptive machining based on z-map stock model
  * Choi and Kim "Die-cavity pocketing via cutting simulation" CAD 29 (1997)
    * DOI: 10.1016/S0010-4485(97)00031-6