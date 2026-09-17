#include <iostream>
#include "gridGraph.h"

int main() {
	Graph a = Graph{};
	a.at(GRAPH_DIM/2).at(GRAPH_DIM/2) = 1;
	a = attach_new_node(a, Coord{GRAPH_DIM/2,GRAPH_DIM/2});
	std::cout << graph_to_string(a);
	/*Graph a = Graph{NodeMap{},LinkVec{}};
	a = attach_new_node(a, 0);
	a = attach_new_node(a, 0);
	a = attach_new_node(a, 1);
	std::cout << graph_to_string(a);*/
/*
	Graph b = Graph{NodeMap{},LinkVec{}};
	b = attach_new_node(b, 0);
	b = attach_new_node(b, 0);
	std::cout << graph_to_string(b);*/

	//Graph c = combine(a, b);
	//std::cout << graph_to_string(c);
	return 0;
}