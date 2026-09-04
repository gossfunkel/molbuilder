#include "raylib.h"
#include "raymath.h"
#include <iostream>
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

    Node(size_t uid, Vector2 position) : m_uid {uid}, m_position {position} {
        std::cout << "Making a node.\n" << std::endl;
    }

    //virtual Graph insert();
    virtual void draw() const {};
};

typedef struct Graph {
    std::vector<Node *> node_data;
    std::vector<Edge> edge_data;
} Graph;

class Circle : public Node {
public:
    size_t m_edges[1];
    const Color m_col {BLUE};

    Circle(size_t uid, Vector2 position) : Node{uid, position} {
        std::cout << "Making a circle.\n" << std::endl;
    }

    virtual void draw() const {
        DrawCircleV(this->m_position, 5, this->m_col);
    }
};

class Ring : public Node {
public:
    size_t m_edges[2] {};
    const Color m_col {RED};

    Ring(size_t uid, Vector2 position) : Node{uid, position} {
        std::cout << "Making a ring.\n" << std::endl;
    }

    virtual void draw() const {
        DrawRing(this->m_position, 2, 4, 0, 0, 1, this->m_col);
    }
};

class Triangle : public Node {
public:
    size_t m_edges[3] {};
    const Color m_col {GREEN};

    Triangle(size_t uid, Vector2 position) : Node{uid, position} {
        std::cout << "Making a triangle.\n" << std::endl;
    }

    virtual void draw() const {
        DrawTriangle(
            Vector2Add(this->m_position, Vector2{0.f,-2.f}),
            Vector2Add(this->m_position, Vector2{2.f,0.f}),
            Vector2Add(this->m_position, Vector2{-2.f,0.f}),
            this->m_col);
    }
};

class Square : public Node {
public:
    size_t m_edges[4] {};
    const Color m_col {ORANGE};

    Square(size_t uid, Vector2 position) : Node{uid, position} {
        std::cout << "Making a square.\n" << std::endl;
    }

    virtual void draw() const {
        DrawRectangle(this->m_position.x - 1.f, this->m_position.y - 1.f, 2, 2, this->m_col);
    }
};

// explore_graph

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Geometric Graph Builder");

    Circle test_circle = Circle(0, Vector2{300.f,300.f});

    Graph g = Graph{std::vector<Node *>{&test_circle}, std::vector<Edge>{}};

    //double dt = 0.f;

    //size_t selected_atom = 0;

    while (!WindowShouldClose()) {
        //dt = GetFrameTime();
        //if (IsKeyReleased(KEY_SPACE)) break;

        BeginDrawing();
            ClearBackground(BLACK);
                
            for (auto nd : g.node_data)
                nd->draw();

        EndDrawing();

    }
    CloseWindow();
    return 0;
}