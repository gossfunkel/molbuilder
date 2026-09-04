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

class Node {
public:
    static size_t num_nodes;
    size_t m_uid;
    Vector2 m_position;

    Node(Vector2 position) : m_position {position} {
        m_uid = num_nodes++;
        std::cout << "Making node " << m_uid << ": ";
    }

    Node() {
        Node(Vector2Zero());
    }

    virtual Graph insert(Graph gr) {};
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

    Circle(Vector2 position) : Node{position} {
        std::cout << "circle." << std::endl;
    }

    Circle() : Node{} {
        std::cout << "circle." << std::endl;
    }

    virtual Graph insert(Graph gr) {
        // disallowed
    }

    virtual void draw() const {
        DrawCircleV(this->m_position, 5, this->m_col);
    }
};

class Ring : public Node {
public:
    size_t m_edges[2] {};
    const Color m_col {RED};

    Ring(Vector2 position) : Node{position} {
        std::cout << "ring." << std::endl;
    }

    Ring() : Node{} {
        std::cout << "ring." << std::endl;
    }

    virtual Graph insert(Graph gr) {
        // find [Circle or Square]
        // replace [Circle or Square] with self (in node_data)
        // update m_position and m_edges
        // create new Circle and attach to other edge of self
    }

    virtual void draw() const {
        DrawRing(this->m_position, 3, 6, 0, 360, 1, this->m_col);
    }
};

class Triangle : public Node {
public:
    size_t m_edges[3] {};
    const Color m_col {GREEN};

    Triangle(Vector2 position) : Node{position} {
        std::cout << "triangle." << std::endl;
    }

    Triangle() : Node{} {
        std::cout << "triangle." << std::endl;
    }

    virtual Graph insert(Graph gr) {
        // find [Circle or Square]
        // replace [Circle or Square] with self (in node_data)
        // update m_position and m_edges
        // create new Circle and attach to other edge of self
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

    Square(Vector2 position) : Node{position} {
        std::cout << "square." << std::endl;
    }

    Square() : Node{} {
        std::cout << "square." << std::endl;
    }

    virtual Graph insert(Graph gr) {
        // find [Circle or Square]
        // replace [Circle or Square] with self (in node_data)
        // update m_position and m_edges
        // create new Circle and attach to other edge of self
    }

    virtual void draw() const {
        DrawRectangle(this->m_position.x - 4.f, this->m_position.y - 4.f, 8, 8, this->m_col);
    }
};

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