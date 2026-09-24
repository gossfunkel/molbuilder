#include "genericGraph.h"
#include "raylib.h"

int main() {
	std::vector<Node<Vector2>> nds;
	nds.emplace_back(Node<Vector2>(Vector2{-10.f,-10.f}, std::vector<size_t>{1}));
	nds.emplace_back(Node<Vector2>(Vector2{10.f,10.f}, std::vector<size_t>{0}));
	Graph g = Graph(nds);
	std::cout << g;
}