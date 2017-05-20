#ifndef READFILES_H_
#define READFILES_H_

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include "Node.h"
#include "Road.h"
#include "Graph.h"
#include "graphviewer.h"

using namespace std;

void readNodes(Graph<Node,Road> & g, GraphViewer *gv);
Road readRoads(unsigned long roadID, GraphViewer *gv);
void readEdges(Graph<Node, Road> & g, GraphViewer *gv);
Node findNode(Graph<Node, Road> & g, unsigned long ID);
float calcWeight(Node from, Node to);
vector<string> getLocals();

#endif
