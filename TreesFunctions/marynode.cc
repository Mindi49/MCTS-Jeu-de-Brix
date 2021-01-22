#include "marynode.hh"
#include <iostream>

MaryNode::MaryNode(value v) : _value(v), _nodeSet(){
}


MaryNode::~MaryNode() {
    for (auto & i : _nodeSet)
        delete i;
    _nodeSet.clear();
}


value const & MaryNode::getValue() const {
    return _value;
}

void MaryNode::setValue(value const & v) {
    _value = v;
}


std::vector<MaryNode*> const & MaryNode::getNodeSet() const {
    return _nodeSet;
}


void MaryNode::addChild (MaryNode* child) {
    _nodeSet.push_back(child);
}


void MaryNode::display(int k) const {
    if (k > 0)
        std::cout << "|";
    for (int j = 0; j < k; j++)
        std::cout << "    ";
    if (k >= 0)
        std::cout << "|- ";

    printf("%2d\n", _value.eval);
    for (auto const & i : _nodeSet)
        i->display(k+1);
}


