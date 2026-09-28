#include "genericGraph.h"
//#include "raylib.h"

int main() {
	/*
	std::vector<Node<Vector2>> nds;
	nds.emplace_back(Node<Vector2>(Vector2{-10.f,-10.f}, 
					std::vector<size_t>{1}));
	nds.emplace_back(Node<Vector2>(Vector2{10.f,10.f}, 
					std::vector<size_t>{0}));
	*/
	/*
	std::vector<Node<float>> nds = {
		Node<float>(-10.f, 0, std::vector<size_t>{1}),
		//		std::vector<Node<float>*>{nullptr}));
		Node<float>(10.f, 1, std::vector<size_t>{0})
		//		std::vector<Node<float>*>{nullptr}));
					
	};
	*/
	//nds.at(0).edges.at(0) = &nds.at(1);
	//nds.at(1).edges.at(0) = &nds.at(0);
	Graph<float> g = {};
	g.insert(Node<float>(-10.f, 0, std::vector<size_t>()));
	g.attach_to(Node<float>(10.f, 1, std::vector<size_t>()), 0);
	std::cout << g;
	return 0;
}
