#include "raylib.h"
#include "raymath.h"
#include <vector>

#define TAU (M_PI*2.f)

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

typedef struct Edge {
    size_t a;
    size_t b;
} Edge;

class Node {
public:
    size_t m_uid;
    Vector2 m_position;

    virtual void draw();
}

class Circle : public Node {
public:
    size_t m_edges[1];
    const Color m_col {BLUE};
    Circle(size_t uid, Vector2 position) : uid {m_uid}, {
        
    }

    void draw() {
        DrawCircleV(this->position, 5, this->col);
    }
}

class Triangle : public Node {
public:
    size_t edges[2] {};
    const Color col {RED};

    void draw() {
        DrawTriangle(
            Vector2Add(this->position, Vector2(0.f,-2.f)),
            Vector2Add(this->position, Vector2(2.f,0.f)),
            Vector2Add(this->position, Vector2(-2.f,0.f)),
            this->col);
    }
}

class Ring : public Node {
public:
    size_t edges[3] {};
    const Color col {GREEN};

    void draw() {
        DrawRing(this->position, 2, 4, 0, 0, 1, this->col);
    }
}

class Square : public Node {
public:
    size_t edges[4] {};
    const Color col {ORANGE};

    void draw() {
        DrawRectangle(this->position.x - 1.f, this->position.y - 1.f, 2, 2, this->col);
    }
}

typedef struct Graph {
    std::vector<Node> node_data;
    std::vector<Edge> edge_data;
} Graph;

// TODO polymorphic function to find node of given type
//      basically pattern matching on type
size_t find_node() {

}

Graph insert_triangle(Graph graph, size_t idx) {
    Node *current_node = graph.node_data.at(idx);
    size_t nodes_visited = 0;
    bool visited[graph.node_data.n_nodes] = {false};
    while (nodes_visited < graph.node_data.n_nodes) {
        if (current_node.chr == 0 || current_node.chr == 2) break;
        for (auto edge : current_node.edges) {
            if (graph.node_data.at(graph.edge_data.at(edge).b)->chr == 0) {
                current_node = &graph.node_data.at(graph.edge_data.at(edge)->b);
                break;
            }
        }
        current_node = 
    }
    return graph;
}

Graph insert_node_to_graph(Graph graph, Node node, EdgeData node_edges) {
    size_t n_circles = graph.node_data.n_circles;
    size_t n_triangles = graph.node_data.n_triangles;
    size_t n_squares = graph.node_data.n_squares;
    size_t n_rings = graph.node_data.n_rings;
    switch (node.chr) {
    case 0: // circle
        n_circles++;
        break;
    case 1: // triangle
        n_triangles++;
        break;
    case 2: // square
        n_squares++;
        break;
    case 3: // circle
        n_rings++;
        break;
    default:
        return (Graph){0};
    }
    NodeData new_nd = (NodeData){n_circles,n_triangles,n_squares,n_rings};
    memcpy(graph.node_data, new_nd, sizeof (Node) * graph.node_data.n_nodes);
    new_nd.nodes[graph.node_data.n_nodes] = node;

    EdgeData new_ed = (EdgeData){node_edges.n_edges + graph.edge_data.n_edges};
    memcpy(graph.edge_data, new_ed, sizeof (Edge) * graph.edge_data.n_edges);
    for (size_t edge = 0; edge < node_edges.n_edges; edge++)
        new_ed.edges[graph.edge_data.n_edges++] = node_edges.edges[edge];
    return (Graph){new_nd, new_ed};
}

// explore_graph

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Geometric Graph Builder");

    //double dt = 0.f;

    size_t selected_atom = 0;

    while (!WindowShouldClose()) {
        //dt = GetFrameTime();
        //if (IsKeyReleased(KEY_SPACE)) break;

        BeginDrawing();
            ClearBackground(BLACK);

            for (size_t nd = 0; nd < ; nd++)
                
            
            
            

        EndDrawing();

    }
    CloseWindow();
    return 0;
}