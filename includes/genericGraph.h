#include <cstddef>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <ranges>
#include <iostream>

template <typename DataType>
struct Node {
	DataType n_data;
	size_t layer;
	//std::vector<Node<DataType>*> edges;
	std::vector<size_t> edges;
};

/*
 * Random-access graph with dijkstra steps encoded in node storage.
 * Each node has a unique identifier for the use of hashmaps.
 * Nodes are kept in buckets called 'layers', numbered according to
 * 	the number of steps to node 0. 
 * Which layer a node is stored in is saved in the _layer_pos field.
 */
template <typename DataType>
class Graph {
	using Layer = std::unordered_map<size_t, Node<DataType>>;

	struct Iterator {
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = size_t;
		using value_type = Node<DataType>;
		using pointer = size_t;
		using reference = value_type&;

		Iterator(Graph *g, pointer id) : _g(g), _id(id){}

		reference operator*() const {
			size_t layer = _g->_layer_pos[_id];
			return _g->_ndata.at(layer)[_id];
		}

		Node<DataType>* operator->() {
			return &(*_g->at(_id));
		}

		Iterator& operator++() {
			++_id;
			return *this;
		}

		Iterator& operator--() {
			--_id;
			return *this;
		}

		Iterator operator++(int) {
			Iterator tmp = *this;
			++_id;
			return tmp;
		}

		Iterator operator--(int) {
			Iterator tmp = *this;
			--_id;
			return tmp;
		}

		friend bool operator==(const Iterator& a, const Iterator& b) {
			return a._id == b._id;
		}

		friend bool operator!=(const Iterator& a, const Iterator& b) {
			return a._id != b._id;
		}

	protected:
		Graph *_g;
		pointer _id;
	};
public:

	Graph() = default;

	/*
	 * Recommended: construct a graph with origin node defined.
	 */
	Graph(Node<DataType> nd) {
		nd.layer = 0;
		_ndata.emplace_back(Layer());
		_ndata.at(0)[0] = nd;
		_layer_pos[0] = 0;
	}

	/*
	Graph(std::vector<Node<Datatype>> v) {
		Graph(v.at(0));
		_layers.emplace_back(Layer());
		for (auto nbr : _ndata.at(0)[0].edges) {
			_layer_pos[nbr] = 1;
			_ndata.at(1)[nbr] = v.at(nbr);
		}
		// TODO then recurse on neighbours
	}
	*/

	Iterator begin() {
		return {this, 0};
	}

	Iterator end() {
		return {this, _layer_pos.size()};
	}

	Iterator cbegin() const {
		return {this, 0};
	}

	Iterator cend() const {
		return {this, _layer_pos.size()};
	}

	Iterator at(size_t node_id) {
		if (_layer_pos.contains(node_id))
			return {this, node_id};
		else return this->end();
	}

	Iterator at(size_t layer, size_t node_id) {
		if (_ndata.size() <= layer) return this->end();
		if (_ndata.at(layer).contains(node_id))
			return {this, node_id};
		else return this->end();
	}

	/*
	 * Apply a function equally to each member of the graph
	 */
	void fmap(DataType (*f)(DataType x)) {
		for (auto lyr : _ndata) 
			for (auto [id,nd] : lyr)
				nd.n_data = f(nd.n_data);
	}

	/*
	 * Apply a function equally to each edge in the graph
	 * (only applies once per edge).
	 */
	void fmap_edges(std::pair<DataType, DataType> 
			(*f)(std::pair<DataType, DataType>)) {
		for (auto [x,y] : get_unique_edges()) {
			std::pair<DataType, DataType> res =
				f(x,y);
			at(x)->n_data = res.first;
			at(y)->n_data = res.second;
		}
	}

	void insert(Node<DataType> nd) {
		size_t new_id = _layer_pos.size();
		nd.layer = 25565; // FIXME
		Node<DataType> *nbr = nullptr;
		for (auto eg : nd.edges) {
			nbr = &_ndata.at(_layer_pos[eg])[eg];
			nbr->edges.emplace_back(new_id);
			if (nbr->layer < nd.layer) nd.layer = nbr->layer;
		}
		nd.layer++;
		_layer_pos[new_id] = nd.layer;
		if (_ndata.size() <= nd.layer) _ndata.emplace_back(Layer());
		_ndata.at(nd.layer)[new_id] = nd;
	}

	void attach_to(Node<DataType> nd, size_t id) {
		if (!_layer_pos.contains(id)) return;
		size_t new_id = _layer_pos.size();
		nd.layer = _layer_pos[id] + 1;
		if(_ndata.size() <= nd.layer) _ndata.emplace_back(Layer());
		_ndata.at(_layer_pos[id])[id].edges.emplace_back(new_id);
		nd.edges.emplace_back(id);
		_layer_pos[new_id] = nd.layer;
		_ndata.at(nd.layer)[new_id] = nd;
	}

	/*
	 * Obtain a vector of pairs of IDs for each edge-
	 * removes duplicates from edge symmetry.
	 */
	std::vector<std::pair<size_t, size_t>> get_unique_edges() {
		std::vector<std::pair<size_t, size_t>> v;
		for (auto [id, lyr] : _layer_pos)
			for (auto e : _ndata.at(lyr)[id].edges)
				v.emplace_back(
					std::pair<size_t, size_t>(id, e)
				);
		std::ranges::sort(v);
		const auto rm = std::ranges::unique(v);
		v.erase(rm.begin(), rm.end());
		return v;
	}
	
	/*
	 * Print a debug/log description of the graph to a stream.
	 */
	friend std::ostream& operator<<(std::ostream& os, Graph& g) {
		for (auto i : g) {
			os << "layer " << i.layer << ": "
			   << "node contains " << i.n_data << ", edges: ";
			for (auto n : i.edges)
				os << n << ", ";
			os << std::endl;
		}
		return os;
	}

	protected:
		std::vector<Layer> _ndata; // raw data
		// lookup table for layer bucket / dijsktra value:
		std::unordered_map<size_t, size_t> _layer_pos;
};
