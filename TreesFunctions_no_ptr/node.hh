#pragma once
#include <vector>
#include <limits>

/**
 * @brief Informations conservées dans le noeud.
 */
struct Value {
    int eval;
};
class Tree;

/**
 * @brief Représentation d'un noeud d'un arbre.
 * @see Tree
 */
class Node {


    public:
        /**
         * @brief Représentation d'un indice de tableau.
         */
        struct Index {
            private:
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
         */
        Node(Value const & value, Index const & index);

        Node(Node const & node) = default;

        /**
         * @return La valeur du noeud.
         */
        Value const & value() const;

        /**
         * @brief Définit la valeur du noeud.
         */
        void setValue(Value const & value);

        /**
         * @return L'indice du noeud dans l'arbre.
         */
        Index const & index() const;

        /**
         * @return L'ensemble des indices des enfants.
         */
        std::vector<Index> const & indexesChildren() const;

        /**
         * @return Le nombre d'enfants du noeud.
         */
        size_t childrenCount() const;

    private:
        /**
         * @brief Ajoute un enfant au noeud par l'intermédiaire de son indice.
         * @param index L'indice de l'enfant.
         */
        void addChildIndex(Index const & index);

        /// La valeur du noeud.
        Value _value;

        /// L'indice du noeud lui-même, dans l'arbre.
        Index _index;

        /// L'ensemble des indices des enfants.
        std::vector<Index> _indexesChild;

    friend Tree;
};
