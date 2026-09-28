#include "raylib.h"
#include "genericGraph.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

struct NodeData {
	Vector2 pos;
	float size;
	Color col;
};

using ND = Node<NodeData>;

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graph tester");

	Graph<NodeData> g = {
		ND{
			NodeData{
				Vector2{
					(SCREEN_WIDTH/2.f)-10.f, 
					SCREEN_HEIGHT/2.f
				}, 
			5.f,
			GREEN
			},
		0, 
		std::vector<size_t>()
		}
	};
	g.attach_to(
		ND{
			NodeData{
				Vector2{
					(SCREEN_WIDTH/2.f)+10.f, 
					SCREEN_HEIGHT/2.f
				}, 
			5.f,
			BLUE
			},
		1, 
		std::vector<size_t>()
		}, 0
	);

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
