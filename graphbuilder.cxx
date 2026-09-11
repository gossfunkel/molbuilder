#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>

#define TAU (M_PI*2.f)

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

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
    UNDEF,
    CIRCLE,
    RING,
    TRIANGLE,
    SQUARE
} NodeType;

class Node {
protected:
    const NodeType nodetype;
    const size_t num_edges;
public:
    static size_t num_nodes;
    size_t m_uid;
    Vector2 m_position;
    std::vector<size_t> m_edges;

    Node(size_t nt) : nodetype {NodeType(nt)}, num_edges {nt}, m_position {Vector2{0.,0.}} {
        m_uid = num_nodes++;
        std::cout << "Making node " << m_uid << ": ";
    }

    Node(Vector2 position, size_t nt) : nodetype {NodeType(nt)}, num_edges {nt}, m_position {position} {
        m_uid = num_nodes++;
        std::cout << "Making node " << m_uid << ": ";
    }

    inline NodeType getNodeType() const {
        return nodetype;
    };

    inline size_t getNumEdges() const {
        return num_edges;
    };
};

size_t Node::num_nodes = 0;

typedef struct Graph {
    std::vector<Node> node_data;
    std::vector<Edge> edge_data;
} Graph;

// leaf node
class Circle : public Node {
public:
    Circle(Vector2 position) : Node(position, 1) {
        std::cout << "circle." << std::endl;
    }

    Circle() : Node(1) {
        std::cout << "circle." << std::endl;
    }
};

class Ring : public Node {
public:
    Ring(Vector2 position) : Node(position, 2) {
        std::cout << "ring." << std::endl;
    }

    Ring() : Node(2) {
        std::cout << "ring." << std::endl;
    }
};

class Triangle : public Node {
public:
    Triangle(Vector2 position) : Node(position, 3) {
        std::cout << "triangle." << std::endl;
    }

    Triangle() : Node(3) {
        std::cout << "triangle." << std::endl;
    }
};

class Square : public Node {
public:
    Square(Vector2 position) : Node(position, 4) {
        std::cout << "square." << std::endl;
    }

    Square() : Node(4) {
        std::cout << "square." << std::endl;
    }
};

// TODO use std::pairs / std::ranges::views::pairwise instead of the edges thing?

Graph add_ring(Graph gr, int node_id) {
    if (node_id < 0 || node_id >= (int)gr.node_data.size()) return gr;
    Node *selected_node = &gr.node_data.at(node_id);
    if (selected_node->getNodeType() != NodeType{CIRCLE}) return gr;
    Ring new_ring = Ring(selected_node->m_position);

    Edge *replaced_edge    = &gr.edge_data.at(gr.node_data.at(node_id).m_edges.at(0));
    size_t *edge_this_end  = ((int)(replaced_edge->a) == node_id) ? &replaced_edge->a : &replaced_edge->b;
    size_t *edge_other_end = ((int)(replaced_edge->a) == node_id) ? &replaced_edge->b : &replaced_edge->a;

    // move current node position opposite node at other end of edge
    Vector2 diffPos = Vector2Subtract(
        gr.node_data.at(node_id).m_position,
        gr.node_data.at(*edge_other_end).m_position
    );
    gr.node_data.at(node_id).m_position = Vector2Add(gr.node_data.at(node_id).m_position, diffPos);

    // replace node in edge connection and make new edge for old node
    new_ring.m_edges.emplace_back(gr.node_data.at(node_id).m_edges.at(0));
    *edge_this_end = new_ring.m_uid;
    gr.edge_data.emplace_back(Edge{new_ring.m_uid, node_id});
    new_ring.m_edges.emplace_back(gr.edge_data.size() - 1);
    gr.node_data.at(node_id).m_edges.at(0) = new_ring.m_edges.at(1);
    gr.node_data.emplace_back(new_ring);
    return gr;
}

/*Graph add_tri(Graph gr, int node_id) {
    if (node_id < 0 || node_id >= (int)gr.node_data.size()) return gr;
    Node *selected_node = &gr.node_data.at(node_id);
    if (selected_node->getNodeType() != NodeType{CIRCLE} && selected_node->getNodeType() != NodeType{RING}) return gr;
    Triangle new_tri = Triangle(selected_node->m_position);

    Edge *replaced_edge_i    = &gr.edge_data.at(gr.node_data.at(node_id).m_edges.at(0));
    size_t *edge_i_this_end  = ((int)(replaced_edge_i->a) == node_id) ? &replaced_edge_i->a : &replaced_edge_i->b;
    size_t *edge_i_other_end = ((int)(replaced_edge_i->a) == node_id) ? &replaced_edge_i->b : &replaced_edge_i->a;

    if (selected_node->getNodeType() == NodeType{RING}) {
        Edge *replaced_edge_j    = &gr.edge_data.at(gr.node_data.at(node_id).m_edges.at(0));
        size_t *edge_j_this_end  = ((int)(replaced_edge_j->a) == node_id) ? &replaced_edge_j->a : &replaced_edge_j->b;
        size_t *edge_j_other_end = ((int)(replaced_edge_j->a) == node_id) ? &replaced_edge_j->b : &replaced_edge_j->a;
    }

    // TODO move current node position to ... 
    Vector2 diffPos = Vector2Subtract(
        gr.node_data.at(node_id).m_position,
        gr.node_data.at(*edge_i_other_end).m_position
    );
    gr.node_data.at(node_id).m_position = Vector2Add(gr.node_data.at(node_id).m_position, diffPos);

    // replace node in edge connection and make new edge for old node
    new_ring.m_edges.emplace_back(gr.node_data.at(node_id).m_edges.at(0));
    *edge_this_end = new_ring.m_uid;
    gr.edge_data.emplace_back(Edge{new_ring.m_uid, node_id});
    new_ring.m_edges.emplace_back(gr.edge_data.size() - 1);
    gr.node_data.at(node_id).m_edges.at(0) = new_ring.m_edges.at(1);
    gr.node_data.emplace_back(new_ring);
    return gr;
}*/

/*
Graph add_square(Graph gr, int node_id) {
    if (node_id < 0 || node_id >= (int)gr.node_data.size()) return gr;
    Node *selected_node = &gr.node_data.at(node_id);
    if (selected_node->getNodeType() != NodeType{RING}) return gr;
    if (gr.edge_data.at(selected_node->m_edges.at(0)).a)getNodeType() != NodeType{RING}) return gr;
    Square new_square = Square(selected_node->m_position);

    Edge *replaced_edge_i    = &gr.edge_data.at(gr.node_data.at(node_id).m_edges.at(0));
    size_t *edge_i_this_end  = ((int)(replaced_edge_i->a) == node_id) ? &replaced_edge_i->a : &replaced_edge_i->b;
    size_t *edge_i_other_end = ((int)(replaced_edge_i->a) == node_id) ? &replaced_edge_i->b : &replaced_edge_i->a;

    if (selected_node->getNodeType() == NodeType{RING}) {
        Edge *replaced_edge_j    = &gr.edge_data.at(gr.node_data.at(node_id).m_edges.at(0));
        size_t *edge_j_this_end  = ((int)(replaced_edge_j->a) == node_id) ? &replaced_edge_j->a : &replaced_edge_j->b;
        size_t *edge_j_other_end = ((int)(replaced_edge_j->a) == node_id) ? &replaced_edge_j->b : &replaced_edge_j->a;
    }

    Vector2 diffPos = Vector2Subtract(
        gr.node_data.at(node_id).m_position,
        gr.node_data.at(*edge_i_other_end).m_position
    );
    // 3 positions of square neighbours
    gr.node_data.at(node_id).m_position = Vector2Add(gr.node_data.at(node_id).m_position, diffPos);
    gr.node_data.at(node_id).m_position = Vector2Add(gr.node_data.at(node_id).m_position, Vector2Rotate(diffPos, 90));
    gr.node_data.at(node_id).m_position = Vector2Add(gr.node_data.at(node_id).m_position, Vector2Rotate(diffPos, -90));

    // replace node in edge connection and make new edge for old node
    new_ring.m_edges.emplace_back(gr.node_data.at(node_id).m_edges.at(0));
    *edge_this_end = new_ring.m_uid;
    gr.edge_data.emplace_back(Edge{new_ring.m_uid, node_id});
    new_ring.m_edges.emplace_back(gr.edge_data.size() - 1);
    gr.node_data.at(node_id).m_edges.at(0) = new_ring.m_edges.at(1);
    gr.node_data.emplace_back(new_ring);
    return gr;
}*/

void draw_node(Node &nd) {
    switch (nd.getNodeType()) {
    case NodeType{CIRCLE}:
        DrawCircleV(nd.m_position, 5, SKYBLUE);
        break;
    case NodeType{RING}:
        DrawRing(nd.m_position, 3, 6, 0, 360, 1, YELLOW);
        break;
    case NodeType{TRIANGLE}:
        DrawTriangle(
            Vector2Add(nd.m_position, Vector2{ 0.f, -4.f}),
            Vector2Add(nd.m_position, Vector2{-5.f,  4.f}),
            Vector2Add(nd.m_position, Vector2{ 5.f,  4.f}),
            GREEN);
        break;
    case NodeType{SQUARE}:
        DrawRectangle(nd.m_position.x - 4.f, nd.m_position.y - 4.f, 8, 8, PURPLE);
        break;
    default:
        DrawCircleLinesV(nd.m_position, 3, RED);
    };
}

// root node is at position 0
// rules refer to root node and edge order

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Geometric Graph Builder");

    //Circle test_circle = Circle(Vector2{250.f,250.f});
    //Ring test_ring = Ring(Vector2{250.f,350.f});
    //Triangle test_tri = Triangle(Vector2{350.f,250.f});
    //Square test_square = Square(Vector2{350.f,350.f});

    //Graph g = Graph{std::vector<Node *>{&test_circle, &test_ring, &test_tri, &test_square}, std::vector<Edge>{Edge{0,1}}};

    Circle starter[2] = {
        Circle(Vector2{250.f,300.f}),
        Circle(Vector2{350.f,300.f})
    };

    Graph g = Graph{std::vector<Node>{starter[0], starter[1]}, std::vector<Edge>{Edge{0,1}}};
    g.node_data.at(0).m_edges.emplace_back(0);
    g.node_data.at(1).m_edges.emplace_back(0);

    //double dt = 0.f;

    NodeType create_node_type = {RING};
    bool hit = false;
    int selected_node = {-1};
    Vector2 mousePos = {0.,0.};

    while (!WindowShouldClose()) {
        //dt = GetFrameTime();
        //if (IsKeyReleased(KEY_SPACE)) break;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            mousePos = GetMousePosition();
            for (auto nd : g.node_data)
                if (CheckCollisionPointCircle(mousePos, nd.m_position, 10)) {
                    selected_node = nd.m_uid;
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
            switch (create_node_type) {
            case NodeType{RING}:
                g = add_ring(g, selected_node);
                break;
            case NodeType{TRIANGLE}:
                //g = add_tri(g, selected_node);
                std::cout << "Adding triangle not yet implemented." << std::endl;
                break;
            case NodeType{SQUARE}:
                //g = add_square(g, selected_node);
                std::cout << "Adding square not yet implemented." << std::endl;
                break;
            default:
                std::cerr << "Undefined node type selected!" << std::endl;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
                
            for (Node nd : g.node_data) draw_node(nd);

            if (selected_node >= 0) DrawCircleLinesV(g.node_data.at(selected_node).m_position, 7, WHITE);

            // TODO start edges closer to the other point (i.e. leave gap for sprite)
            for (auto ej : g.edge_data)
                DrawLineV(g.node_data.at(ej.a).m_position, g.node_data.at(ej.b).m_position, WHITE);

        EndDrawing();

    }
    CloseWindow();
    return 0;
}