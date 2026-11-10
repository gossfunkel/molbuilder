#include "raylib.h"
//#include "raymath.h"
#include "genericGraph.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fresh Graph");

    // initialise graph
    Graph<Vector2> g = Graph<Vector2>(
        Node<Vector2>{
            Vector2{
                SCREEN_WIDTH/2.f,
                SCREEN_HEIGHT/2.f
            },
            0,
            std::vector<EdgeConnection>{}
    });

    while(!WindowShouldClose()) {
        // TODO node-insertion logic

        // TODO update positions (physics)
        // 1) iterate through nodes to find repulsion force,
        //     subtract force on 0 from all others.
        // 2) iterate through edges to rotate nodes around neighbours
        //     dijkstra order: edges 'above' cause all other edges to rotate
        //     (i.e. node frame of reference rotates), edges 'below' rotate
        //     around node individually (but inherit inverted rotation from other side)

        BeginDrawing();
            ClearBackground(RAYWHITE);

            // TODO draw graph
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
