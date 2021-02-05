#include "node.hh"
#include "tree.hh"

#include <iostream>

Node::Node(Value const & value, Index const & index) : _value(value), _index(index), _indexesChild() {}

Value const & Node::value() const {
    return _value;
}

void Node::setValue(Value const & value) {
   _value = value;
}

Node::Index const & Node::index() const {
    return _index;
}

std::vector<Node::Index> const & Node::indexesChildren() const {
    return _indexesChild;
}

size_t Node::childrenCount() const {
    return indexesChildren().size();
}

void Node::addChildIndex(Index const & index) {
    _indexesChild.push_back(index);
}
