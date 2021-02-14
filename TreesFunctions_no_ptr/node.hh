#pragma once
#include "value.hh"

#include <limits>
#include <vector>

/**
 * @brief Représentation d'un noeud d'un arbre.
 * @see Tree
 */
class Tree;
class Node {
    public:
        /**
         * @brief Représentation d'un indice de tableau.
         */
        struct Index {
            private:
                Index() : _value(std::numeric_limits<size_t>::max()) {}
                Index(size_t value) : _value(value) {}

                inline bool isValid() const { return _value != std::numeric_limits<size_t>::max(); }
                inline size_t value() const { return _value; }
                inline void setValue(size_t value) { _value = value; }

                size_t _value;
                friend Tree;
        };

        /**
         * @brief Constructeur prenant les informations sur un noeud de l'arbre.
         * @param value Valeur du noeud.
         * @param index L'indice de ce noeud dans l'arbre.
         * @param parentIndex L'indice du parent de ce noeud dans l'arbre. Doit correspondre
         * à un indice invalide si le noeud ne possède pas de parent (racine).
         */
        Node(Value const & value, Index const & index, Index const & parentIndex)
            : _value(value), _index(index), _parentIndex(parentIndex), _indexesChildren() {}

        Node(Node const & node) = default;

        /**
         * @return La valeur du noeud.
         */
        Value const & value() const {
            return _value;
        }

        /**
         * @return La valeur du noeud.
         */
        Value & value() {
            return _value;
        }

        /**
         * @return L'indice du noeud dans l'arbre.
         */
        Index const & index() const {
            return _index;
        }

        /**
         * @return L'indice du noeud parent dans l'arbre.
         */
        Index const & parentIndex() const {
            return _parentIndex;
        }

        /**
         * @return L'ensemble des indices des enfants.
         */
        std::vector<Index> const & indexesChildren() const {
            return _indexesChildren;
        }

        /**
         * @return Le nombre d'enfants du noeud.
         */
        size_t childrenCount() const {
            return _indexesChildren.size();
        }

    private:
        /**
         * @brief Ajoute un enfant au noeud par l'intermédiaire de son indice.
         * @param index L'indice de l'enfant.
         */
        void addChildIndex(Index const & index) {
            _indexesChildren.push_back(index);
        }

        /// La valeur du noeud.
        Value _value;

        /// L'indice du noeud lui-même, dans l'arbre.
        Index _index;

        /// L'indice du noeud parent, dans l'arbre.
        Index _parentIndex;

        /// L'ensemble des indices des enfants.
        std::vector<Index> _indexesChildren;

        friend Tree;
};
