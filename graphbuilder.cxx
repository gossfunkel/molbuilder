#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>

#define TAU (M_PI*2.f)

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

/* NOTES/TODO -----------------------------------------------------------------
 *
 *  We can't store indices in a dynamic graph without having to check and 
 *      update all references whenever a change is made before the end of the 
 *      data.
 *  Node uid allows specific nodes to be found; use hashmaps for nodes and 
 *      edges.
 *
 *  Should the constructor for an object also insert it? Or should we delay 
 *      initialisation until we have searched the graph to check if there's a 
 *      legal move?
 *
 */

typedef struct Edge {
    size_t a;
    size_t b;
} Edge;

typedef enum NodeType {
    NULL,
    CIRCLE,
    RING,
    TRIANGLE,
    SQUARE
} NodeType;

class Node {
public:
    static size_t num_nodes;
    size_t m_uid;
    Vector2 m_position;
    bool m_is_leaf;

    Node(Vector2 position) : m_position {position} {
        m_uid = num_nodes++;
        std::cout << "Making node " << m_uid << ": ";
    }

    Node() {
        Node(Vector2Zero());
    }

    virtual void draw() const {};
};

size_t Node::num_nodes = 0;

typedef struct Graph {
    std::vector<Node *> node_data;
    std::vector<Edge> edge_data;
} Graph;

class Circle : public Node {
public:
    size_t m_edges[1];
    const Color m_col {BLUE};
    const NodeType m_nodetype {NodeType::CIRCLE};

    Circle(Vector2 position) : Node{position} {
        std::cout << "circle." << std::endl;
    }

    Circle() : Node{} {
        std::cout << "circle." << std::endl;
    }

    virtual void draw() const {
        DrawCircleV(this->m_position, 5, this->m_col);
    }
};

class Ring : public Node {
public:
    size_t m_edges[2] {};
    const Color m_col {RED};
    const NodeType m_nodetype {NodeType::RING};

    Ring(Vector2 position) : Node{position} {
        std::cout << "ring." << std::endl;
    }

    Ring() : Node{} {
        std::cout << "ring." << std::endl;
    }

    virtual void draw() const {
        DrawRing(this->m_position, 3, 6, 0, 360, 1, this->m_col);
    }
};

class Triangle : public Node {
public:
    size_t m_edges[3] {};
    const Color m_col {GREEN};
    const NodeType m_nodetype {NodeType::TRIANGLE};

    Triangle(Vector2 position) : Node{position} {
        std::cout << "triangle." << std::endl;
    }

    Triangle() : Node{} {
        std::cout << "triangle." << std::endl;
    }

    virtual void draw() const {
        DrawTriangle(
            Vector2Add(this->m_position, Vector2{ 0.f, -4.f}),
            Vector2Add(this->m_position, Vector2{-5.f,  4.f}),
            Vector2Add(this->m_position, Vector2{ 5.f,  4.f}),
            this->m_col);
    }
};

class Square : public Node {
public:
    size_t m_edges[4] {};
    const Color m_col {ORANGE};
    const NodeType m_nodetype {NodeType::SQUARE};

    Square(Vector2 position) : Node{position} {
        std::cout << "square." << std::endl;
    }

    Square() : Node{} {
        std::cout << "square." << std::endl;
    }

    virtual void draw() const {
        DrawRectangle(this->m_position.x - 4.f, this->m_position.y - 4.f, 8, 8, this->m_col);
    }
};

void explore_node(Group *gr, Node *node, std::vector<Node *> *node_list) {
    // TODO this doesn't work if the graph contains loops (is this possible?)
    if (!node->m_is_leaf) {
        for (auto nbr_uid : curr_node->m_edges) {
            explore_node(gr, gr->node_data.at(nbr_uid), node_list);
        }
    }
    node_list->emplace_back(*node);
}

int find_node(Group *gr, size_t node_type, int backup_node_type) {
    // 1. linearise graph in priority order (nodetype then position)
    //      1.1 start with root node
    //      1.2 go through first edge until a leaf is found
    //      1.3 add node to list, step back and continue for all neighbours
    //      1.4 repeat until all neighbours have been explored
    // 2. check each node for a match, stopping on the first match
    // explore unexplored nodes
    std::vector<Node *> node_list;
    explore_node(gr, gr.node_data.at(0), &node_list);
    for (auto node : node_list) if (node->m_nodetype == node_type) return node->m_uid;
    if (backup_node_type >= 0)
        for (auto node : node_list) if (node->m_nodetype == backup_node_type) return node->m_uid;
    return -1;
}

Group add_ring(Group gr) {
    int node_id = find_node(&gr, CIRCLE, TRIANGLE);
    if (node_id < 0) return gr;
    Ring new_ring = Ring(gr.node_data.at(node_id)->m_position);
    // TODO move current node position
    // TODO replace node in edge connections and connect old node to new node
    return gr;
}

// root node is at position 0
// rules refer to root node and edge order

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Geometric Graph Builder");

    Circle test_circle = Circle(Vector2{250.f,250.f});
    Ring test_ring = Ring(Vector2{250.f,350.f});
    Triangle test_tri = Triangle(Vector2{350.f,250.f});
    Square test_square = Square(Vector2{350.f,350.f});

    Graph g = Graph{std::vector<Node *>{&test_circle, &test_ring, &test_tri, &test_square}, std::vector<Edge>{Edge{0,1}}};

    //double dt = 0.f;

    //size_t selected_atom = 0;

    while (!WindowShouldClose()) {
        //dt = GetFrameTime();
        //if (IsKeyReleased(KEY_SPACE)) break;

        BeginDrawing();
            ClearBackground(BLACK);
                
            for (auto nd : g.node_data)
                nd->draw();

            // TODO start edges closer to the other point (i.e. leave gap for sprite)
            for (auto ej : g.edge_data)
                DrawLineV(g.node_data.at(ej.a)->m_position, g.node_data.at(ej.b)->m_position, WHITE);

        EndDrawing();

    }
    CloseWindow();
    return 0;
}