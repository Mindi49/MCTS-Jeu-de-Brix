#include "tree.hh"
#include <iostream>
#include <iomanip>

Tree::Tree(size_t nodeCount) {
    _nodes.reserve(nodeCount > 0 ? nodeCount : 1);
}

Tree::Tree(Tree const & tree) : _nodes() {
    _nodes.reserve(tree._nodes.capacity());
    _nodes = tree._nodes;
}

Tree & Tree::operator= (Tree const & tree) {
    if (&tree != this) {
        _nodes.clear();
        _nodes.reserve(tree._nodes.capacity());
        _nodes = tree._nodes;
    }
    return *this;
}

void Tree::display() const {
    if (nodesCount() > 0) {
        displayNode(_nodes[0]);
    } else {
        std::cout << "Arbre vide" << std::endl;
    }
}

void Tree::displayNode(Node const & node, int k) const {
    if (k > 0)
        std::cout << "|";
    for (int j = 0; j < k; j++)
        std::cout << "    ";
    if (k >= 0)
        std::cout << "|- ";
    std::cout << std::setw(2) << node.value().eval << "\n";
    for (Node::Index const & i : node.indexesChildren()) {
        displayNode(getNodeFromIndex(i), k + 1);
    }
}
