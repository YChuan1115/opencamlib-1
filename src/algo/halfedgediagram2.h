/*  $Id: $
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
#ifndef HEDI2_H
#define HEDI2_H

#include <vector>
#include <list>

#include <boost/graph/adjacency_list.hpp>
#include <boost/foreach.hpp> 

#include "point.h"
#include "numeric.h"
namespace ocl
{
    
// bundled BGL properties, see: http://www.boost.org/doc/libs/1_44_0/libs/graph/doc/bundles.html

// dcel notes from http://www.holmes3d.net/graphics/dcel/

// vertex (boost::out_edges)
//  -leaving pointer to HalfEdge that has this vertex as origin
//   if many HalfEdges have this vertex as origin, choose one arbitrarily

// HalfEdge
//  - origin pointer to vertex (boost::source)
//  - face to the left of halfedge
//  - twin pointer to HalfEdge (on the right of this edge)
//  - next pointer to HalfEdge
//     this edge starts from h->twin->origin and ends at next vertex in h->face
//     traveling ccw around boundary
//     (allows face traverse, follow h->next until we arrive back at h)

// Face
//  - edge pointer to HalfEdge
//    this edge has this Face object as face
//    half-edge can be any one on the boundary of face


// special "infinite face", face on "outside" of boundary
// may or may not store edge pointer

// fwd decl:    


//template <class VertexProps, class EdgeProps, class FaceProps>



template <class OutEdgeList, 
          class VertexList,
          class Directed, 
          class VertexProperties,
          class EdgeProperties,
          class FaceProperties,
          class GraphProperties,
          class EdgeList 
          >
class HEDIGraph : public boost::adjacency_list< OutEdgeList,           // out-edge storage
                                                VertexList,            // vertex set storage
                                                Directed,              // directed tag
                                                VertexProperties,      // vertex properties
                                                EdgeProperties,        // edge properties
                                                GraphProperties,       // graph-properties (?)
                                                EdgeList               // edge storage
                                                > 
{
    public:
        typedef unsigned int HEFace; 
        typedef boost::adjacency_list< OutEdgeList,            
                                                VertexList,            
                                                Directed,   
                                                VertexProperties,             
                                                EdgeProperties,                
                                                GraphProperties,
                                                EdgeList
                                                > BGLGraph;
        /// use base class operator[] for vertex and edge properties
        using BGLGraph::operator[]; 
        /// operator[] to access face properties
        FaceProperties& operator[](HEFace f)  { 
            return faces[f]; 
        }
        /// const operator[] for accessing face properties
        const FaceProperties& operator[](HEFace f) const  { 
            return faces[f]; 
        }
//DATA
        std::vector<FaceProperties> faces;
};

namespace hedi  { // collect half-edge diagram functions here.

/// add a blank vertex and return its descriptor
template<class Graph>
typename boost::graph_traits< Graph >::vertex_descriptor add_vertex(Graph& g) { 
    return boost::add_vertex( g );
}

/// add a vertex with given properties, return vertex descriptor
template <class Graph, class VertexProperty>
typename boost::graph_traits< Graph >::vertex_descriptor add_vertex(const VertexProperty& prop, Graph& g) {
    return boost::add_vertex( prop, g );
}

/// add an edge between vertices v1-v2
template <class Graph>
typename boost::graph_traits< Graph >::edge_descriptor add_edge(typename boost::graph_traits< Graph >::vertex_descriptor v1, 
                                                       typename boost::graph_traits< Graph >::vertex_descriptor v2, 
                                                       Graph& g) {
    typename boost::graph_traits< Graph >::edge_descriptor e;
    bool b;
    boost::tie( e , b ) = boost::add_edge( v1, v2, g);
    return e;
}
        
        
/*        
        /// add an edge with given properties
        HEEdge add_edge(HEVertex v1, HEVertex v2, EdgeProps prop) {
            HEEdge e;
            bool b;
            boost::tie( e , b ) = boost::add_edge( v1, v2, prop, g);
            return e;
        }
*/

/// add a face with given properties
template <class Graph, class FaceProperty>
unsigned int add_face(FaceProperty f_prop, Graph& g) {
    g.faces.push_back(f_prop); 
    unsigned int index = g.faces.size()-1;
    g.faces[index].idx = index;
    return index;    
}
        

/// return the target vertex of the given edge
template <class Graph>
typename boost::graph_traits< Graph >::vertex_descriptor target( typename boost::graph_traits< Graph >::edge_descriptor e,
                                                                const Graph& g) { 
    return boost::target( e, g);
}

/// return the source vertex of the given edge
template <class Graph>
typename boost::graph_traits< Graph >::vertex_descriptor source( typename boost::graph_traits< Graph >::edge_descriptor e,
                                                        const Graph& g)  { 
    return boost::source( e, g); 
}

/// return all vertices in a vector of vertex descriptors
template<class Graph>
typename std::vector< typename boost::graph_traits<Graph>::vertex_descriptor > vertices(const Graph& g)  {
    typedef typename boost::graph_traits< Graph >::vertex_descriptor  HEVertex;
    typedef std::vector<HEVertex> VertexVector;
    typedef typename boost::graph_traits< Graph >::vertex_iterator    HEVertexItr;
    VertexVector vv;
    HEVertexItr it_begin, it_end, itr;
    boost::tie( it_begin, it_end ) = boost::vertices( g );
    for ( itr=it_begin ; itr != it_end ; ++itr ) {
        vv.push_back( *itr );
    }
    return vv;
}

/// return all vertices adjecent to given vertex
template <class Graph>
typename std::vector< typename boost::graph_traits< Graph >::vertex_descriptor > 
    adjacent_vertices(  typename boost::graph_traits< Graph >::vertex_descriptor v, Graph& g) {
    typedef typename boost::graph_traits< Graph >::vertex_descriptor  HEVertex;
    typedef typename boost::graph_traits< Graph >::edge_descriptor    HEEdge;
    typedef std::vector<HEVertex> VertexVector;
    VertexVector vv;
    BOOST_FOREACH( HEEdge edge, out_edges( v , g) ) {
        vv.push_back( target( edge , g) );
    }
    return vv;
}

/// return all vertices of given face
template <class Graph>
typename std::vector< typename boost::graph_traits< Graph >::vertex_descriptor > face_vertices(unsigned int face_idx, Graph& g) {
    typedef typename boost::graph_traits< Graph >::vertex_descriptor  HEVertex;
    typedef typename boost::graph_traits< Graph >::edge_descriptor    HEEdge;
    typedef std::vector<HEVertex> VertexVector;
    
    VertexVector verts;
    HEEdge startedge = g[face_idx].edge; // the edge where we start
    HEVertex start_target = boost::target( startedge, g); 
    verts.push_back(start_target);
    HEEdge current = g[startedge].next;
    do {
        HEVertex current_target = boost::target( current, g); 
        assert( current_target != start_target );
        verts.push_back(current_target);
        current = g[current].next;
    } while ( current != startedge );
    return verts;
}

/// return degree of given vertex
template <class Graph>
unsigned int degree(typename boost::graph_traits< Graph >::vertex_descriptor v, const Graph& g)  { 
    return boost::degree( v, g); 
}

/// return number of vertices in graph
template <class Graph>
unsigned int num_vertices(const Graph& g) { 
    return boost::num_vertices( g ); 
}

/// return out_edges of given vertex
template <class Graph>
typename std::vector< typename boost::graph_traits< Graph >::edge_descriptor > out_edges( 
        typename boost::graph_traits< Graph >::vertex_descriptor v , const Graph& g)  {
    typedef typename boost::graph_traits< Graph >::edge_descriptor  HEEdge;
    typedef std::vector<HEEdge> EdgeVector;  
    typedef typename boost::graph_traits< Graph >::out_edge_iterator  HEOutEdgeItr;
    EdgeVector ev;
    HEOutEdgeItr it, it_end;
    boost::tie( it, it_end ) = boost::out_edges( v, g );
    for ( ; it != it_end ; ++it ) {
        ev.push_back(*it);
    }
    return ev;
}

/// return all edges
template <class Graph>
typename std::vector< typename boost::graph_traits< Graph >::edge_descriptor  >  edges(Graph& g) {
    typedef typename boost::graph_traits< Graph >::edge_descriptor  HEEdge;
    typedef typename std::vector< HEEdge > EdgeVector;
    typedef typename boost::graph_traits< Graph >::edge_iterator      HEEdgeItr; 
    EdgeVector ev;
    HEEdgeItr it, it_end;
    boost::tie( it, it_end ) = boost::edges( g );
    for ( ; it != it_end ; ++it ) {
        ev.push_back(*it);
    }
    return ev;
}
        
        
        /// return edges of face f
        /*
        EdgeVector face_edges(HEFace f) {
            HEEdge start_edge = faces[f].edge;
            HEEdge current_edge = start_edge;
            EdgeVector out;
            do {
                out.push_back(current_edge);
                current_edge = g[current_edge].next;
            } while( current_edge != start_edge );
            return out;
        }*/

/// return the previous edge. traverses all edges in face until previous found.
template <class Graph>
typename boost::graph_traits< Graph >::edge_descriptor previous_edge(
                      typename boost::graph_traits< Graph >::edge_descriptor e, Graph& g) {
    typename boost::graph_traits< Graph >::edge_descriptor previous = g[e].next;
    while ( g[previous].next != e ) {
        previous = g[previous].next;
    }
    return previous;
}


/// return true if v1-v2 edge exists
template <class Graph>
bool edge( typename boost::graph_traits< Graph >::vertex_descriptor v1, 
           typename boost::graph_traits< Graph >::vertex_descriptor v2, 
           Graph& g ) {
    typedef typename boost::graph_traits< Graph >::edge_descriptor HEEdge;
    typedef typename std::pair<HEEdge, bool> EdgeBool;
    EdgeBool result = boost::edge(v1, v2, g );
    return result.second;
}


/// return adjacent faces to the given vertex
template <class Graph>
std::vector<unsigned int> adjacent_faces( typename boost::graph_traits< Graph >::vertex_descriptor q , Graph& g) {
    typedef typename boost::graph_traits< Graph >::out_edge_iterator  HEOutEdgeItr;
    typedef std::vector<unsigned int> FaceVector;
    
    std::set<unsigned int> face_set;
    HEOutEdgeItr itr, itr_end;
    boost::tie( itr, itr_end) = boost::out_edges(q, g);
    for ( ; itr!=itr_end ; ++itr ) {
        face_set.insert( g[*itr].face );
    }
    //assert( face_set.size() == 3); // degree of q is three, so has three faces
    FaceVector fv;
    BOOST_FOREACH(unsigned int m, face_set) {
        fv.push_back(m);
    }
    return fv;
}

/// return number of faces in graph
template <class Graph>
unsigned int num_faces(Graph& g) { 
    return g.faces.size(); 
}

/// return number of edges in graph
template <class Graph>
unsigned int num_edges(const Graph& g) { 
    return boost::num_edges( g ); 
}

/// inserts given vertex and its twin into edge e
template <class Graph>
void insert_vertex_in_edge(typename boost::graph_traits< Graph >::vertex_descriptor  v, 
                           typename boost::graph_traits< Graph >::edge_descriptor e, 
                           Graph& g) {
    typedef typename boost::graph_traits< Graph >::edge_descriptor    HEEdge;
    typedef typename boost::graph_traits< Graph >::vertex_descriptor  HEVertex;
    // the vertex v is in the middle of edge e
    //                    face
    //                    e1   e2
    // previous-> source  -> v -> target -> next
    //            tw_trg  <- v <- tw_src <- tw_previous
    //                    te2  te1
    //                    twin_face
    
    HEEdge twin = g[e].twin;
    HEVertex source = boost::source( e , g );
    HEVertex target = boost::target( e , g);
    HEVertex twin_source = boost::source( twin , g);
    HEVertex twin_target = boost::target( twin , g );
    assert( source == twin_target );    
    assert( target == twin_source );
    
    unsigned int face = g[e].face;
    unsigned int twin_face = g[twin].face;
    HEEdge previous = previous_edge(e, g);
    assert( g[previous].face == g[e].face );
    HEEdge twin_previous = previous_edge(twin, g);
    assert( g[twin_previous].face == g[twin].face );
    
    HEEdge e1 = add_edge( source, v , g);
    HEEdge e2 = add_edge( v, target , g);
    
    // preserve the left/right face link
    g[e1].face = face;
    g[e2].face = face;
    // next-pointers
    g[previous].next = e1;
    g[e1].next = e2;
    g[e2].next = g[e].next;
    
    
    HEEdge te1 = add_edge( twin_source, v  , g);
    HEEdge te2 = add_edge( v, twin_target  , g);
    
    g[te1].face = twin_face;
    g[te2].face = twin_face;
    
    g[twin_previous].next = te1;
    g[te1].next = te2;
    g[te2].next = g[twin].next;
    
    // TWINNING (note indices 'cross', see ASCII art above)
    g[e1].twin = te2;
    g[te2].twin = e1;
    g[e2].twin = te1;
    g[te1].twin = e2;
    
    // update the faces (required here?)
    g.faces[face].edge = e1;
    g.faces[twin_face].edge = te1;
    
    // finally, remove the old edge
    boost::remove_edge( e   , g);
    boost::remove_edge( twin, g);
}

        
        /// check that all edges belong to the correct face
        /*
        bool checkFaces() {
            BOOST_FOREACH(FaceProps f, faces) {
                BOOST_FOREACH( HEEdge e, face_edges(f.idx)) {
                    if ( g[e].face != f.idx )
                        return false;
                }
            }
            return true;
        }*/

/// delete a vertex
template <class Graph>
void delete_vertex(typename boost::graph_traits< Graph >::vertex_descriptor  v, Graph& g) { 
    clear_vertex(v, g);
    remove_vertex(v, g); 
}

/// clear given vertex. this removes all edges connecting to the vertex.
template <class Graph>
void clear_vertex(typename boost::graph_traits< Graph >::vertex_descriptor v, Graph& g) { 
    boost::clear_vertex( v, g ); 
}
/// remove given vertex
template <class Graph>
void remove_vertex(typename boost::graph_traits< Graph >::vertex_descriptor v, Graph& g) { 
    boost::remove_vertex( v , g );
}

} // end hedi namespace

} // end ocl namespace
#endif
// end halfedgediagram2.h
