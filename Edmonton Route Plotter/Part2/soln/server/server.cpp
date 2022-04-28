 /*---------------------------------------------------
 Name: Zamam Moeez
 ID: 1665264
 CMPUT 275 , Winter 2022
 CCID: zamam

 Assignment part 2
 ---------------------------------------------------*/
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <list>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "wdigraph.h"
#include "dijkstra.h"

struct Point {
    long long lat, lon;
};

// returns the manhattan distance between two points
long long manhattan(const Point& pt1, const Point& pt2) {
  long long dLat = pt1.lat - pt2.lat, dLon = pt1.lon - pt2.lon;
  return abs(dLat) + abs(dLon);
}

// finds the id of the point that is closest to the given point "pt"
int findClosest(const Point& pt, const unordered_map<int, Point>& points) {
  pair<int, Point> best = *points.begin();

  for (const auto& check : points) {
    if (manhattan(pt, check.second) < manhattan(pt, best.second)) {
      best = check;
    }
  }
  return best.first;
}

// read the graph from the file that has
// the same format as the "Edmonton graph" file
void readGraph(const string& filename, WDigraph& g,
  unordered_map<int, Point>& points) {
  ifstream fin(filename);
  string line;

  while (getline(fin, line)) {
    // split the string around the commas, there will be 4 substrings either way
    string p[4];
    int at = 0;
    for (auto c : line) {
      if (c == ',') {
        // start new string
        ++at;
      } else {
        // append character to the string we are building
        p[at] += c;
      }
    }

    if (at != 3) {
      // empty line
      break;
    }

    if (p[0] == "V") {
      // new Point
      int id = stoi(p[1]);
      // sanity check:assert if some id is not 32-bit
      assert(id == stoll(p[1]));
      points[id].lat = static_cast<long long>(stod(p[2])*100000);
      points[id].lon = static_cast<long long>(stod(p[3])*100000);
      g.addVertex(id);
    } else {
      // new directed edge
      int u = stoi(p[1]), v = stoi(p[2]);
      g.addEdge(u, v, manhattan(points[u], points[v]));
    }
  }
}

int create_and_open_fifo(const char * pname, int mode) {
  // creating a fifo special file in the current working directory
  // with read-write permissions for communication with the plotter
  // both proecsses must open the fifo before they can perform
  // read and write operations on it
  if (mkfifo(pname, 0666) == -1) {
    cout << "Unable to make a fifo. Ensure that this pipe does not exist already!" << endl;
    exit(-1);
  }

  // opening the fifo for read-only or write-only access
  // a file descriptor that refers to the open file description is
  // returned
  int fd = open(pname, mode);

  if (fd == -1) {
    cout << "Error: failed on opening named pipe." << endl;
    exit(-1);
  }

  return fd;
}

// keep in mind that in part 1, the program should only handle 1 request
// in part 2, you need to listen for a new request the moment you are done
// handling one request

#define MAX_SIZE 1024
int main() {
  WDigraph graph;  // init graph object
  unordered_map<int, Point> points;  // init points

  const char *inpipe = "inpipe";
  const char *outpipe = "outpipe";

  // Open the two pipes
  int in = create_and_open_fifo(inpipe, O_RDONLY);
  cout << "inpipe opened..." << endl;
  int out = create_and_open_fifo(outpipe, O_WRONLY);
  cout << "outpipe opened..." << endl;

  char requ[MAX_SIZE] = {};  // init readin requ char array

  // build the graph
  readGraph("server/edmonton-roads-2.0.1.txt", graph, points);
while (true) {  // while loop to constantly wait for requests
    // read a request
    read(in, requ, 1024);
    if (requ[0] == 'Q') {  // if Q break
      break;
    }
    stringstream request(requ);  // create string stream
    double sla;  // request read in as doubles
    double slo;
    double ela;
    double elo;
    Point sPoint, ePoint;
    request >> sla >> slo >> ela >> elo;
    // struct stores long long
    sPoint.lat = static_cast<long long>(sla*100000.0);
    sPoint.lon = static_cast<long long>(slo*100000.0);
    ePoint.lat = static_cast<long long>(ela*100000.0);
    ePoint.lon = static_cast<long long>(elo*100000.0);
    int start = findClosest(sPoint, points);  // find closest points
    int end = findClosest(ePoint, points);
    unordered_map<int, PIL> tree;
    dijkstra(graph, start, tree);  // run algo
    if (tree.find(end) == tree.end()) {
      write(out, requ, 0);  // if no path, write nothing
    } else {
      list<int> path;
      while (end != start) {
        path.push_front(end);
        end = tree[end].first;
      }
      path.push_front(start);
      for (int v : path) {  // convert points to string with decimals
        string lat = to_string(static_cast<double>(points[v].lat/100000.0));
        string lon = to_string(static_cast<double>(points[v].lon/100000.0));
        lat.pop_back();  // remove extra zero at end
        lon.pop_back();
        string latlon = lat + ' ' + lon + '\n';  // concatenate
        write(out, latlon.c_str(), latlon.length());  // write
      }
    }
    write(out, "E\n", 2);  // waypoints ended, write E
  }
  close(in);  // close all
  close(out);
  unlink(inpipe);
  unlink(outpipe);
  return 0;
}
