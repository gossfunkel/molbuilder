#include <iostream>
#include "graph.h"

int main() {
	Graph a = Graph{NodeMap{},LinkVec{}};
	a = attach_new_node(a, 0);
	a = attach_new_node(a, 0);
	a = attach_new_node(a, 1);
	std::cout << graph_to_string(a);
/*
	Graph b = Graph{NodeMap{},LinkVec{}};
	b = attach_new_node(b, 0);
	b = attach_new_node(b, 0);
	std::cout << graph_to_string(b);*/

	//Graph c = combine(a, b);
	//std::cout << graph_to_string(c);
	return 0;
}