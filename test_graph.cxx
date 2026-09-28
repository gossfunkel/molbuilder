#include "raylib.h"
#include "raymath.h"
#include "genericGraph.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define TAU M_PI*2.
#define EDGE_VEC Vector2{0.f,-15.f}

struct NodeData {
	Vector2 pos;
	Vector2 vel;
	float size;
	Color col;

	friend std::ostream& operator<<(std::ostream& os, NodeData& nd) {
		os << "{pos:" << nd.pos.x << "'" << nd.pos.y 
		   << ", vel:" << nd.vel.x << "'" << nd.vel.y
		   << ", size:" << nd.size << "}";
		return os;
	}
};

using ND = Node<NodeData>;

void add_node(Graph<NodeData> *g, size_t attach_id) {
	ND *attach_nd = &(*g->at(attach_id));
	Vector2 attached_pos = attach_nd->n_data.pos;
	Vector2 new_pos = attached_pos;
	if (attach_nd->edges.size() > 0) {
	// find 'node north' (vector along first edge)
	Vector2 nnorth = Vector2Subtract(attached_pos, 
		g->at(attach_nd->edges.at(0))->n_data.pos);
	// rotate north edge around attach_node by TAU*n/n+1
	new_pos = Vector2Rotate(nnorth, TAU * 
			attach_nd->edges.size() /
			attach_nd->edges.size() + 1);
	} else new_pos = Vector2Add(attached_pos, EDGE_VEC);
	ND new_node = ND{NodeData{
		new_pos, attach_nd->n_data.vel, 5.f, SKYBLUE}};
	g->attach_to(new_node, attach_id);
}

template <typename DataType>
std::pair<DataType, DataType> 
physics(std::pair<DataType, DataType> edge) {
	/*
	if(edge.first.vel != edge.second.vel) {
		// TODO balance between edges
		// neighbour 2 could move a node after neighbour 1 constrained it
		// find parallel and orthogonal components
		// 	of velocities
		Vector2 edge_hat = Vector2Normalize(Vector2Subtract(
			edge.second.pos, edge.first.pos));
		Vector2 e1v_p = Vector2Scale(edge_hat, 
				Vector2Dot(edge.first.vel, edge_hat));
		Vector2 e2v_p = Vector2Scale(edge_hat, 
				Vector2Dot(edge.second.vel, edge_hat));
		Vector2 e1v_o = Vector2Subtract(edge.first.vel, e1v_p);
		Vector2 e2v_o = Vector2Subtract(edge.second.vel, e2v_p);
		// average parallel components
		Vector2 total_vel = Vector2Scale(
					Vector2Add(e1v_p, e2v_p), .5f);
		// project orthogonal to curve

	}
	*/
	// TODO capture dt
	edge.first.pos += edge.first.vel;
	edge.second.pos += edge.second.vel;

	return edge;
}

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graph tester");

	Graph<NodeData> g = {
		ND{NodeData{Vector2{(SCREEN_WIDTH/2.f), 
				     SCREEN_HEIGHT/2.f}, 
			Vector2Zero(),
			5.f, GREEN},
		0, std::vector<size_t>()}
	};
	add_node(&g, 0);
	g.at(1)->n_data.vel = Vector2{1.f,0.f};

	while(!WindowShouldClose()) {

		if(IsKeyReleased(KEY_P)) std::cout << g;
		
		g.fmap_edges(&physics);
		// observer follows pinned origin node
		Vector2 global_vel = g.at(0)->n_data.vel;
		g.fmap(+[](NodeData n_d, Vector2 global_vel){
			n_d.pos -= global_vel;
			return n_d;
		}, global_vel);
			

		BeginDrawing();
		ClearBackground(BLACK);
		g.fmap(+[](NodeData n_d){
			DrawCircleV(n_d.pos, n_d.size, n_d.col); 
			return n_d;
		});
		for (auto [start, end] : g.get_unique_edges())
			DrawLineV(
				g.at(start)->n_data.pos,
				g.at(end)->n_data.pos,
				WHITE
			);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
