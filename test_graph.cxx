#include "raylib.h"
#include "raymath.h"
#include "genericGraph.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define TAU M_PI*2.
#define EDGE_VEC Vector2{0.f,15.f}

struct NodeData {
	Vector2 pos;
	float size;
	Color col;
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
	ND new_node = ND{NodeData{new_pos, 5.f, SKYBLUE}};
	g->attach_to(new_node, attach_id);
}

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graph tester");

	Graph<NodeData> g = {
		ND{
			NodeData{
				Vector2{
					(SCREEN_WIDTH/2.f), 
					SCREEN_HEIGHT/2.f
				}, 
			5.f,
			GREEN
			},
		0, 
		std::vector<size_t>()
		}
	};
	add_node(&g, 0);

	while(!WindowShouldClose()) {
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
