#include "raylib.h"
#include "genericGraph.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graph tester");

	Graph<Vector2> g = {Node<Vector2>{
		Vector2{(SCREEN_WIDTH/2.f)-10.f, SCREEN_HEIGHT/2.f}, 
		0, 
		std::vector<size_t>()
	}};
	g.attach_to(Node<Vector2>{
		Vector2{(SCREEN_WIDTH/2.f)+10.f, SCREEN_HEIGHT/2.f}, 
		1, 
		std::vector<size_t>()
	}, 0);

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		g.fmap(+[](Vector2 pos){
			DrawCircleV(pos, 5.f, SKYBLUE); return pos;
		});
		for (auto [start, end] : g.get_unique_edges())
			DrawLineV(
				g.at(start)->n_data,
				g.at(end)->n_data,
				WHITE
			);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
