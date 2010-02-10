/*  Copyright 2010 Anders Wallin (anders.e.e.wallin "at" gmail.com)
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
#include <stdio.h>
#include <sstream>

#include "triangle.h"
#include "point.h"

int Triangle::count=0;

Triangle::Triangle()
{
    setId();
    p[0]=Point(1,0,0);
    p[1]=Point(0,1,0);
    p[2]=Point(0,0,1);
    calcNormal();
}

Triangle::Triangle(Point p1, Point p2, Point p3)
{
    setId();
    p[0]=p1;
    p[1]=p2;
    p[2]=p3;
    calcNormal();
}

void Triangle::calcNormal()
{
    Vector v1=p[0]-p[1];
    Vector v2=p[0]-p[2];
    // the normal is in the direction of the cross product between the edge vectors
    n = v1.cross(v2); 
    n =  n*(1 / n.norm()); // normalize to length==1
}

void Triangle::setId()
{
    id=count;
    ++count;
}

std::string Triangle::str()
{
	std::ostringstream o;
	o << "T"<< id <<"(" << p[0] << ", " << p[1] << ", " << p[2] << ")";
	return o.str();
}


std::ostream &operator<<(std::ostream &stream, const Triangle t)
{
  stream <<  "Tri(" << t.id << ") " << t.p[0] << " " << t.p[1] << " " << t.p[2] ;
  return stream;
}
