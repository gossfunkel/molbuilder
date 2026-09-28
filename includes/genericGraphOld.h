#include <cstddef>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <iostream>

template <typename DataType>
struct Node {
	DataType n_data;
	size_t layer;
	//std::vector<Node<DataType>*> edges;
	std::vector<size_t> edges;
};

template <typename DataType>
class NodeIterator {
public:
	NodeIterator(std::vector<Node<DataType>&> peers, size_t idx) {
		_it = peers.at(idx);
		_prev = (idx == 0) 
		      ? peers.size() - 1
		      : idx - 1;
		_next = (idx == peers.size() - 1) 
		      ? 0
		      : idx + 1;
	}
	~NodeIterator() {}

	NodeIterator next() {
		return {_peers, _next};
	}

	NodeIterator prev() {
		return {_peers, _prev};
	}

protected:
	std::vector<Node<DataType>*> _peers;
	std::vector<Node<DataType>*>::Iterator _it;
	size_t _prev;
	size_t _next;
};

template <typename DataType>
class Graph {
protected:
	std::unordered_map<size_t, Node<DataType>> _nodes;
public:
	struct Iterator {
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = Node<DataType>;
		using pointer = value_type*;
		using reference = value_type&;

		Iterator(pointer ptr) : _ptr(ptr){}

		reference operator*() const {
			return *_ptr;
		}

		pointer operator->() {
			return _ptr;
		}

		Iterator& operator++() {
			++_ptr;
			return *this;
		}

		Iterator& operator--() {
			--_ptr;
			return *this;
		}

		Iterator operator++(int) {
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		Iterator operator--(int) {
			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		friend bool operator==(const Iterator& a, const Iterator& b) {
			return a._ptr == b._ptr;
		}

		friend bool operator!=(const Iterator& a, const Iterator& b) {
			return a._ptr != b._ptr;
		}
	protected:
		pointer _ptr;
	};

	Graph() {
	};
	Graph(Node<DataType> nd) {
		_nodes[0] = nd;
	}
	Graph(Node<DataType>& nd) {
		_nodes[0] = nd;
	}
	Graph(std::vector<Node<DataType>> nds) {
		for (size_t idx = 0; idx < nds.size(); ++idx)
			_nodes[idx] = nds.at(idx);
	}
	~Graph(){
	}

	friend std::ostream& operator<<(std::ostream& os, Graph& g) {
		for (auto i : g) {
			os << "node contains " << i.n_data << ", edges: ";
			for (auto n : i.edges)
				os << n << ", ";
			os << std::endl;
		}
		return os;
	}

	Iterator begin() {
		return Iterator(&_nodes[0]);
	}

	Iterator end() {
		return Iterator(&_nodes[_nodes.size()]);
	}
	
	/*
	const_iterator cbegin() {
		return const_iterator(this, 0);
	}

	const_iterator cend() {
		return const_iterator(this, _nodes.size());
	}

	reverse_iterator rbegin() {
		return reverse_iterator(this, _nodes.size()-1);
	}

	reverse_iterator rend() {
		return reverse_iterator(this, _nodes.size());
	}
	
	const_reverse_iterator crbegin() {
		return const_reverse_iterator(this, _nodes.size()-1);
	}

	const_reverse_iterator crend() {
		return const_reverse_iterator(this, _nodes.size());
	}
	*/

	size_t size() {
		return _nodes.size();
	}

	DataType& at(size_t idx) {
		return _nodes[idx];
	}

	NodeIterator<DataType> edges_of(Node<DataType>& nd) {
		//std::vector<Node<DataType>*> v;
		//for (auto nbr_id : nd.edges)
		//	v.emplace_back(_nodes[nbr_id]);
		return {nd.edges, 0};
	}

	void insert(size_t idx, Node<DataType> node) {
		size_t lowest_layer = 25565; // FIXME
		for (auto edge : node.edges) { 
			edge.edges.emplace_back(edge(idx));
			if (edge.layer < lowest_layer) lowest_layer = edge.layer;
		}
		node.layer = 1 + lowest_layer;
		_nodes[idx] = node;
	}

	void emplace_back(Node<DataType> node) {
		insert(_nodes.size(), node);
	}
};
