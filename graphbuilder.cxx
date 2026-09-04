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
        DrawRing(this->m_position, 3, 6, 0, 360, 1, this->m_col);
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

    Square(size_t uid, Vector2 position) : Node{uid, position} {
        std::cout << "Making a square.\n" << std::endl;
    }

    virtual void draw() const {
        DrawRectangle(this->m_position.x - 4.f, this->m_position.y - 4.f, 8, 8, this->m_col);
    }
};

// explore_graph

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Geometric Graph Builder");

    Circle test_circle = Circle(0, Vector2{250.f,250.f});
    Ring test_ring = Ring(1, Vector2{250.f,350.f});
    Triangle test_tri = Triangle(2, Vector2{350.f,250.f});
    Square test_square = Square(3, Vector2{350.f,350.f});

    Graph g = Graph{std::vector<Node *>{&test_circle, &test_ring, &test_tri, &test_square}, std::vector<Edge>{}};

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