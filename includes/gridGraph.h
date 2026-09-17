
#define GRAPH_DIM 50
#include <vector>
#include <array>
#include <string>

typedef std::array<std::array<int, GRAPH_DIM>, GRAPH_DIM> Graph;
typedef std::pair<int, int> Coord;

typedef std::pair<Coord, Coord> Edge;
typedef std::vector<Edge> Edges;

Graph attach_new_node (Graph g, Coord attach_to) {
	if (g.at(attach_to.first).at(attach_to.second) <= 0) return g;
	for (int x = -1; x < 2; x += 2) {
		if (g.at(attach_to.first + x).at(attach_to.second) == 0) {
			g.at(attach_to.first + x).at(attach_to.second) = 1;
			return g;
		}
	}
	for (int y = -1; y < 2; y += 2) {
		if (g.at(attach_to.first).at(attach_to.second + y) == 0) {
			g.at(attach_to.first).at(attach_to.second + y) = 1;
			return g;
		}
	}
	return g;
}

Edges get_edges(Graph g) {
	Edges egs = {};
	for (int x = 0; x < GRAPH_DIM-1; ++x)
		for (int y = 0; y < GRAPH_DIM-1; ++y) {
			if (g.at(x).at(y) > 0 && g.at(x+1).at(y) > 0)
				egs.emplace_back(Edge{Coord{x,y},Coord{x+1,y}});
			if (g.at(x).at(y) > 0 && g.at(x).at(y+1) > 0)
				egs.emplace_back(Edge{Coord{x,y},Coord{x,y+1}});
		}
	return egs;
}

std::vector<Coord> get_nodes(Graph g) {
	std::vector<Coord> nodes = {};
	for (int x = 0; x < GRAPH_DIM-1; ++x)
		for (int y = 0; y < GRAPH_DIM-1; ++y)
			if (g.at(x).at(y) > 0) nodes.emplace_back(Coord{x,y});
	return nodes;
}

std::string graph_to_string (Graph g) {
	std::string out_str = "Graph: \n";
	for (auto [x, y] : get_nodes(g))
		out_str += "node at " + std::to_string(x) + ","
							  + std::to_string(y) + ".\n";
	for (auto [left, right] : get_edges(g))
		out_str += "--> Edge from ["  + std::to_string(left.first) 
								+ "," + std::to_string(left.second) + "] : "
								+ "[" + std::to_string(right.first) 
								+ "," + std::to_string(right.second) + "].\n";
	return out_str;
}