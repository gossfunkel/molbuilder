#include "raylib.h"
#include "raymath.h"
#include "genericGraph.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define TAU M_PI*2.
#define EDGE_VEC Vector2{0.f,-15.f}

struct NodeData {
	Vector2 pos;
	Vector2 vel;
	Vector2 force;
	float size;
	Color col;

	friend std::ostream& operator<<(std::ostream& os, NodeData& nd) {
		os << "{pos:" << nd.pos.x << "'" << nd.pos.y 
		   << ", vel:" << nd.vel.x << "'" << nd.vel.y
		   << ", size:" << nd.size << "}";
		return os;
	}
};

using ND = Node<NodeData>;

// project a vector onto the line on which the other vector lies
#define project_onto(vx, vy) Vector2Scale(Vector2Normalize(vy), \
		 Vector2DotProduct((vx), Vector2Normalize(vy)))

// get the rejection vector 
// (component of vector on basis orthogonal to projection)
#define rejection_of(vx, vy) Vector2Subtract(vx, project_onto(vx, vy))

// constrain a movement of pos x by vx to its distance to pos y
#define constrain_to(vx, x, y) Vector2Subtract(Vector2Add( 	\
		Vector2Rotate(Vector2Subtract(x, y), 		\
		Vector2Length(vx)), x), y)
/*
 * constrain :: constraints -> (nodes -> nodes)
 * f(xs, ys, vxs, vys, dt)
 * position and velocities of all nodes is a function of all node 
 * 	positions and velocities, given for a set of constraints
 * so physics function must be constructed by constraint function,
 * 	or take some state that defines its parameters
 * 	i.e. f(xs, ys, vxs, vys, dt, constraints)
 */

void add_node(Graph<NodeData> *g, size_t attach_id) {
	ND *attach_nd = &(*g->at(attach_id));
	Vector2 attached_pos = attach_nd->n_data.pos;
	Vector2 new_pos = attached_pos;
	if (attach_nd->edges.size() > 0) {
		// find 'node north' (vector along first edge)
		Vector2 nnorth = Vector2Subtract( 
			g->at(attach_nd->edges.at(0))->n_data.pos,
			attached_pos);
		// rotate north edge around attach_node by TAU*n/n+1
		float n_edges = attach_nd->edges.size();
		//if (attach_id == 0) n_edges -= 1;
		new_pos = Vector2Add(Vector2Rotate(nnorth, TAU * 
				n_edges / (n_edges + 1.f)),
				attached_pos);
	} else new_pos = Vector2Add(attached_pos, EDGE_VEC);
	ND new_node = ND{NodeData{
		new_pos, 
		attach_nd->n_data.vel, 
		Vector2Zero(), 
		5.f, 
		SKYBLUE},
		attach_nd->layer + 1,
		std::vector<size_t>{}};
	g->attach_to(new_node, attach_id);
}

std::pair<NodeData, NodeData>
repel(std::pair<NodeData, NodeData> nd_pair) {
	Vector2 dist = Vector2Subtract(nd_pair.first.pos,
			nd_pair.second.pos);
	float f_mag = 2.f/Vector2Length(dist);
	/*std::cout << "repelling nodes at " 
		  << nd_pair.first.force.x << ", "
		  << nd_pair.first.force.y << " and "
		  << nd_pair.second.force.x << ", "
		  << nd_pair.second.force.y << ".\n";
	*/
	nd_pair.first.force  -= Vector2Scale(Vector2Normalize(dist),
						f_mag);
	nd_pair.second.force += Vector2Scale(Vector2Normalize(dist),
						f_mag);
	return nd_pair;
}

std::pair<NodeData, NodeData> 
constrain_nodes(std::pair<NodeData, NodeData> edge, double dt) {
	if(edge.first.vel != edge.second.vel) {
		// TODO balance between edges:
		// 	neighbour 2 could move a node after 
		// 		neighbour 1 constrained it
		// 	forces on an edge
		// 	fixed origin matches any applied force
		// 	edges cannot resize so nodes must have 
		// 		equal vel along edge (parallel)
		// 	if layers unequal, parallel forces go to
		// 		higher layer 
		edge.first.vel += Vector2Scale(edge.first.force, dt);
		edge.second.vel += Vector2Scale(edge.second.force, dt);
		// find parallel and orthogonal components of vels
		Vector2 e = Vector2Subtract(
			edge.second.pos, edge.first.pos);
		Vector2 edge_hat = Vector2Normalize(e);
		Vector2 e1v_p = Vector2Scale(edge_hat, 
			Vector2DotProduct(edge.first.vel, edge_hat));
		Vector2 e2v_p = Vector2Scale(edge_hat, 
			Vector2DotProduct(edge.second.vel, edge_hat));
		Vector2 e1v_o = Vector2Subtract(edge.first.vel, e1v_p);
		Vector2 e2v_o = Vector2Subtract(edge.second.vel, e2v_p);
		// sum parallel components
		Vector2 total_vel = Vector2Add(e1v_p, e2v_p);
		// project orthogonal to curve
		// TODO curve around barycentre/edge?
		edge.first.vel = Vector2Subtract(Vector2Add(
			Vector2Rotate(e, 180 + Vector2Length(e1v_o)), 
				edge.second.pos), edge.first.pos);
		edge.second.vel = Vector2Subtract(Vector2Add(
			Vector2Rotate(e, Vector2Length(e2v_o)), 
				edge.first.pos), edge.second.pos);
		edge.first.vel = Vector2Add(edge.first.vel, total_vel);
		edge.second.vel = Vector2Add(edge.second.vel, total_vel);
	}
	return edge;
}


int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graph tester");

	Graph<NodeData> g = {
		ND{NodeData{Vector2{(SCREEN_WIDTH/2.f), 
				     SCREEN_HEIGHT/2.f}, 
			Vector2Zero(), Vector2Zero(),
			5.f, GREEN},
		0, std::vector<size_t>()}
	};
	add_node(&g, 0);
	add_node(&g, 0);
	add_node(&g, 1);
	add_node(&g, 1);
	g.at(1)->n_data.vel = Vector2{10.f,0.f};

	//std::cout << "Initialised graph:\n" << g;

	double dt;
	while(!WindowShouldClose()) {
		dt = GetFrameTime();
		if(IsKeyReleased(KEY_P)) std::cout << g;
		
		// 1) each node repels all others + reverse
		g.cartesian_map(&repel);
		// 2) find node pressure on edges
		g.fmap(&press_edges);
		// 3) put combined forces on edges back onto nodes
		g.fmap_edges(&constrain_nodes, dt);
		// 4) observer follows pinned origin node
		Vector2 global_vel = g.at(0)->n_data.vel;
		g.fmap(+[](NodeData n_d, Vector2 global_vel){
			n_d.pos -= global_vel;
			return n_d;
		}, global_vel);
		// 5) move nodes by final vel
		g.fmap(+[](NodeData n_d, double dt) {
			n_d.pos += Vector2Scale(n_d.vel, dt);
			return n_d;
		}, dt);

		BeginDrawing();
		ClearBackground(BLACK);
		g.fmap(+[](NodeData n_d){
			DrawCircleV(n_d.pos, n_d.size, n_d.col); 
			return n_d;
		});
		for (auto [start, end] : g.get_unique_edges())
			DrawLineV(
				g.at(start)->n_data.pos,
				g.at(end)->n_data.pos,
				WHITE
			);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
