/*
 We can't do it completely linearly because forces propagate along edges symmetrically 
 	(except at the fixed origin)

 Edges perfectly oppose any force which would move a node to more or less than the 
 	fixed edge length from a neighbour. Therefore:
 - Repulsion between neighbouring nodes is completely cancelled by the edge between them.
 - Each edge cancels the component of the force that runs parallel to the edge direction.
 
 Node positions can be expressed in angular, relative terms.
 
 Tension - transfer of force between nodes, or restorative force to edge angles?

 1) calculate repulsive forces between nodes (minus those directly cancelled by edges)
 2) calculate node momenta (p = mv, v1 = v0 - dt * a, a = f / m) where m is the sum of the 
 	mass of node and neighbours (the edges perfectly distribute the force over the neighbours)
 3) solve for tensile constraints (from edge lengths) to node velocities (transfer of momentum)
 	by adding the momentum of the nodes pushed on by tensile forces via edges.
 4) write new velocities, calculate and write new positions


 velocities sum along edges:
 0 --- 1 --- 2
 v2 = v2 + v1

 tensile forces result from the velocity of a node taking it out of the constraint area/length/point
	they perfectly cancel these movements, but the force is transferred on
 if we do
 1) calculate point on radius from velocity + neighbour relative pos
 2) define translational difference = difference in length
 3) angular difference is discarded
 4) translational difference is inherited as momentum
 we still need to find a way to compromise between contributions such that the function terminates

 is it better with springy bonds?
     e.g. node with v = (1., .5), pos = (0., 0.) 
 bonded to one with v = (0., 0.), pos = (2., 2.)
 would end up at (1., .5) after dt = 1
 if the bond length is fixed at 4,
 bond would push both nodes away from its centrepoint by 2.75
 
 centrepoint of edge should constrain both nodes to edge length from centrepoint
 but one node is fixed so the other gets double
 and then the forces should redirect around the constraints of the other edges of the nodes

 for layer:
 	for node:
		for edge: 
			if edge.other_end.layer == node.layer:
				

 */

typedef std::pair<size_t, size_t> Link;

typedef struct {
	float length;
	Link ids;
} Edge;

typedef struct {
	size_t layer;
	Vector2 pos;
	std::vector<Edge> edges;
} Node;

typedef std::unordered_map<size_t, Node> Graph;

Graph update_nodes(Graph g) {
	// find repel force for each node - O(n**2)
	// constrain force to edges via projection
	// 		project a f nbr = scale a . norm . (add f) . sub nbr
	// 			find vector from neighbour to node
	// 			add force vector to resulting vector
	// 			find vector from neighbour node to resulting vector
	// 			normalise vector and scale by edge length
	// note: multiply connected nodes must stay on the point of intersection of their neighbours
	// FIXME nodes should push back on their neighbours
	// apply constrained force to node pos
	for (auto [id,node] : g) {
		for (auto edge : node.edges) {

		}
	}
}
