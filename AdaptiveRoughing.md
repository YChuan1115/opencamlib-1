This is a rather un-organized collection of ideas related to adaptive roughing. More to follow as the ideas develop.

# Introduction #

The idea with adaptive roughing is to calculate the tool path using a simulation. That way you can control how much material is moved at all times, if we are using a conventional cut or a climb cut, how sharp corners we make etc.

All of this is good for tool life (roughly constant material removal rate), high-speed machining (no sharp corners where the control needs to fully stop the machine).


# Stock model #

Discrete vs. Continuous ? A discrete model approximates the stock with pixels or voxels. A continuous model would accurately model the surface of the stock at all times.

A discrete model could be made more accurate by adaptively increasing the pixel density in areas where it is needed. (compare to adaptive mesh generation in FEM?)

For 2D roughing the simplest stock model is a bitmap. Stock pixels are marked 1, cleared pixels are marked 0, and fixtures and other no-no's could be marked with a third value. To make the model more accurate the actual stock and part boundaries could be represented with line segments(+arcs?) overlaid on the bitmap.

How to calculate the pixel size? (relative to part-model? relative to cutter size?)

For 2.5D or 3D

4+D machining requires better modelling of the z-direction. This can be done either with a depth-pixel type of model.

# Cutter movement control #
contact angle or material removal rate (MRR) window (min-max). This allows cutting less on 'thin walls' of part to achieve smoother paths. Conventional/Climb mode.

'Push-cutter' with adaptive step length. Push cutter until these conditions:
  * MRR limit
  * part/stock/fixture boundary

Steering:
  * dynamics (no sharp corners)
  * first calculate cuts, then rapids?
  * rapids require a path finding algorithm that finds a path with no cutting and positions the tool at the correct place (+velocity) for next cut.

Start point selection: spiral-in, drill-hole

List of preliminary ideas:
  * chase curves? could these be used to guide the cutting?
  * genetic algorithms for motion control?