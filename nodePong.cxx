#include <iostream>
#include <ranges>
#include "gridGraph.h"
#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1100
#define SCREEN_HEIGHT 1100

#define NUM_SOUNDS 4

#define coord_to_position(coord) Vector2{(float)(coord).first  * 20.f + 50.f, \
										 (float)(coord).second * 20.f + 50.f}

#define get_node(gr, coord) ((gr).at((coord).first).at((coord).second))

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "NodePong");
    InitAudioDevice();

    std::array<Sound, NUM_SOUNDS> fx = {
    	LoadSound("pluck_c.wav"),
    	LoadSound("pluck_d.wav"),
    	LoadSound("pluck_e.wav"),
    	LoadSound("pluck_f.wav")
    };

	Graph main_graph = Graph{};
	main_graph.at(GRAPH_DIM/2).at(GRAPH_DIM/2) = 1;
    main_graph = attach_new_node(main_graph, Coord{GRAPH_DIM/2,GRAPH_DIM/2});

    bool node_selected = false;
    Coord selected_node = {0,0};
    Vector2 mousePos = {0.f,0.f};

    Edge cursor_edge = get_edges(main_graph).at(0);
   	float cursor_progress = {0.f};

	auto contains_either = [&](auto e) mutable {
		if (e.first == cursor_edge.second || e.second == cursor_edge.second) 
			return true; 
		else return false; 
	};

    while (!WindowShouldClose()) {
    	cursor_progress += GetFrameTime();
    	if (cursor_progress > 1.f) {
    		if (get_node(main_graph, cursor_edge.second) < 4)
    			PlaySound(fx.at(get_node(main_graph, cursor_edge.second) - 1));
    		else PlaySound(fx.at(3));
    		auto egs = get_edges(main_graph) 
    			| std::views::filter(contains_either)
    			| std::ranges::to<std::vector>();
    		auto eg = egs.at(GetRandomValue(0, egs.size()-1));
			if (eg.first == cursor_edge.second) {
				cursor_edge = eg;
			} else {
				cursor_edge.second = eg.first;
				cursor_edge.first = eg.second;
			}
    		cursor_progress = 0.f;
    	}
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            node_selected = false;
            mousePos = GetMousePosition();
            for (auto nd_coord : get_nodes(main_graph))
                if (CheckCollisionPointCircle(mousePos, coord_to_position(nd_coord) - Vector2{0.f,35.f}, 10)) {
                    selected_node = nd_coord;
                    node_selected = true;
                }
        }

        if (IsKeyReleased(KEY_SPACE) && node_selected)
            main_graph = attach_new_node(main_graph, selected_node);

        if (IsKeyReleased(KEY_ENTER) && node_selected)
            get_node(main_graph, selected_node) += 1;

        BeginDrawing();
            ClearBackground(BLACK);

            for (auto nd_coord : get_nodes(main_graph)) {
            	if (get_node(main_graph, nd_coord) == 1)
            		DrawCircleV(coord_to_position(nd_coord), 5, RED);
            	else if (get_node(main_graph, nd_coord) == 2)
            		DrawCircleV(coord_to_position(nd_coord), 5, GREEN);
            	else if (get_node(main_graph, nd_coord) == 3)
            		DrawCircleV(coord_to_position(nd_coord), 5, YELLOW);
            	else if (get_node(main_graph, nd_coord) > 3) 
            		DrawCircleV(coord_to_position(nd_coord), 5, BLUE);
            }

            for (auto [first, second] : get_edges(main_graph))
            	DrawLineV(coord_to_position(first), coord_to_position(second), WHITE);

            if (node_selected) DrawCircleLinesV(coord_to_position(selected_node), 10, PINK);

            DrawCircleV(
            	Vector2Lerp(
            		coord_to_position(cursor_edge.first),
            		coord_to_position(cursor_edge.second),
            		cursor_progress
            	), 8, LIME
            );
        EndDrawing();

    }
    for (auto s : fx) UnloadSound(s);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}