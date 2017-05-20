#include "readFiles.h"
#include "Node.h"
#include "Road.h"
#include "Graph.h"
#include "graphviewer.h"

/**
 * Calculates the distance from a node to another(in km)
 */
float calcWeight(Node from, Node to) {
	int r = 6371; // raio da terra em kms
	float dLat = to.getLatRad() - from.getLatRad();
	float dLon = to.getLonRad() - from.getLonRad();
	float a = sin(dLat / 2) * sin(dLat / 2)
			+ cos(from.getLatRad()) * cos(to.getLatRad()) * sin(dLon / 2)
					* sin(dLon / 2);
	float c = 2 * atan2(sqrt(a), sqrt(1 - a));
	float d = r * c;

	return d;
}

/**
 * Finds a node in the graph, through his ID
 */
Node findNode(Graph<Node, Road> & g, unsigned long ID) {
	for (unsigned int i = 0; i < g.getVertexSet().size(); i++) {
		if (g.getVertexSet().at(i)->getInfo().getId() == ID) {
			return g.getVertexSet().at(i)->getInfo();
		}
	}
}

/**
 * Reads the nodes from a file and loads them into the graph
 */
void readNodes(Graph<Node, Road> & g, GraphViewer *gv) {
	ifstream inFile;

	//Ler o ficheiro nos.txt
	inFile.open("database/nodesDemo.txt");

	if (!inFile) {
		cerr << "Unable to open file nodesDemo.txt";
		exit(1);
	}

	std::string line;

	unsigned long idNo = 0;
	double lat_deg = 0, lon_deg = 0, lat_rad = 0, lon_rad = 0;
	while (std::getline(inFile, line)) {
		std::stringstream linestream(line);
		std::string data;

		linestream >> idNo;

		std::getline(linestream, data, ';'); // read up-to the first ; (discard ;).
		linestream >> lat_deg;
		std::getline(linestream, data, ';'); // read up-to the first ; (discard ;).
		linestream >> lon_deg;
		std::getline(linestream, data, ';'); // read up-to the first ; (discard ;).
		linestream >> lon_rad;
		std::getline(linestream, data, ';'); // read up-to the first ; (discard ;).
		linestream >> lat_rad;
		Node n(idNo, lat_deg, lon_deg, lat_rad, lon_rad);
		g.addVertex(n);
		gv->addNode(idNo, 800 - (((lon_deg) - (long) (lon_deg)) * -1000),
				800 - (((lat_deg) - (long) (lat_deg)) * 1000));
	}

	inFile.close();

}

/**
 * Reads the edges from a file and loads them into the graph
 */
void readEdges(Graph<Node, Road> & g, GraphViewer *gv) {
	ifstream inFile;

	//Ler o ficheiro subroads.txt
	inFile.open("database/subroadsDemo.txt");

	if (!inFile) {
		cerr << "Unable to open file subroadsDemo.txt";
		exit(1);   // call system to stop
	}

	std::string line;

	unsigned long roadID;
	unsigned long node1ID, node2ID;

	while (std::getline(inFile, line)) {
		std::stringstream linestream(line);
		std::string data;

		linestream >> roadID;

		std::getline(linestream, data, ';'); // read up-to the first ; (discard ;).
		linestream >> node1ID;
		std::getline(linestream, data, ';'); // read up-to the first ; (discard ;).
		linestream >> node2ID;

		float weight = calcWeight(findNode(g, node1ID), findNode(g, node2ID));
		gv->addEdge(roadID, node1ID, node2ID, EdgeType::UNDIRECTED);
		Road r = readRoads(roadID, gv);
		if (r.is_two_way())
			g.addEdge1(findNode(g, node2ID), findNode(g, node1ID), weight, r);

		g.addEdge1(findNode(g, node1ID), findNode(g, node2ID), weight, r);

	}

	inFile.close();

}

/**
 * Finds the info of a road, through the ID of the road
 */
Road readRoads(unsigned long roadID, GraphViewer *gv) {
	ifstream inFile;

	//Ler o ficheiro nos.txt
	inFile.open("database/roadsDemo.txt");

	if (!inFile) {
		cerr << "Unable to open file roadsDemo.txt";
		exit(1);
	}

	std::string line;

	unsigned long idNo = 0;
	string name;
	string two_way;

	while (std::getline(inFile, line)) {
		std::stringstream linestream(line);
		std::string data;

		linestream >> idNo;

		std::stringstream s(line);
		std::getline(s, data, ';'); // read up-to the first ; (discard ;).
		std::getline(s, name, ';'); // read up-to the first ; (discard ;).
		std::getline(s, two_way); // read up-to the first ; (discard ;).

		gv->setEdgeLabel(idNo, name);

		if (idNo == roadID) {
			if (two_way == "FALSE") {
				Road r(idNo, name, false);
				return r;
			} else {
				Road r(idNo, name, true);
				return r;
			}
		}
	}

	inFile.close();
}

/**
 * Returns a vector with string containing the name of each hotel
 */
vector<string> getLocals() {
	ifstream inFile;

	//Ler o ficheiro hotel.txt
	inFile.open("database/locals.txt");

	if (!inFile) {
		cerr << "Unable to open file locals.txt";
		exit(1);
	}

	std::string line;

	vector<string> locals;
	string local_name;

	while (std::getline(inFile, line)) {
		std::string data;

		std::stringstream s(line);
		std::getline(s, data, ';'); // read up-to the first ; (discard ;).
		std::getline(s, local_name, ';'); // read up-to the first ; (discard ;).

		locals.push_back(local_name);

	}

	inFile.close();
	return locals;
}

