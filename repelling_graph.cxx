#include <vector>
#include <unordered_map>
#include "raylib.h"
#include "raymath.h"
#include "graph.h"

#define EDGE_LEN 25.f

typedef struct Graph {
	NodeMap nodes;
	LinkVec links;
} Graph;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Geometric Graph Builder");

    Graph g = {};

    Graph g = Graph{NodeData{}, NbrPairs{}};
    g.node_data.emplace(0, Circle(Vector2{SCREEN_WIDTH/2.f - 50.f, SCREEN_HEIGHT/2.f}));
    g.node_data.emplace(1, Circle(Vector2{SCREEN_WIDTH/2.f + 50.f, SCREEN_HEIGHT/2.f}));
    g.node_data.at(0).m_neighbours.emplace_back(1);
    g.node_data.at(1).m_neighbours.emplace_back(0);
    g.all_nbrs = refresh_all_nbrs(g.node_data);

    while (!WindowShouldClose()) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            mousePos = GetMousePosition();
            for (auto& [uid, nd] : g.node_data)
                if (CheckCollisionPointCircle(mousePos, nd.m_position, 10)) {
                    selected_node = uid;
                    hit = true;
                }
            if (!hit) selected_node = -1;
            hit = false;
        }

        if (IsKeyPressed(KEY_R)) {
            create_node_type = NodeType{RING};
        } else if (IsKeyPressed(KEY_T)) {
            create_node_type = NodeType{TRIANGLE};
        } else if (IsKeyPressed(KEY_S)) {
            create_node_type = NodeType{SQUARE};
        }

        if (IsKeyReleased(KEY_ENTER)) {
            int new_node_uid;
            switch (create_node_type) {
            case NodeType{RING}:
                new_node_uid = add_ring(&g.node_data, selected_node);
                break;
            case NodeType{SQUARE}:
                new_node_uid = add_square(&g.node_data, selected_node);
                break;
            case NodeType{TRIANGLE}:
                //g = add_tri(g, selected_node);
                std::cout << "Adding triangle not yet implemented." << std::endl;
            default:
                new_node_uid = -1;
            }
            if (new_node_uid >= 0) {
                if (cursor_state == selected_node) cursor_state = new_node_uid;
                if (cursor_dest == selected_node) cursor_dest = new_node_uid;
                g.all_nbrs = refresh_all_nbrs(g.node_data);
            }
        }

		move_nbrs(0);

        BeginDrawing();
            ClearBackground(BLACK);
                
            for (auto& [uid, nd] : g.node_data) draw_node(nd);

            if (selected_node >= 0) {
                DrawRectangle(20,20,200,160,LIGHTGRAY);
                std::string selected_text = "Selected node: " + std::to_string(g.node_data.at(selected_node).m_uid);
                for (auto nbr : g.node_data.at(selected_node).m_neighbours)
                    selected_text.append("\n" + std::to_string(nbr));
                DrawText(selected_text.c_str(), 30, 40, 18, WHITE);
                DrawCircleLinesV(g.node_data.at(selected_node).m_position, 7, WHITE);
            }
	
	return 0;
}