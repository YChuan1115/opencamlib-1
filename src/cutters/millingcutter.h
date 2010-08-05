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

#ifndef MILLING_CUTTER_H
#define MILLING_CUTTER_H

#include <iostream>
#include <string>
#include <vector>

#include <boost/python.hpp>

#include "stlsurf.h"
#include "fiber.h"
#include "point.h"
#include "clpoint.h"
#include "ccpoint.h"
namespace ocl
{

class Triangle;
class STLSurf;

///
/// \brief MillingCutter is a base-class for all milling cutters
///
class MillingCutter {
    public:
        /// default constructor
        MillingCutter();
        virtual ~MillingCutter() {};
        
        /// set the diameter of the cutter to d
        void setDiameter(double d);
        /// return the diameter of the cutter
        double getDiameter() const;
        /// return the radius of the cutter
        double getRadius() const;
        /// set the length of the cutter to l
        void setLength(double l);
        /// return the length of the cutter
        double getLength() const;
        
        /// return a MillingCutter which is larger than *this by d
        virtual MillingCutter* offsetCutter(const double d) const;
        
        /// does the cutter bounding-box, positioned at cl, overlap with the bounding-box of Triangle t?
        /// works in the xy-plane 
        bool overlaps(Point &cl, const Triangle &t) const;
        
        /// drop cutter at (cl.x, cl.y) against the three vertices of Triangle t.
        /// needs to be redefined by a subclass
        virtual int vertexDrop(CLPoint &cl, const Triangle &t) const {return -1;};
        
        /// drop cutter at (cl.x, cl.y) against facet of Triangle t
        /// needs to be defined by a subclass
        virtual int facetDrop(CLPoint &cl, const Triangle &t) const {return -1;};
        
        /// drop cutter at (cl.x, cl.y) against the three edges of Triangle t
        /// needs to be defined by a subclass
        virtual int edgeDrop(CLPoint &cl, const Triangle &t) const {return -1;};
        
        /// drop the MillingCutter at Point cl down along the z-axis
        /// until it makes contact with Triangle t.
        /// This function calls vertexDrop, facetDrop, and edgeDrop to do its job.
        /// Follows the template-method, or "self-delegation" design pattern.
        int dropCutter(CLPoint &cl, const Triangle &t) const;

        /// drop the MillingCutter at Point cl down along the z-axis
        /// until it makes contact with a triangle in the STLSurf s
        /// NOTE: no kd-tree optimization, this function will make 
        /// dropCutter() calls for each and every Triangle in s.
        // should not really be used for real work, demo/debug only
        int dropCutterSTL(CLPoint &cl, const STLSurf &s) const;
        
        // pushCutter methods
        virtual bool vertexPush(const Fiber& f, Interval& i, const Triangle& t) const{assert(0);return false;};
        virtual bool facetPush(const Fiber& f, Interval& i, const Triangle& t) const{assert(0);return false;};
        virtual bool edgePush(const Fiber& f, Interval& i, const Triangle& t) const{assert(0);return false;};
        
        
        virtual std::string str() const {return "MillingCutter (all derived classes should override this)";};
        
    protected:
        /// diameter of cutter
        double diameter;
        /// radius of cutter
        double radius;
        /// length of cutter
        double length;
};


/* required wrapper class for virtual functions in boost-python */
/// \brief a wrapper required for boost-python
// see documentation:
// http://www.boost.org/doc/libs/1_43_0/libs/python/doc/tutorial/doc/html/python/exposing.html#python.inheritance

class MillingCutterWrap : public MillingCutter, public boost::python::wrapper<MillingCutter>
{
    public:
        // vertex
        int vertexDrop(CLPoint &cl, const Triangle &t) const
        {
            if ( boost::python::override ovr_vertexDrop = this->get_override("vertexDrop"))
                return ovr_vertexDrop(cl, t);
            return MillingCutter::vertexDrop(cl, t);
        };
        int default_vertexDrop(CLPoint &cl, const Triangle &t) const{
            return this->MillingCutter::vertexDrop(cl,t);
        };
        
        // facet
        int facetDrop(CLPoint &cl, const Triangle &t) const
        {
            if ( boost::python::override ovr_facetDrop = this->get_override("facetDrop"))
                return ovr_facetDrop(cl, t);
            return MillingCutter::facetDrop(cl, t);
        };
        int default_facetDrop(CLPoint &cl, const Triangle &t) const{
            return this->MillingCutter::facetDrop(cl,t);
        };
        
        
        // edge
        int edgeDrop(CLPoint &cl, const Triangle &t) const
        {   
            if ( boost::python::override ovr_edgeDrop = this->get_override("edgeDrop"))
                return ovr_edgeDrop(cl, t);
            return MillingCutter::edgeDrop(cl, t);
        };    
        int default_edgeDrop(CLPoint &cl, const Triangle &t) const{
            return this->MillingCutter::edgeDrop(cl,t);
        };
        
        // offset cutter
        // FIXME, return type to python does not seem to work... ?
        MillingCutter* offsetCutter(const double d) const
        {
            if ( boost::python::override ovr_offsetCutter = this->get_override("offsetCutter") )
                return ovr_offsetCutter(d);
            return MillingCutter::offsetCutter(d);
        };   
        MillingCutter* default_offsetCutter(const double d) const {
            return this->MillingCutter::offsetCutter(d);
        };
        
        // string repr
        std::string str() const
        {
            if ( boost::python::override ovr_str = this->get_override("str")) {
                return ovr_str();
            }
            return MillingCutter::str();
        }; 
        std::string default_str() const {
            return this->MillingCutter::str();
        };
    
};






/* ********************************************************************
 *  CYLINDER    cylcutter.cpp
 * ********************************************************************/
///
/// \brief Cylindrical MillingCutter (flat-endmill)
///
/// defined by one parameter, the cutter diameter
class CylCutter : public MillingCutter {
    public:
        /// create CylCutter with diameter = 1.0
        CylCutter();
        /// create CylCutter with diameter = d
        explicit CylCutter(const double d);
        
        MillingCutter* offsetCutter(const double d) const;

        // dropCutter methods
        int vertexDrop(CLPoint &cl, const Triangle &t) const;
        int facetDrop(CLPoint &cl, const Triangle &t) const;
        int edgeDrop(CLPoint &cl, const Triangle &t) const;
        
        // pushCutter methods
        bool vertexPush(const Fiber& f, Interval& i, const Triangle& t) const;
        bool facetPush(const Fiber& f, Interval& i, const Triangle& t) const;
        bool edgePush(const Fiber& f, Interval& i, const Triangle& t) const;
        
        /// text output
        friend std::ostream& operator<<(std::ostream &stream, CylCutter c);
        /// string repr
        std::string str() const;
        
};


/* ********************************************************************
 *  SPHERE      ballcutter.cpp
 * ********************************************************************/
/// \brief Ball or Spherical MillingCutter (ball-nose endmill)
///
/// defined by one parameter. the cutter diameter.
/// the sphere radius will be diameter/2
class BallCutter : public MillingCutter {
    public:
        BallCutter();
        /// create a BallCutter with diameter d and radius d/2
        explicit BallCutter(const double d);
        
        MillingCutter* offsetCutter(const double d) const;
        
        // dropCutter methods
        int vertexDrop(CLPoint &cl, const Triangle &t) const;
        int facetDrop(CLPoint &cl, const Triangle &t) const;
        int edgeDrop(CLPoint &cl, const Triangle &t) const;
        
        // pushCutter methods   NOT DONE
        bool vertexPush(const Fiber& f, Interval& i, const Triangle& t) const;
        bool facetPush(const Fiber& f, Interval& i, const Triangle& t) const;
        bool edgePush(const Fiber& f, Interval& i, const Triangle& t) const;
        
        
        /// string repr
        friend std::ostream& operator<<(std::ostream &stream, BallCutter c);
        /// string repr
        std::string str() const;
        
};

/* ********************************************************************
 *  TORUS       bullcutter.cpp
 * ********************************************************************/
/// \brief Bull-nose or Toroidal MillingCutter (filleted endmill)
///
/// defined by the cutter diameter and by the corner radius
///
class BullCutter : public MillingCutter {
    public:
        /// Create bull-cutter with default diameter and corner radius.
        BullCutter();
        /// Create bull-cutter with diamter d and corner radius r.
        BullCutter(const double d, const double r);
        
        MillingCutter* offsetCutter(const double d) const;
        
        /// drop cutter
        int vertexDrop(CLPoint &cl, const Triangle &t) const;
        int facetDrop(CLPoint &cl, const Triangle &t) const;
        int edgeDrop(CLPoint &cl, const Triangle &t) const;
        
        /// string repr
        friend std::ostream& operator<<(std::ostream &stream, BullCutter c);
        /// string repr
        std::string str() const;
        
    protected:
        
        /// set radius of cutter
        void setRadius();  
        /// radius of cylindrical part of cutter
        double radius1;
        /// tube radius of toroid
        double radius2;
};

/* ********************************************************************
 *  CONE        conecutter.cpp
 * ********************************************************************/
/// \brief Conical MillingCutter 
///
/// cone defined by diameter and the cone half-angle(in radians). sharp tip. 
/// 60 degrees or 90 degrees are common
class ConeCutter : public MillingCutter {
    public:
        /// default constructor
        ConeCutter();
        /// create a ConeCutter with specified maximum diameter and cone-angle
        /// for a 90-degree cone specify the half-angle  angle= pi/4
        ConeCutter(const double d, const double angle);
        
        MillingCutter* offsetCutter(const double d) const;
        // drop-cutter
        int vertexDrop(CLPoint &cl, const Triangle &t) const;
        int facetDrop(CLPoint &cl, const Triangle &t) const;
        int edgeDrop(CLPoint &cl, const Triangle &t) const;
        
        /// string repr
        friend std::ostream& operator<<(std::ostream &stream, ConeCutter c);
        std::string str() const;
        
    protected:
        /// the half-angle of the cone, in radians
        double angle;
        /// the height of the cone
        double height;
};



/* ********************************************************************
 *  CompoundCutter(s)        compoundcutter.cpp
 * ********************************************************************/
/// \brief a CompoundCutter is composed one or more MillingCutters
/// the cutters are stored in a vector *cutter* and their axial offsets
/// from eachother in *zoffset*. The different cutters apply in different
/// radial regions. cutter[0] from r=0 to r=radius[0] after that 
/// cutter[1] from r=radius[0] to r=radius[1] and so on. 
class CompoundCutter : public MillingCutter {
    public:
        /// create an empty CompoundCutter
        CompoundCutter();
        /// add a MillingCutter to this CompoundCutter
        /// the cutter is valid from the previous radius out to radius
        /// and its axial offset is given by zoffset
        void addCutter(MillingCutter& c, double radius, double zoff);
        /// return true if cc_tmp is in the valid region of cutter n
        bool ccValid(int n, CLPoint& cl) const;
        
        // offsetCutter
        MillingCutter* offsetCutter(const double d) const;
        
        // dropCutter methods
        int vertexDrop(CLPoint &cl, const Triangle &t) const;
        int facetDrop(CLPoint &cl, const Triangle &t) const;
        int edgeDrop(CLPoint &cl, const Triangle &t) const;
        
        // string output
        std::string str() const;
        
    protected:        
        /// vector that holds the radiuses of the different cutters
        std::vector<double> radiusvec; // vector of radiuses
        /// vector of the axial offsets 
        std::vector<double> zoffset; // vector of z-offset values for the cutters
        /// vector of cutters in this CompoundCutter
        std::vector<MillingCutter*> cutter; // vector of pointers to cutters
};

/// \brief a MillingCutter::CompoundCutter with a cylindrical/flat central part of diameter diam1
/// and a conical outer part sloping at angle, with a max diameter diam2
class CylConeCutter : public CompoundCutter {
    public:
        CylConeCutter() {}; // dummy, required(?) by python wrapper
        CylConeCutter(double diam1, double diam2, double angle);
};

/// \brief a MillingCutter::CompoundCutter with a spherical central part of diameter diam1
/// and a conical outer part sloping at angle, with a max diameter diam2
/// the cone is positioned so that the tangent of the cone matches the tangent of the sphere
class BallConeCutter : public CompoundCutter {
    public:
        BallConeCutter() {}; // dummy, required(?) by python wrapper
        BallConeCutter(double diam1, double diam2, double angle);
};

/// \brief a MillingCutter::CompoundCutter with a toroidal central part of diameter diam1 
/// and corner radius radius1
/// The outer part is conical sloping at angle, with a max diameter diam2
/// the cone is positioned so that the tangent of the cone matches the tangent of the torus
class BullConeCutter : public CompoundCutter {
    public:
        BullConeCutter() {}; // dummy, required(?) by python wrapper
        BullConeCutter(double diam1, double radius1, double diam2, double angle);
};

/// \brief a MillingCutter::CompoundCutter with a conical central part with diam1/angle1 
/// and a conical outer part with diam2/angle2
class ConeConeCutter : public CompoundCutter {
    public:
        ConeConeCutter() {}; // dummy, required(?) by python wrapper
        ConeConeCutter(double diam1, double angle1, double diam2, double angle2);
};


} // end namespace
#endif
// end file millingcutter.h
