#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <ranges>
#include <unordered_map>

#define TAU (M_PI*2.f)

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define CURSOR_COL Color{100, 200, 20, 120}

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
    const size_t num_neighbours;
public:
    static size_t num_nodes;
    size_t m_uid;
    Vector2 m_position;
    std::vector<size_t> m_neighbours;

    Node(size_t nt) : nodetype {NodeType(nt)}, num_neighbours {nt}, m_position {Vector2{0.,0.}} {
        m_uid = num_nodes++;
        std::cout << "Making node " << m_uid << ": ";
    }

    Node(Vector2 position, size_t nt) : nodetype {NodeType(nt)}, num_neighbours {nt}, m_position {position} {
        m_uid = num_nodes++;
        std::cout << "Making node " << m_uid << ": ";
    }

    Node(Node &&) = default;

    Node(const Node &) = default;

    Node& operator=(Node &other) {
        Node(other.m_position, other.getNumNbrs());
        return *this;
    }

    Node& operator=(Node &&other) {
        Node(other.m_position, other.getNumNbrs());
        m_uid = other.m_uid;
        m_neighbours = std::move(other.m_neighbours);
        return *this;
    }

    inline NodeType getNodeType() const {
        return nodetype;
    };

    inline size_t getNumNbrs() const {
        return num_neighbours;
    };
};

size_t Node::num_nodes = 0;

typedef std::unordered_map<size_t, Node> NodeData;
typedef std::vector<std::pair<size_t,size_t>> NbrPairs;

typedef struct Graph {
    NodeData node_data;
    NbrPairs all_nbrs;
} Graph;

// TODO can i use std::views::join / std::views::join_with or suchlike?
NbrPairs refresh_all_nbrs(NodeData node_data) {
    NbrPairs new_pairs;
    for (const auto& [uid, nd] : node_data)
        for (auto nb : nd.m_neighbours) new_pairs.emplace_back(nd.m_uid, nb);
    std::ranges::sort(new_pairs);
    const auto rm = std::ranges::unique(new_pairs);
    new_pairs.erase(rm.begin(), rm.end());
    return new_pairs;
}

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

NodeData add_ring(NodeData n_d, int node_id) {
    if (node_id < 0 || node_id >= (int)n_d.size()) return n_d;
    Node *selected_node = &n_d.at(node_id);
    Node *selected_nbr  = &n_d.at(selected_node->m_neighbours.at(0));
    if (selected_node->getNodeType() != NodeType{CIRCLE}) return n_d;
    Ring new_ring = Ring(selected_node->m_position);
    std::cout << "Selected node " << node_id << " with neighbour " 
              << selected_node->m_neighbours.at(0) << " replaced by ring " 
              << new_ring.m_uid << "." << std::endl;

    // move current node position opposite node at other end of edge
    Vector2 diffPos = Vector2Subtract(
        selected_node->m_position,
        selected_nbr->m_position
    );
    selected_node->m_position = Vector2Add(selected_node->m_position, diffPos);

    // add neighbours 
    new_ring.m_neighbours.emplace_back(selected_node->m_neighbours.at(0));
    new_ring.m_neighbours.emplace_back(node_id);
    n_d.emplace(new_ring.m_uid, new_ring);
    // FIXME detect if this is this neighbour or other side
    selected_node->m_neighbours.at(0) = new_ring.m_uid;
    if (selected_nbr->getNodeType() == NodeType{RING})
        selected_nbr->m_neighbours.at(1) = new_ring.m_uid;
    else if (selected_nbr->getNodeType() == NodeType{CIRCLE}) 
        selected_nbr->m_neighbours.at(0) = new_ring.m_uid;
    return n_d;
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

NodeData add_square(NodeData n_d, int node_id) {
    if (node_id < 0 || node_id >= (int)n_d.size()) return n_d;
    Node *selected_node = &n_d.at(node_id);
    Node *selected_nbr  = &n_d.at(selected_node->m_neighbours.at(0));
    if (selected_node->getNodeType() != NodeType{RING}
     || selected_nbr->getNodeType() != NodeType{RING}) return n_d;
    Square new_square = Square(selected_node->m_position);
    std::cout << "Selected node " << node_id << " with neighbour " 
              << selected_node->m_neighbours.at(0) << " replaced by square " 
              << new_square.m_uid << "." << std::endl;

    Vector2 diffPos = Vector2Subtract(
        n_d.at(node_id).m_position,
        n_d.at(selected_node->m_neighbours.at(0)).m_position
    );
    // positions of 2 unfilled neighbours
    Circle top_circle = Circle(Vector2Add(n_d.at(node_id).m_position, Vector2Rotate(diffPos, 90)));
    top_circle.m_neighbours.emplace_back(new_square.m_uid);
    Circle btm_circle = Circle(Vector2Add(n_d.at(node_id).m_position, Vector2Rotate(diffPos, -90)));
    btm_circle.m_neighbours.emplace_back(new_square.m_uid);

    // replace node in edge connection and make new edge for old node
    new_square.m_neighbours.emplace_back(top_circle.m_uid);
    new_square.m_neighbours.emplace_back(btm_circle.m_uid);
    // FIXME detect if this is this neighbour or other side
    new_square.m_neighbours.emplace_back(selected_node->m_neighbours.at(0));
    new_square.m_neighbours.emplace_back(selected_node->m_neighbours.at(1));
    n_d.at(selected_node->m_neighbours.at(0)).m_neighbours.at(1) = new_square.m_uid;
    n_d.at(selected_node->m_neighbours.at(1)).m_neighbours.at(0) = new_square.m_uid;
    n_d.erase(node_id);
    n_d.emplace(new_square.m_uid, new_square);
    n_d.emplace(top_circle.m_uid, top_circle);
    n_d.emplace(btm_circle.m_uid, btm_circle);
    return n_d;
}

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

    Graph g = Graph{NodeData{}, NbrPairs{}};
    g.node_data.emplace(0, Circle(Vector2{SCREEN_WIDTH/2.f - 50.f, SCREEN_HEIGHT/2.f}));
    g.node_data.emplace(1, Circle(Vector2{SCREEN_WIDTH/2.f + 50.f, SCREEN_HEIGHT/2.f}));
    g.node_data.at(0).m_neighbours.emplace_back(1);
    g.node_data.at(1).m_neighbours.emplace_back(0);
    g.all_nbrs = refresh_all_nbrs(g.node_data);

    double dt = 0.f;

    NodeType create_node_type = {RING};
    bool hit = false;
    int selected_node = {-1};
    Vector2 mousePos = {0.,0.};

    size_t cursor_state = {0};
    size_t cursor_dest = {1};
    float cursor_progress = {0.f};

    while (!WindowShouldClose()) {
        dt = GetFrameTime();
        cursor_progress += dt;

        if (cursor_progress > 1.f) {
            cursor_state = cursor_dest;
            Node *cursor_node = &g.node_data.at(cursor_dest);
            if (cursor_node->getNumNbrs() > 1)
                cursor_dest = cursor_node->m_neighbours.at(GetRandomValue(0, cursor_node->getNumNbrs()-1));
            else cursor_dest = cursor_node->m_neighbours.at(0);
            cursor_progress = 0.f;
        }

        //if (IsKeyReleased(KEY_SPACE)) break;
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
            switch (create_node_type) {
            case NodeType{RING}:
                g.node_data = add_ring(g.node_data, selected_node);
                break;
            case NodeType{TRIANGLE}:
                //g = add_tri(g, selected_node);
                std::cout << "Adding triangle not yet implemented." << std::endl;
                break;
            case NodeType{SQUARE}:
                g.node_data = add_square(g.node_data, selected_node);
                //std::cout << "Adding square not yet implemented." << std::endl;
                break;
            default:
                std::cerr << "Undefined node type selected!" << std::endl;
            }
            g.all_nbrs = refresh_all_nbrs(g.node_data);
        }

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

            // TODO replace edges with neighbours
            // TODO start edges closer to the other point (i.e. leave gap for sprite)
            
            for (auto& [first, second] : g.all_nbrs) 
                DrawLineV(g.node_data.at(first).m_position, g.node_data.at(second).m_position, WHITE);
            
            DrawText(std::to_string(cursor_state).c_str(), SCREEN_WIDTH-40, 30, 18, GOLD);
            DrawText(std::to_string(cursor_dest).c_str(),  SCREEN_WIDTH-40, 60, 18, GOLD);
            DrawCircleV(Vector2Lerp(g.node_data.at(cursor_state).m_position, g.node_data.at(cursor_dest).m_position, cursor_progress), 10, CURSOR_COL);

        EndDrawing();

    }
    CloseWindow();
    return 0;
}