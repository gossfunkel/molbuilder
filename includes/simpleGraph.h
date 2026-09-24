#define EDGE_VEC Vector2{0., -25.}

struct Node {
	Vector2 pos;
	std::vector<size_t> nbrs;
};

typedef std::unordered_map<size_t, Node> Graph;

Graph add_node(Graph g, size_t add_to) {
	size_t new_id = g.size();
	g.at(add_to).nbrs.emplace_back(new_id);
	g[new_id] = Node{Vector2Zero(), std::vector<size_t>{}};
	size_t num_nbrs = g.at(add_to).nbrs.size();
	// TODO go through upstream nodes - add to total transform
	for (auto nbr : g.at(add_to).nbrs)
		g.at(nbr).pos = Vector2Add(g.at(add_to), Vector2Rotate(EDGE_VEC, 360.f/num_nbrs));
	// TODO go through downstream nodes and apply total transform
	return g;
}