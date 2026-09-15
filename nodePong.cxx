#include <iostream>
#include "graph.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

Graph main_graph = Graph{NodeMap{},LinkVec{}};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "NodePong");
    InitAudioDevice();

    main_graph = attach_new_node(main_graph, 0);
    main_graph.nodes[0].pos = Vector2{SCREEN_WIDTH/2., SCREEN_HEIGHT/2.};

    bool hit = false;
    int selected_node = {-1};
    Vector2 mousePos = {0.,0.};

    while (!WindowShouldClose()) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            mousePos = GetMousePosition();
            for (auto& [id, nd] : main_graph.nodes)
                if (CheckCollisionPointCircle(mousePos, nd.pos, 10)) {
                    selected_node = id;
                    hit = true;
                }
            if (!hit) selected_node = -1;
            hit = false;
        }

        if (IsKeyReleased(KEY_SPACE) && selected_node >= 0)
            main_graph = attach_new_node(main_graph, selected_node);

        BeginDrawing();
            ClearBackground(BLACK);

            for (auto [idx, node] : main_graph.nodes)
            	DrawCircleV(node.pos, 5, RED);

            for (auto [first, second] : main_graph.links)
            	DrawLineV(main_graph.nodes[first].pos, main_graph.nodes[second].pos, WHITE);

            DrawCircleLinesV(main_graph.nodes[selected_node].pos, 7, YELLOW);
        EndDrawing();

    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}