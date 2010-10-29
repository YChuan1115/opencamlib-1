/*  $Id:  $
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
#include <boost/progress.hpp>

#ifdef _OPENMP // this should really not be a check for Windows, but a check for OpenMP
    #include <omp.h>
#endif

#include "point.h"
#include "triangle.h"
#include "pointdropcutter.h"
#include "kdtree3.h"

namespace ocl
{

//********   ********************** */

PointDropCutter::PointDropCutter() {
    //clpoints = new std::vector<CLPoint>();
    dcCalls = 0;
#ifdef _OPENMP
    nthreads = omp_get_num_procs(); // figure out how many cores we have
#endif
    cutter = NULL;
    bucketSize = 1;
    root = new KDTree<Triangle>();
}

void PointDropCutter::setSTL(const STLSurf &s) {
    std::cout << "pdc::setSTL()\n";
    surf = &s;
    root->setXYDimensions(); // we search for triangles in the XY plane, don't care about Z-coordinate
    root->setBucketSize( bucketSize );
    root->build(s.tris);
    std::cout << "pdc::setSTL() done.\n";
}

void PointDropCutter::setCutter(MillingCutter* c) {
    cutter = c;
}


void PointDropCutter::run(CLPoint& clp) {
    pointDropCutter1(clp);
}

// use OpenMP to share work between threads
void PointDropCutter::pointDropCutter1(CLPoint& clp) {
    //std::cout << "dropCutterSTL5 " << clpoints->size() << 
    //        " cl-points and " << surf->tris.size() << " triangles.\n";
    //boost::progress_display show_progress( clpoints->size() );
    dcCalls = 0;
    int calls=0;
    //long int ntris = 0;
    std::list<Triangle>* tris;
    //unsigned int n;
    //unsigned int Nmax = clpoints->size();
    //std::vector<CLPoint>& clref = *clpoints; 
    //int nloop=0;
    //unsigned int ntriangles = surf->tris.size();
    tris=new std::list<Triangle>();
    tris = root->search_cutter_overlap( cutter, &clp );
    std::list<Triangle>::iterator it;
    //assert( tris );
    //assert( tris->size() <= ntriangles ); // can't possibly find more triangles than in the STLSurf 
    for( it=tris->begin(); it!=tris->end() ; ++it) { // loop over found triangles  
        if ( cutter->overlaps(clp,*it) ) { // cutter overlap triangle? check
            if (clp.below(*it)) {
                cutter->dropCutter(clp,*it);
                ++calls;
            }
        }
    }
    //ntris += tris->size();
    delete( tris );
    dcCalls = calls;
    //std::cout << "\n " << dcCalls << " dropCutter() calls.\n";
    return;
}

}// end namespace
// end file batchdropcutter.cpp
