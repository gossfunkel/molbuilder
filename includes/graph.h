#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <vector>
#include <ranges>
#include <unordered_map>
#include <string>
#include "raylib.h"
#include "raymath.h"

#define TAU (M_PI*2.)

/*
 * Intended optimisations:
 * 	Single point of read/write access to edges (no potential mismatches) for:
 * - modifying edges: fast lookup of neighbour from node
 * - drawing edges: view of unique list of edges
 * Implementation:
 * 	Adjacency matrix has redundancies (worse for bigger data). 
 *  Let's just track each node's neighbours and calculate the edges by
 * 		removing duplicates when we need to.
 */

// TODO just make a graph iterator already


//#define MAX_EDGES 4
#define EDGE_VEC Vector2{0., -25.}

typedef std::pair<size_t, size_t> Link;

typedef struct {
	Vector2 pos;
	std::vector<size_t> edges;
} Node;

typedef struct {
	std::unordered_map<size_t, Node> nodes;
	std::vector<std::vector<size_t>> layers;
} 

// FIXME deal with cyclic constraints
void move_nodes(Graph g, double dt) {
	for (auto layer_nodes : g.layers) {

	}
	for (auto [id, node] : g) {
		size_t num_nbrs = node.nbrs.length();
		if (num_nbrs <= 1) continue;
		for (size_t edge_idx = 0; edge_idx < num_nbrs; ++edge_idx) {
			Edge &edge = node.edges.at(edge_idx);
			Edge &next_edge = node.edges.at((edge_idx + 1) % num_nbrs);
			Edge &prev_edge = node.edges.at((edge_idx - 1) % num_nbrs);
			if (g.at(edge.end_id).layer >= node.layer) {
				// force = difference in angles
				for (auto e_e : g[edge].edges) {
					// TODO polygons; angle = 360/verts or 360 - 360/verts
					if (prev_edge.end_id == next_edge.end_id) {
						// triangle: fix diff in angles to 60 or 300
					} else {
						if (e_e.end_id == next_edge.end_id)
							// connected neighbours; constrain diff in angles
						if (e_e.end_id == prev_edge.end_id)
							// connected neighbours; constrain diff in angles
					}
				}
				edge.angle_vel -= (edge.angle - next_edge.angle) * dt;
				edge.angle_vel -= (edge.angle - prev_edge.angle) * dt;
			} else if (g.at(edge.end_id).layer == node.layer) {
			}
		}
	}
	for (auto [id, node] : g)
		for (auto edge : node.edges) 
			edge.angle += edge.angle_vel * dt;
}

Graph attach_new_node (Graph g, size_t attach_to) {
	if (g.size() == 0) {
		g[0] = Node {0, Neighbours{}};
		return g;
	}
	if (!g.contains(attach_to)) return g;//attach_to = 0;
	//if (g[attach_to].edges >= MAX_EDGES) return g; // recurse until edge found?
	size_t new_id = g.size();
	g[new_id] = Node{
		g[attach_to].layer + 1, 
		Neighbours{Edge{0.f, 0.f, attach_to}}
	};
	g[attach_to].edges.emplace_back(Edge{
		360.f / (g[attach_to].edges.size() + 1),
		0.f,
		new_id
	});
	return g;
}

Graph attach_node (Graph g, size_t attach_to, size_t nID) {
	if (g.contains(attach_to) && g.contains(nID)) {
		if (g[nID].layer > g[attach_to].layer)
			g[nID].layer = g[attach_to].layer + 1;
		g[attach_to].edges.emplace_back(Edge{
			360.f / (g[attach_to].edges.size() + 1),
			0.f,
			nID
		});
		g[nID].edges.emplace_back(Edge{
			360.f / (g[nID].edges.size() + 1),
			0.f,
			attach_to
		});
	}
	return g;
}

Graph attach_node (Graph g, size_t attach_to, Node nd) {
	if (!g.contains(attach_to)) return g;
	size_t new_id = g.size();
	if (nd.layer > g[attach_to].layer)
		nd.layer = g[attach_to].layer + 1;
	if ()
	g[new_id] = nd;
	g[attach_to].edges.emplace_back(Edge{
		360.f / (g[attach_to].edges.size() + 1),
		0.f,
		nID
	});
	g[new_id].edges.emplace_back(Edge{
		360.f / (g[new_id].edges.size() + 1),
		0.f,
		attach_to
	});
	return g;
}

std::vector<Link> get_all_edges(Graph g) {
    std::vector<Link> pairs;

    for (const auto& [uid, nd] : g)
        for (auto edge : nd.edges) 
			pairs.emplace_back(std::make_pair(uid, edge.end_id));
    
    const auto rm = std::ranges::sort(new_pairs);
	rm = std::ranges::unique(rm);
    new_pairs.erase(rm.begin(), rm.end());
    return new_pairs;// | std::ranges::views::remove(rm);
}

void draw_graph(Graph g) {
	std::unordered_map<size_t, Vector2> node_positions;
	node_positions[0] = (0.f,0.f);

	for (size_t n_idx = 0; n_idx < g.size(); ++n_idx) {
		for (auto edge : g[n_idx].edges) {
			if (node_positions.contains(n_idx)) continue;
			node_positions.emplace_back(
				Vector2Rotate(EDGE_VEC, edge.angle);
			);	
		}
	}
}

std::string graph_to_string (Graph g) {
	std::string out_str = "Graph: \n";
	for (auto [id, nl] : get_all_edges(g))
		for (auto [left,right] : nl)
			out_str += "--> " + std::to_string(left) 
						+ ":" + std::to_string(right) + ".\n";
	for (auto [idx,node] : g)
		out_str += "node " + std::to_string(idx) + " at pos " 
					+ std::to_string(node.pos.x) + ","
					+ std::to_string(node.pos.y) + ".\n";
	return out_str;
}

/*

typedef struct {
	float angle;
	float angle_vel;
	size_t end_id;
} Edge;

typedef std::pair<size_t, size_t> Link;
typedef std::vector<Edge> Neighbours;

typedef struct {
	size_t layer;
	Neighbours edges;
} Node;

typedef std::unordered_map<size_t, Node> Graph;

typedef std::pair<size_t, size_t> Link;
typedef std::unordered_map<size_t, NodeLinks> LinkVec;

typedef struct {
	size_t edges;
	Vector2 pos;
	std::vector<size_t> neighbours;
} Node;

std::vector<Link> get_all_edges(Graph g) {
    std::vector<Link> pairs;
    std::unordered_map<Link, bool> uniques;

    for (const auto& [uid, nd] : g)
        for (auto nbr : nd.neighbours) 
        	if (!uniques.at(p)) {
				uniques.at(p) = true;
				pairs.emplace_back(std::make_pair(uid, nbr));
			}
	std::vector<Link> new_pairs;
	for (size_t p = 0; p < pairs.size(); ++p) 
		if (!uniques.at(p)) {
			uniques.at(p) = true;
			new_pairs.emplace_back(pairs.at(p));
		}
    //const auto rm = std::ranges::sort(new_pairs);
	//rm = std::ranges::unique(rm);
    //new_pairs.erase(rm.begin(), rm.end());
    return new_pairs | std::ranges::views::remove(rm);
}

Graph attach_new_node (Graph g, size_t attach_to) {
	if (g.size() == 0) {
		g[0] = Node {0, Vector2Zero()};
		return g;
	}
	if (!g.contains(attach_to)) return g;//attach_to = 0;
	//if (g[attach_to].edges >= MAX_EDGES) return g; // recurse until edge found?
	size_t new_id = g.size();
	Vector2 new_pos = Vector2Add(g[attach_to].pos, 
				Vector2Rotate(EDGE_VEC, (TAU/(g[attach_to].edges+1)) * g[attach_to].edges)
		);
	g[new_id] = Node {1, new_pos, std::vector<size_t>{attach_to}};
	g[attach_to].edges += 1;
	g[attach_to].neighbours.emplace_back(new_id);
	return g;
}

NodeLinks get_all_edges(Graph g) {
    NodeLinks pairs;
    //std::unordered_map<Link, bool> uniques;

    for (const auto& [uid, nd] : g)
        for (auto nb : nd.neighbours) 
        	if (!uniques.at(p)) {
				uniques.at(p) = true;
				pairs.emplace_back(std::make_pair(uid, nb));
			}
	NodeLinks new_pairs;
	for (size_t p = 0; p < pairs.size(); ++p) 
		if (!uniques.at(p)) {
			uniques.at(p) = true;
			new_pairs.emplace_back(pairs.at(p));
		}
    
    //const auto rm = std::ranges::sort(new_pairs);
	//rm = std::ranges::unique(rm);

    //new_pairs.erase(rm.begin(), rm.end());
    return new_pairs | std::ranges::views::remove(rm);
}

std::string graph_to_string (Graph g) {
	std::string out_str = "Graph: \n";
	for (auto [id, nl] : get_all_edges(g))
		for (auto [left,right] : nl)
			out_str += "--> " + std::to_string(left) 
						+ ":" + std::to_string(right) + ".\n";
	for (auto [idx,node] : g)
		out_str += "node " + std::to_string(idx) + " at pos " 
					+ std::to_string(node.pos.x) + ","
					+ std::to_string(node.pos.y) + ".\n";
	return out_str;
}

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

Graph combine (Graph a, Graph b) {
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
}

typedef std::vector<std::vector<bool>> AdjMatrix;
typedef struct Graph {
	NodeMap nodes;
	AdjMatrix links;
} Graph;
*/