#include <iostream>

#include "graph.h"

/*
#include "gridGraph.h"

void test_grid_graph() {
	Graph a = Graph{};
	a.at(GRAPH_DIM/2).at(GRAPH_DIM/2) = 1;
	a = attach_new_node(a, Coord{GRAPH_DIM/2,GRAPH_DIM/2});
	std::cout << graph_to_string(a);
}
*/

int main() {
	Graph a = Graph{0, Node{0, Vector2{GRAPH_DIM/2,GRAPH_DIM/2}, std::std::vector<size_t>{}}};
	a = attach_new_node(a, 0);
	a = attach_new_node(a, 0);
	a = attach_new_node(a, 1);
	std::cout << graph_to_string(a);
	return 0;
}