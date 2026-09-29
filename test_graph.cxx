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
	Vector2 force;
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
		int n_edges = attach_nd->edges.size();
		if (attach_id == 0) n_edges -= 1;
		new_pos = Vector2Add(Vector2Rotate(nnorth, TAU * 
				n_edges / (n_edges + 1)),
				attached_pos);
	} else new_pos = Vector2Add(attached_pos, EDGE_VEC);
	ND new_node = ND{NodeData{
		new_pos, 
		attach_nd->n_data.vel, 
		Vector2Zero(), 
		5.f, 
		SKYBLUE},
		attach_nd->layer + 1,
		std::vector<size_t>{attach_id}};
	g->attach_to(new_node, attach_id);
}

template <typename DataType>
std::pair<DataType, DataType>
repel(std::pair<DataType, DataType> nd_pair) {
	Vector2 dist = Vector2Subtract(nd_pair.first.pos,
			nd_pair.second.pos);
	float f_mag = 2.f/Vector2Length(dist);
	nd_pair.first.force += Vector2Scale(Vector2Normalize(dist),
						-f_mag);
	nd_pair.second.force += Vector2Scale(Vector2Normalize(dist),
						 f_mag);
	return nd_pair;
}

template <typename DataType>
std::pair<DataType, DataType> 
constrain_nodes(std::pair<DataType, DataType> edge, double dt) {
	if(edge.first.vel != edge.second.vel) {
		// TODO balance between edges
		// 	forces on an edge
		// 	fixed origin matches any applied force
		// neighbour 2 could move a node after neighbour 1 constrained it
		edge.first.vel += Vector2Scale(edge.first.force, dt);
		edge.second.vel += Vector2Scale(edge.second.force, dt);
		// find parallel and orthogonal components of vels
		Vector2 e = Vector2Subtract(
			edge.second.pos, edge.first.pos);
		Vector2 edge_hat = Vector2Normalize(e);
		Vector2 e1v_p = Vector2Scale(edge_hat, 
			Vector2DotProduct(edge.first.vel, edge_hat));
		Vector2 e2v_p = Vector2Scale(edge_hat, 
			Vector2DotProduct(edge.second.vel, edge_hat));
		Vector2 e1v_o = Vector2Subtract(edge.first.vel, e1v_p);
		Vector2 e2v_o = Vector2Subtract(edge.second.vel, e2v_p);
		// sum parallel components
		Vector2 total_vel = Vector2Add(e1v_p, e2v_p);
		// project orthogonal to curve
		edge.first.vel = Vector2Subtract(Vector2Add(
			Vector2Rotate(e, 180 + Vector2Length(e1v_o)), 
				edge.second.pos), edge.first.pos);
		edge.second.vel = Vector2Subtract(Vector2Add(
			Vector2Rotate(e, Vector2Length(e2v_o)), 
				edge.first.pos), edge.second.pos);
		edge.first.vel = Vector2Add(edge.first.vel, total_vel);
		edge.second.vel = Vector2Add(edge.second.vel, total_vel);
	}
	return edge;
}


int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graph tester");

	Graph<NodeData> g = {
		ND{NodeData{Vector2{(SCREEN_WIDTH/2.f), 
				     SCREEN_HEIGHT/2.f}, 
			Vector2Zero(), Vector2Zero(),
			5.f, GREEN},
		0, std::vector<size_t>()}
	};
	add_node(&g, 0);
	add_node(&g, 0);
	add_node(&g, 1);
	add_node(&g, 1);
	g.at(1)->n_data.vel = Vector2{10.f,0.f};

	double dt;
	while(!WindowShouldClose()) {
		dt = GetFrameTime();
		if(IsKeyReleased(KEY_P)) std::cout << g;
		
		g.cartesian_map(&repel);
		g.fmap_edges(&constrain_nodes, dt);
		// observer follows pinned origin node
		Vector2 global_vel = g.at(0)->n_data.vel;
		g.fmap(+[](NodeData n_d, Vector2 global_vel){
			n_d.pos -= global_vel;
			return n_d;
		}, global_vel);
		g.fmap(+[](NodeData n_d, double dt) {
			n_d.pos += Vector2Scale(n_d.vel, dt);
			return n_d;
		}, dt);
			

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
