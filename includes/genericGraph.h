#include <cstddef>
#include <vector>
#include <unordered_map>
#include <iostream>

template <typename DataType>
struct Node {
	DataType n_data;
	std::vector<Node*> edges;
};

template <typename DataType>
class NodeIterator : std::vector<Node*>::Iterator{
public:

	NodeIterator<DataType> next() {
		
		return NodeIterator<DataType>(++_ptr);
	}

	NodeIterator<DataType> prev() {
		return NodeIterator<DataType>(--_ptr, --_prev, --_next);
	}

protected:
	Node<DataType>* _ptr;
}

template <typename DataType>
class GraphIterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = Node<DataType>;
	using difference_type = std::ptr_diff_t;
	using pointer = std::ptr_t;
	using reference = Node<DataType>&;

public:
	GraphIterator(Graph<DataType>& graph = nullptr, size_t id = 0) {
		_id = id;
		_graph = graph;
	}

	GraphIterator(const GraphIterator<DataType> it) = default;

	~GraphIterator() {
	}

	GraphIterator<DataType>& operator=(const GraphIterator<DataType> it) = default;
	GraphIterator<DataType>& operator=(int id) {
		_id = id % _graph.size();
		return (*this);
	}

	operator bool() const {
        return (_id >= _graph.size()) ? true : false;
    }

    bool operator==(const GraphIterator<DataType> it) const {
    	return (_id == it.getConstID());
    }
    bool operator!=(const GraphIterator<DataType> it) const {
    	return (_id != it.getConstID());
    }

    // TODO step through neighbours
    GraphIterator<DataType>& operator+=(const difference_type& step) {
    	_id = (_id + step) % _graph.size();
    	return (*this);
    }
    GraphIterator<DataType>& operator-=(const difference_type& step) {
    	_id = (_id - step) % _graph.size();
    	return (*this);
    }

    GraphIterator<DataType>& operator++() {
    	if (_id == _graph.size() - 1) _id = 0;
    	else ++_id;
    	return (*this);
    }

    GraphIterator<DataType>& operator--() {
    	if (_id == 0) _id = _graph.size() - 1;
    	else --_id;
    	return (*this);
    }

    GraphIterator<DataType> operator++(int) {
    	auto temp(*this);
    	if (_id == _graph.size() - 1) _id = 0;
    	else ++_id;
    	return temp;
    }

    GraphIterator<DataType> operator--(int) {
    	auto temp = *this;
    	if (_id == 0) _id = _graph.size() - 1;
    	else --_id;
    	return temp;
    }

    GraphIterator<DataType> operator+(const difference_type& step) {
    	auto old = _id;
    	_id = (_id + step) % _graph.size();
    	auto temp = *this;
    	_id = old;
    	return temp;
    }

    GraphIterator<DataType> operator-(const difference_type& step) {
    	auto old = _id;
    	_id = (_id - step) % _graph.size();
    	auto temp = *this;
    	_id = old;
    	return temp;
    }

    difference_type operator-(const GraphIterator<DataType>& it) {
    	auto itid = it.getConstID();
    	return (itid > _id) ? _id + (_graph.size() - itid) : itid - _id;
    }

    Node<DataType>& operator*() {
    	return _graph.at(_id);
    }

    const Node<DataType>& operator*() const {
    	return _graph.at(_id);
    }

    Node<DataType>* operator->() {
    	return *_graph.at(_id);
    }

    size_t getID() const {
    	return _id;
    }

    const size_t getConstID() const {
    	return _id;
    }

    Node<DataType>* getPtr() const {
    	return *_graph.at(_id);
    }

    const Node<DataType>* getConstPtr() const {
    	return *_graph.at(_id);
    }

protected:
	size_t _id;
	Graph<DataType>& _graph;
};

template <typename DataType>
class GraphReverseIterator<DataType> : GraphIterator<DataType> {
public:
	GraphReverseIterator(Graph<DataType>& graph = nullptr, size_t id = 0) :
		GraphIterator<DataType>(graph, id) {
	}

	GraphReverseIterator(const GraphIterator<DataType>& it) {
		this._id = it.getID();
	}

	GraphReverseIterator(const GraphReverseIterator<DataType>& it) = default;

	~GraphReverseIterator(){
	}

	GraphReverseIterator<DataType>& operator=(const GraphReverseIterator<DataType>& it) = default;

	GraphReverseIterator<DataType>& operator=(const GraphIterator<DataType>& it) {
		this->_id = it.getID() % _graph.size();
		return (*this);
	}

	GraphReverseIterator<DataType>& operator=(size_t idx) {
		this->_id = idx % _graph.size();
		return (*this);
	}

	GraphReverseIterator<DataType>& operator+=(const difference_type& step) {
    	_id = (_id - step) % _graph.size();
    	return (*this);
    }

	GraphReverseIterator<DataType>& operator-=(const difference_type& step) {
    	_id = (_id + step) % _graph.size();
    	return (*this);
    }

	GraphReverseIterator<DataType>& operator++() {
    	if (_id == 0) _id = _graph.size() - 1;
    	else --_id;
    	return (*this);
    }

	GraphReverseIterator<DataType>& operator--() {
    	if (_id == _graph.size() - 1) _id = 0;
    	else ++_id;
    	return (*this);
    }

	GraphReverseIterator<DataType> operator++(int) {
    	auto temp = *this;
    	if (_id == 0) _id = _graph.size() - 1;
    	else --_id;
    	return temp;
    }

	GraphReverseIterator<DataType> operator--(int) {
    	auto temp(*this);
    	if (_id == _graph.size() - 1) _id = 0;
    	else ++_id;
    	return temp;
    }

	GraphReverseIterator<DataType> operator+(const difference_type& step) {
    	auto old = _id;
    	_id = (_id - step) % _graph.size();
    	auto temp = *this;
    	_id = old;
    	return temp;
    }

	GraphReverseIterator<DataType> operator-(const difference_type& step) {
    	auto old = _id;
    	_id = (_id + step) % _graph.size();
    	auto temp = *this;
    	_id = old;
    	return temp;
    }

	difference_type operator-(const GraphReverseIterator<DataType>& it) {
    	auto itid = it.getConstID();
    	return (itid <= _id) ? itid - _id : _id + (_graph.size() - itid);
    }

	GraphIterator<DataType> base() {
		GraphIterator<DataType> fwd = GraphIterator<DataType>(this->_id);
		++fwd;
		return fwd;
	}
};

template <typename DataType>
class Graph {
protected:
	std::unordered_map<Node<DataType>> _nodes;
public:
	typedef GraphIterator<DataType> iterator;
	typedef GraphIterator<const DataType> const_iterator;

	typedef GraphReverseIterator<DataType> reverse_iterator;
	typedef GraphReverseIterator<const DataType> const_reverse_iterator;

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

	friend std::ostream& operator<<(std::ostream& os, const Graph& g);

	iterator begin() {
		return iterator(this, 0);
	}

	iterator end() {
		return iterator(this, _nodes.size());
	}
	
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

	size_t size() {
		return _nodes.size();
	}

	DataType& at(size_t idx) {
		return _nodes[idx];
	}
};

std::ostream& Graph::operator<<(std::ostream& os, const Graph& g) {
	for (auto i : g) {
		os << "node at " << i->n_data << ", edges: ";
		for (auto n : i->edges)
			os << n << ", ";
		os << std::endl;
	}
	return os;
}