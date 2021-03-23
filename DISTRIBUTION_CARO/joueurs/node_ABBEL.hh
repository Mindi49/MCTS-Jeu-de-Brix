#pragma once
#include "value_ABBEL.hh"

#include <limits>
#include <vector>

/**
 * @brief Représentation d'un noeud d'un arbre.
 * @see Tree
 */
class Tree_ABBEL;
class Node_ABBEL {
    public:
        /**
         * @brief Représentation d'un indice de tableau.
         */
        struct Index {
            public:
                Index() : _value(std::numeric_limits<size_t>::max()) {}
            private:
                Index(size_t value) : _value(value) {}

                inline bool isValid() const { return _value != std::numeric_limits<size_t>::max(); }
                inline size_t value() const { return _value; }
                inline void setValue(size_t value) { _value = value; }

                size_t _value = std::numeric_limits<size_t>::max();
                friend Tree_ABBEL;
        };

        /**
         * @brief Constructeur prenant les informations sur un noeud de l'arbre.
         * @param value Valeur du noeud.
         * @param index L'indice de ce noeud dans l'arbre.
         * @param parentIndex L'indice du parent de ce noeud dans l'arbre. Doit correspondre
         * à un indice invalide si le noeud ne possède pas de parent (racine).
         */
        Node_ABBEL(Value_ABBEL const & value, Index const & index, Index const & parentIndex)
            : _value(value), _index(index), _parentIndex(parentIndex), _indexesChildren() {}

        Node_ABBEL(Node_ABBEL const & node) = default;

        /**
         * @return La valeur du noeud.
         */
        Value_ABBEL const & value() const {
            return _value;
        }

        /**
         * @return La valeur du noeud.
         */
        Value_ABBEL & value() {
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
        Value_ABBEL _value;

        /// L'indice du noeud lui-même, dans l'arbre.
        Index _index;

        /// L'indice du noeud parent, dans l'arbre.
        Index _parentIndex;

        /// L'ensemble des indices des enfants.
        std::vector<Index> _indexesChildren;

        friend Tree_ABBEL;
};
