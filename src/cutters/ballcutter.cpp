/*  $Id$ 
 * 
 *  Copyright 2010 Anders Wallin (anders.e.e.wallin "at" gmail.com)
 *  
 *  This file is part of OpenCAMlib.
 *
 *  OpenCAMlib is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OpenCAMlib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with OpenCAMlib.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/foreach.hpp>

#include "millingcutter.h"
#include "point.h"
#include "triangle.h"
#include "stlsurf.h"
#include "millingcutter.h"
#include "numeric.h"

namespace ocl
{

//********   CylCutter ********************** */
BallCutter::BallCutter()
{
    setDiameter(1.0);
}

BallCutter::BallCutter(const double d)
{
    setDiameter(d);
}

//********   drop-cutter methods ********************** */
int BallCutter::vertexDrop(CLPoint &cl, const Triangle &t) const
{
    // some math here: http://www.anderswallin.net/2007/06/drop-cutter-part-13-cutter-vs-vertex/
    
    int result = 0;
    CCPoint* cc_tmp;
    BOOST_FOREACH( const Point& p, t.p)
    {
        double q = cl.xyDistance(p); // distance in XY-plane from cl to p
        if (q<= radius) { // p is inside the cutter
            // q^2 + h2^2 = r^2
            // h2 = sqrt( r^2 - q^2 )
            // h1 = r - h2
            // cutter_tip = p.z - h1
            double h1 = radius - sqrt( square(radius) - square(q) );
            cc_tmp = new CCPoint(p);
            if (cl.liftZ(p.z - h1) ) { // we need to lift the cutter
                cc_tmp->type = VERTEX;
                cl.cc = cc_tmp;
                result = 1;
            } else {
                delete cc_tmp;
            }
        } 
    }
    return result;
}

//********   facet ********************** */
int BallCutter::facetDrop(CLPoint &cl, const Triangle &t) const
{
    // Drop cutter at (cl.x, cl.y) against facet of Triangle t

    Point normal; // facet surface normal
    
    if ( isZero_tol( t.n->z ) )  {// vertical surface
        return -1;  //can't drop against vertical surface
    } else if (t.n->z < 0) {  // normal is pointing down
        normal = -1* (*t.n); // flip normal
    } else {
        normal = *t.n;
    }   
    
    assert( isPositive( normal.z ) );
    
    if ( (isZero_tol(normal.x)) && (isZero_tol(normal.y)) ) { // horizontal plane special case
        // so any vertex is at the correct height
        CCPoint* cc_tmp = new CCPoint();
        cc_tmp->x = cl.x;
        cc_tmp->y = cl.y;
        cc_tmp->z = t.p[0].z;
        if (cc_tmp->isInside(t)) { // assuming cc-point is on the axis of the cutter...       
            if ( cl.liftZ(cc_tmp->z) ) {
                cc_tmp->type = FACET;
                cl.cc = cc_tmp;
                return 1;
            } else {
                delete cc_tmp;
            }
        } else { // not inside facet
            delete cc_tmp;
            return 0;
        }
    } // end horizontal plane case.
    
    
    // define plane containing facet
    // a*x + b*y + c*z + d = 0, so
    // d = -a*x - b*y - c*z, where
    // (a,b,c) = surface normal
    double a = normal.x;
    double b = normal.y;
    double c = normal.z;
    //double d = - a * t.p[0].x - b * t.p[0].y - c * t.p[0].z;
    double d = - normal.dot(t.p[0]);
        
    normal.normalize(); // make length of normal == 1.0
    
    // define the radiusvector which points from the 
    // ball-center to the cc-point.
    Point radiusvector = -radius*normal;
    
    // find the xy-coordinates of the cc-point
    CCPoint* cc_tmp = new CCPoint(); 
    *cc_tmp = cl + radiusvector;
    
    // find the z-coordinate of the cc-point.
    // it lies in the plane.
    cc_tmp->z = (1.0/c)*(-d-a*cc_tmp->x-b*cc_tmp->y); // NOTE: potential for divide-by-zero (?!)
    cc_tmp->type = FACET;
    
    // now find the z-coordinate of the cl-point
    double tip_z = cc_tmp->z - radiusvector.z - radius;
        
    if (cc_tmp->isInside(t)) { // NOTE: cc.z is ignored in isInside()       
        if ( cl.liftZ(tip_z) ) {
            cl.cc = cc_tmp;
            return 1;
        } else {
            delete cc_tmp;
        }
    } else {
        delete cc_tmp;
        return 0;
    }
    
    
    return 0; // we never get here (?)
}


//********   edge **************************************************** */
int BallCutter::edgeDrop(CLPoint &cl, const Triangle &t) const
{
    // Drop cutter at (p.x, p.y) against edges of Triangle t
    // strategy:
    // 1) calculate distance to infinite line
    // 2) calculate intersection points w. cutter
    // 3) pick the higher intersection point and test if it is in the edge
    
    int result = 0;
    
    for (int n=0;n<3;n++) { // loop through all three edges
    
        // 1) distance from point to line in xy plane
        int start=n;      // index of the start-point of the edge
        int end=(n+1)%3;  // index of the end-point of the edge
        //std::cout << "testing edge " << start << " to " << end << "\n";
        Point p1 = t.p[start];
        Point p2 = t.p[end];
        
        // check that there is an edge in the xy-plane
        // can't drop against vertical edges!
        if ( !isZero_tol( p1.x - p2.x) || !isZero_tol( p1.y - p2.y) ) {
        
            //std::cout << "Points " << p1 << " to " << p2 << "\n";
            double d = cl.xyDistanceToLine(p1, p2);
            assert( d >= 0.0 );
                
            if (d<=radius) { // potential hit
            
                // the plane of the line will slice the spherical cutter at
                // a distance d from the center of the cutter
                // here the radius of the circular section is
                double s = sqrt( square(radius) - square(d) );
                    
                // the center-point of this circle, in the xy plane lies at
                Point sc = cl.xyClosestPoint( p1, p2 );   
                
                Point v = p2 - p1;
                v.z=0;
                v.xyNormalize();
                
                double p2u = (p2-sc).dot(v); // u-coord of p2 in plane coordinates.
                double p1u = (p1-sc).dot(v);

                // in the vertical plane of the line:
                // (du,dz) points in the direction of the line
                // so (dz, -du) is a normal to the line                
                double dz = p2.z - p1.z;  
                double du = p2u-p1u;             
                Point normal = Point (dz, -du, 0);
                normal.xyNormalize();
                if (normal.y < 0) { // flip normal so it points upward
                    normal = -1*normal;
                } 

                double cl_z;
                CCPoint* cc_tmp = new CCPoint();
                
                if ( isZero_tol(normal.y) ) { // this is the special case where the edge is horizontal
                    *cc_tmp = sc;
                    // locate cc_tmp on the edge
                    // edge = p1 + t*(p2-p1)
                    if ( fabs(p2.x - p1.x) > fabs(p2.y - p1.y) ) { // use x-coord
                        // x = p1.x + t*(p2.x-p1.x)
                        // t = (x - p1.x) / (p2.x -p1.x)
                        // z = p1 + t*(p2-p1)
                        double t = (cc_tmp->x - p1.x) / (p2.x - p1.x);
                        cc_tmp->z = p1.z + t*(p2.z-p1.z);
                    } else {
                        // the y-coord is better for the above calculation
                        double t = (cc_tmp->y - p1.y) / (p2.y - p1.y);
                        cc_tmp->z = p1.z + t*(p2.z-p1.z); 
                    }
                    
                    cl_z = cc_tmp->z + s - radius;
                    
                } else {
                    // this is the general case
                    // now normal should point up
                    if (  !isPositive(normal.y)) {
                        std::cout << "ballcutter.cpp edgeDrop() normal.y=" << normal.y << " !!\n";
                        assert( isPositive(normal.y) );
                    }
                
                    Point start2sc = sc - p1;
                    //double sc_u = start2sc.dot( start2sc_dir  ); // horiz distance from startpoint to sc
                    
                    double cc_u = - s * normal.x; // horiz dist of cc-point in plane-cordinates
                    
                    *cc_tmp = sc + cc_u*v; // located in the XY-plane
                    
                    // now locate z-coord of cc_tmp on edge
                    double t;
                    if ( fabs(p2.x-p1.x) > fabs(p2.y-p1.y) ) {
                        t = (cc_tmp->x - p1.x) / (p2.x-p1.x);
                    } else {
                        t = (cc_tmp->y - p1.y) / (p2.y-p1.y);
                    }
                    cc_tmp->z = p1.z + t*(p2.z-p1.z);
                    
                    cl_z = cc_tmp->z + s*normal.y - radius;
                } // end non-horizontal case
                
                // test if cc-point is in edge
                if ( cc_tmp->isInsidePoints( p1, p2 ) ) {
                    if (cl.liftZ(cl_z)) {
                        cc_tmp->type = EDGE;
                        cl.cc = cc_tmp;
                        result = 1;
                    }
                }
                
            }// end if(potential hit)

        }// end if(vertical edge)
        
    } // end loop through all edges
        
    return result;
}

//******************* PUSH-CUTTER **************************************

bool BallCutter::vertexPush(const Fiber& f, Interval& i, const Triangle& t) const {
    // same test as for cylcutter, but with an adjusted eff_radius
    bool result = false;
    BOOST_FOREACH( const Point& p, t.p) {
        if ( ( p.z >= f.p1.z ) && ( p.z <= (f.p1.z+getLength()) ) ) { // p.z is within cutter
            Point pq = p.xyClosestPoint(f.p1, f.p2); // closest point on fiber
            double q = (p-pq).xyNorm(); // distance in XY-plane from fiber to p
            double h = p.z - f.p1.z;
            assert( h>= 0.0);
            double eff_radius = radius; // default, shaft radius
            if (h< radius) // eff_radius is smaller if we hit the ball
                eff_radius = sqrt( square(radius) - square(radius-h) );
                    
            if ( q <= eff_radius ) { // we are going to hit the vertex p
                double ofs = sqrt( square(eff_radius) - square(q) ); // distance along fiber 
                Point start = pq - ofs*f.dir;
                Point stop  = pq + ofs*f.dir;
                CCPoint cc_tmp = CCPoint(p);
                cc_tmp.type = VERTEX;
                i.updateUpper( f.tval(stop) , cc_tmp );
                i.updateLower( f.tval(start) , cc_tmp );
                result = true;                
            }             
        }
    }
    return result;
}

bool BallCutter::facetPush(const Fiber& fib, Interval& i,  const Triangle& t) const {
    bool result = false;
    Point normal; // facet surface normal 
    if ( t.n->zParallel() ) { // normal points in z-dir   
        return result; //can't push against horizontal plane, stop here.
    }
    else if (t.n->z < 0) {  // normal is pointing down
        normal = -1* (*t.n); // flip normal
    } else {
        normal = *t.n;
    }
    // now we know the normal points upwards
    
    // find a point on the plane from which radius*normal lands on the fiber+radius*Point(0,0,1) 
    //
    //  (u,v) locates a point on the triangle facet    v0+ u*(v1-v0)+v*(v2-v0)    u,v in [0,1]
    //   t locates a point along the fiber:            p1 + t*(p2-p1)             t in [0,1]
    // 
    //   facet-point + radius * normal = fiber-point + radius*Point(0,0,1)
    //   =>
    //   v0+ u*(v1-v0)+v*(v2-v0) + radius*normal = p1 + t*(p2-p1) + radius*Point(0,0,1)
    //
    //   v0x + u*(v1x-v0x) + v*(v2x-v0x) + r*nx = p1x + t*(p2x-p1x)         p2x-p1x==0 for Y-fiber
    //   v0y + u*(v1y-v0y) + v*(v2y-v0y) + r*ny = p1y + t*(p2y-p1y)         p2y-p1y==0 for X-fiber
    //   v0z + u*(v1z-v0z) + v*(v2z-v0z) + r*nz = p1z + t*(p2z-p1z) + r    (p2z-p1z)==0 for XY-fibers!!
    //   X-fiber:
    //   v0x + u*(v1x-v0x) + v*(v2x-v0x) + r*nx = p1x + t*(p2x-p1x)         
    //   v0y + u*(v1y-v0y) + v*(v2y-v0y) + r*ny = p1y                    solve these  two for (u,v)
    //   v0z + u*(v1z-v0z) + v*(v2z-v0z) + r*nz = p1z + r                and substitute above for t
    //   or
    //   [ (v1y-v0y)    (v2y-v0y) ] [ u ] = [ -v0y - r*ny + p1y     ]
    //   [ (v1z-v0z)    (v2z-v0z) ] [ v ] = [ -v0z - r*nz + p1z + r ]
    //
    //   Y-fiber:
    //   v0x + u*(v1x-v0x) + v*(v2x-v0x) + r*nx = p1x                        p2x-p1x==0 for Y-fiber
    //   v0y + u*(v1y-v0y) + v*(v2y-v0y) + r*ny = p1y + t*(p2y-p1y)         
    //   v0z + u*(v1z-v0z) + v*(v2z-v0z) + r*nz = p1z +  r                   (p2z-p1z)==0 for XY-fibers!!
    //   or 
    //   [ (v1x-v0x)    (v2x-v0x) ] [ u ] = [ -v0x - r*nx + p1x     ]
    //   [ (v1z-v0z)    (v2z-v0z) ] [ v ] = [ -v0z - r*nz + p1z + r ]
    
    double a;
    double b;
    double c = t.p[1].z - t.p[0].z;
    double d = t.p[2].z - t.p[0].z;
    double e;
    double f = -t.p[0].z - radius*normal.z + fib.p1.z + radius;
    double u;
    double v;
    // a,b,e depend on the fiber:
    if ( fib.p1.y == fib.p2.y ) {
        a = t.p[1].y - t.p[0].y;
        b = t.p[2].y - t.p[0].y;
        e = -t.p[0].y - radius*normal.y + fib.p1.y;
        if (!two_by_two_solver(a,b,c,d,e,f,u,v))
            return result;
        CCPoint cc = t.p[0] + u*(t.p[1]-t.p[0]) + v*(t.p[2]-t.p[0]);
        cc.type = FACET;
        if ( ! cc.isInside( t ) ) 
            return result;
        // v0x + u*(v1x-v0x) + v*(v2x-v0x) + r*nx = p1x + t*(p2x-p1x) 
        // =>
        // t = 1/(p2x-p1x) * ( v0x + r*nx - p1x +  u*(v1x-v0x) + v*(v2x-v0x)       )
        assert( !isZero_tol( fib.p2.x - fib.p1.x )  );
        double tval = (1.0/( fib.p2.x - fib.p1.x )) * ( t.p[0].x + radius*normal.x - fib.p1.x + u*(t.p[1].x-t.p[0].x)+v*(t.p[2].x-t.p[0].x) );
        if ( tval < 0.0 || tval > 1.0  ) {
            std::cout << "BallCutter::facetPush() tval= " << tval << " error!?\n";
        } 
        assert( tval > 0.0 && tval < 1.0 );
        i.updateUpper( tval  , cc );
        i.updateLower( tval  , cc );
        result = true;
        
    } else if (fib.p1.x == fib.p2.x) {
        a = t.p[1].x - t.p[0].x;
        b = t.p[2].x - t.p[0].x;
        e = -t.p[0].x - radius*normal.x + fib.p1.x;
        if (!two_by_two_solver(a,b,c,d,e,f,u,v))
            return result;
        CCPoint cc = t.p[0] + u*(t.p[1]-t.p[0]) + v*(t.p[2]-t.p[0]);
        cc.type = FACET;
        if ( ! cc.isInside( t ) ) 
            return result;
        assert( !isZero_tol( fib.p2.y - fib.p1.y )  );
        double tval = (1.0/( fib.p2.y - fib.p1.y )) * ( t.p[0].y + radius*normal.y - fib.p1.y + u*(t.p[1].y-t.p[0].y)+v*(t.p[2].y-t.p[0].y) );
        if ( tval < 0.0 || tval > 1.0  ) {
            std::cout << "BallCutter::facetPush() tval= " << tval << " error!?\n";
        } 
        assert( tval > 0.0 && tval < 1.0 );
        i.updateUpper( tval  , cc );
        i.updateLower( tval  , cc );
        result = true;    
    } else {
        assert(0);
    }
    return result;
}

bool BallCutter::edgePush(const Fiber& f, Interval& i,  const Triangle& t) const {
    bool result = false;
    // FIXME, nothing here yet.
    // assert(0);
    return result;
}

//**********************************************************************

/// offset of ball is a bigger ball
MillingCutter* BallCutter::offsetCutter(const double d) const {
    return  new BallCutter(diameter+2*d) ;
}

//******** string output ********************** */
std::string BallCutter::str() const
{
    std::ostringstream o;
    o << *this; 
    return o.str();
}

std::ostream& operator<<(std::ostream &stream, BallCutter c)
{
  stream << "BallCutter(d=" << c.diameter << ", radius=" << c.radius << ")";
  return stream;
}

} // end namespace
// end file ballcutter.cpp
