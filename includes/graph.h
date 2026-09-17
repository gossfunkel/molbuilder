#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <vector>
#include <ranges>
#include <unordered_map>
#include <string>
#include "raylib.h"
#include "raymath.h"

/*
 * Intended optimisations:
 * 	Single point of read/write acces to edges (no potential mismatches) for:
 * - modifying edges: fast lookup of neighbour from node
 * - drawing edges: view of unique list of edges
 * Implementation:
 * 	Adjacency matrix has redundancies
 */

#define MAX_EDGES 4
#define EDGE_VEC Vector2{25.,0.}

typedef struct Node {
	size_t edges;
	Vector2 pos;
} Node;

typedef std::pair<size_t, size_t> Link;
typedef std::vector<Link> NodeLinks;
typedef std::unordered_map<size_t, NodeLinks> LinkVec;
typedef std::unordered_map<size_t, Node> NodeMap;
//typedef std::vector<std::vector<bool>> AdjMatrix;

typedef struct Graph {
	NodeMap nodes;
	//AdjMatrix links;
	LinkVec links;
} Graph;

/*Graph combine (Graph a, Graph b) {
	Graph g = a;
	for (auto [idx,node] : b.nodes) 
		if (!a.nodes.contains(idx)) {
			g.nodes[idx] = node;
		} // UPDATE ASSOCIATED LINKS?
	g.links.insert(g.links.begin(), b.links.begin(), b.links.end());
    std::ranges::sort(g.links);
    const auto rm = std::ranges::unique(g.links);
    g.links.erase(rm.begin(), rm.end());
	return a;
}*/

NodeLinks get_unique_links(Graph g) {
	NodeLinks links = NodeLinks{};
	for (auto [id, nl] : g.links)
		links.insert(links.end(), nl.begin(), nl.end());

    std::ranges::sort(links);
    const auto rm = std::ranges::unique(links);
    links.erase(rm.begin(), rm.end());
    return links;
}

Graph attach_new_node (Graph g, size_t attach_to) {
	if (g.nodes.size() == 0) {
		g.nodes[0] = Node {0, Vector2Zero()};
		return g;
	}
	if (!g.nodes.contains(attach_to)) return g;//attach_to = 0;
	if (g.nodes[attach_to].edges >= MAX_EDGES) return g; // recurse until edge found?
	Vector2 new_pos = Vector2Add(g.nodes[attach_to].pos, 
				Vector2Rotate(EDGE_VEC, .5f * M_PI * g.nodes[attach_to].edges)
		);
	if (g.nodes[attach_to].edges > 0) {
		// TODO calculate unoccupied position among all nodes
		for (auto [a,b] : g.links[attach_to])
			if(CheckCollisionPointCircle(new_pos, g.nodes[b].pos, 10))
				new_pos = Vector2Rotate(new_pos, .5f * M_PI);
	}
	g.nodes[attach_to].edges += 1;
	size_t new_id = g.nodes.size();
	g.nodes[new_id] = Node {1, new_pos};
	g.links[attach_to].emplace_back(attach_to, new_id);
	g.links[new_id] = NodeLinks{Link{new_id, attach_to}};
	return g;
}

std::string graph_to_string (Graph g) {
	std::string out_str = "Graph: \n";
	for (auto [id, nl] : g.links)
		for (auto [left,right] : nl)
			out_str += "--> " + std::to_string(left) 
						+ ":" + std::to_string(right) + ".\n";
	for (auto [idx,node] : g.nodes)
		out_str += "node " + std::to_string(idx) + " at pos " 
					+ std::to_string(node.pos.x) + ","
					+ std::to_string(node.pos.y) + ".\n";
	return out_str;
}