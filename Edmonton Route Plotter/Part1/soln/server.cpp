 /*---------------------------------------------------
 Name: Zamam Moeez
 ID: 1665264
 CMPUT 275 , Winter 2022
 CCID: zamam

 Assignment part 1
 ---------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <list>
#include "wdigraph.h"
#include "dijkstra.h"

struct Point {
long long lat;  // latitude of the point
long long lon;  // longitude of the point
};

long long manhattan(const Point& pt1, const Point& pt2) {
// Return the Manhattan distance between the two given points
	return abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
}

/* reads input from file and creates WDigraph object 

Arguements:
string filename
WDigraph& graph
unordered_map<int, Point>& points

Returns:
Nothing

*/
void readGraph(string filename, WDigraph& graph,
  unordered_map<int, Point>& points) {
ifstream fin(filename);
  string line;  // initialise string
  while (getline(fin, line)) {  // while these is a line of text
    stringstream ss(line);  // create a string stream
    string Label, ID, v1, v2, l1, l2;  // init 3 substrings
    getline(ss, ID, ',');  // parse string and store substr in ID
    if (ID == "V") {  // if V store first vertex and add to graph
      getline(ss, v1, ',');
      int id = stoi(v1);
      graph.addVertex(id);
      getline(ss, l1, ',');
      getline(ss, l2, ',');
      points[id].lat = static_cast<long long>(stod(l1) * 100000);  // store lat
      points[id].lon = static_cast<long long>(stod(l2) * 100000);  // store lon
    } else if (ID == "E") {  // if E get 2 vertices and add edge and cost
      getline(ss, v1, ',');
      getline(ss, v2, ',');
      graph.addEdge(stoi(v1), stoi(v2),
        manhattan(points[stoi(v1)], points[stoi(v2)]));
    }
  }
  fin.close();  // close file
}

/* returns closest vertex to latv and lonv

Arguements:
long long latv
long long lonv
const unordered_map<int, Point> &points

Returns:
int vert

*/
int nearvert(long long latv, long long lonv,
  const unordered_map<int, Point> &points) {
  int vert;
  long long lowerdist, closedist;
  for (auto& it : points) {  // iterate through points, compute init distance
            lowerdist = abs(it.second.lat - latv) + abs(it.second.lon - lonv);
            if (it.first == points.begin()->first) {
                closedist = lowerdist;
            }  // if distance is less than previous lowst
            if (lowerdist < closedist) {
                vert = it.first;  // closer vertex found
                closedist = lowerdist;  // new lowest distance
            }
        }
        return vert;  // return closest vertexx
}

int main() {
    string filename = "edmonton-roads-2.0.1.txt";  // data file
    WDigraph graph;  // init graph obj
    unordered_map<int, Point> points;  // init points map
    list<int> path;  // init path
    readGraph(filename, graph, points);  // build graph
    long long startlat, startlon, endlat, endlon;  // init start/end coord
    int verts, verte;  // init start,end vertices
    char request;  // init request
    cin >> request;  // input request
    if (request == 'R') {  // if R
        cin >> startlat >> startlon >> endlat >> endlon;
        unordered_map<int, PIL> tree;  // search tree
        verts = nearvert(startlat, startlon, points);  // closest vertices
        verte = nearvert(endlat, endlon, points);
        dijkstra(graph, verts, tree);  // build path
        if (tree.find(verte) == tree.end()) {  // if tree empty then no path
          cout << "N 0" << endl;
          cout << "E" << endl;
        } else {
          int conid = verte;  // conecutive id
          while (conid != verts) {  // if not the start
            path.push_front(conid);  // add to path
            conid = tree[conid].first;  // move to next
          }
          path.push_front(verts);  // add first
          cout << "N" << " " << path.size() << endl;  // number of vertices
          for (auto i : path) {
            cin >> request;
            if (request == 'A') {  // if A print waypoint
              cout << "W " << points[i].lat << " " << points[i].lon << endl;
            }
          }
          cin >> request;  // print that it has ended
          if (request == 'A') {
            cout << "E" << endl;
          }
        }
    }
    return 0;
}
