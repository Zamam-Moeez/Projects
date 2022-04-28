 /*---------------------------------------------------
 Name: Zamam Moeez
 ID: 1665264
 CMPUT 275 , Winter 2022
 CCID: zamam

 Assignment part 1
 ---------------------------------------------------*/
 #include <queue>
 #include <vector>
 #include "dijkstra.h"

//comparision so queue considers ordering
 class compare {
   public:
     bool operator()(PIPIL a, PIPIL b) {
       return a.second.second > b.second.second;
     }
 };

 /*
     Description:  calls dijkstra's algorithm to construct
                     a searchTree with the shortest path from a start vertex
                     to different verticies. 
     args: 
             graph (WDigraph) = filled graph to search to find shortest path
             searchTree (unordered map) = empty unordered map that will be changed via reference to have shortest paths
             startVertex (int) = start vertex to begin searching from
     returns: none    */

 void dijkstra(const WDigraph & graph, int startVertex,
   unordered_map < int, PIL > & searchTree) {

   // All active fires stored as follows:
   // say an entry is (v, (u, d)), then there is a fire that started at u
   // and will burn the u->v edge, reaching v at time d
   priority_queue < PIPIL, std::vector < PIPIL > , compare > fires;
   // initially startVertex burns so prev neighbour doesnt exist    
   fires.push(PIPIL(startVertex, PIL(-1, 0)));
   // while there is an active fire
   while (!fires.empty()) {
     auto earliestFire = fires.top();
     int v = earliestFire.first;
     int u = earliestFire.second.first;
     int d = earliestFire.second.second;
     // remove fire
     fires.pop();
     // if v is already "burned", there nothing to do
     if (searchTree.find(v) != searchTree.end()) {
       continue;
     }
     // record that 'v' is burned at time 'd' by a fire started from 'u'
     searchTree[v] = PIL(u, d);

     // now start fires from all edges exiting vertex v
     for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
       int nbr = * iter;
       // 'v' catches on fire at time 'd' and the fire will reach 'nbr'
       // at time d + (length of v->nbr edge)
       long long burn = d + graph.getCost(v, nbr);
       fires.push(PIPIL(nbr, PIL(v, burn)));
     }
   }
 }
