#pragma once
#include "node.hh"
#include "value.hh"

#include <iostream>
#include <stdexcept>
#include <utility>

/**
 * @brief Représentation d'un arbre n-aire.
 * Cette classe contient les informations de la racine de l'arbre.
 */
class Tree {
    public:
        /**
        * @brief Constructeur de l'arbre.
        * Il est possible de passer le nombre de noeuds que contiendra l'arbre à créer.
        * @param nodeCount  Le nombre de noeuds qui vont être ajoutés à l'arbre après sa création.
        * À préciser uniquement si on connaît le nombre de noeuds et que l'on souhaite
        * optimiser la création de l'arbre.
        */
        Tree(size_t nodeCount = 1) {
            _nodes.reserve(nodeCount > 0 ? nodeCount : 1);
        }

        /**
         * @brief Constructeur par recopie.
        */
        Tree(Tree const & tree) {
            _nodes.reserve(tree._nodes.capacity());
            _nodes = tree._nodes;
        }

        /**
         * @brief Crée un noeud enfant dont la valeur est passée en paramètre et dont son parent est le noeud à l'indice spécifié.
         * Peut invalider toutes les références des noeuds de l'arbre sauvegardées avant l'appel de la fonction.
         * @param index Indice du noeud parent.
         * @param value Valeur du noeud à ajouter.
         * @throw invalid_argument Si l'index du noeud parent passé en paramètre est un index invalide.
         * @return Une référence du noeud qui vient d'être créé.
         */
        inline Node & addChildFor(Node::Index const & index, Value const & value) {
            return addChildFor(getNodeFromIndex(index), value);
        }

        /**
         * @brief Crée un noeud enfant dont la valeur est passée en paramètre et dont son parent est le noeud spécifié.
         * Peut invalider toutes les références des noeuds de l'arbre sauvegardées avant l'appel de la fonction.
         * @param node  Noeud parent.
         * @param value Valeur du noeud à ajouter.
         * @throw invalid_argument Si le noeud parent passé en paramètre n'est pas présent dans l'arbre.
         * @return Une référence du noeud qui vient d'être créé.
         */
        inline Node & addChildFor(Node const & node, Value const & value) {
            if (!isNodePresent(node)) {
                throw std::invalid_argument("Le noeud renseigné n'est pas présent dans l'arbre.");
            }
            Node::Index const indexNode = node.index();
            _nodes.emplace_back(value, Node::Index{nodesCount()}, indexNode);
            _nodes[indexNode.value()].addChildIndex(_nodes.back().index());
            return _nodes.back();
        }

        /**
         * @brief Récupère le noeud à l'indice donné en paramètre.
         * @param index L'indice du noeud à récupérer.
         * @throw invalid_argument Si l'index passé en paramètre est invalide.
         * @return Une référence du noeud dont l'indice est spécifié.
         */
        inline Node & getNodeFromIndex(Node::Index const & index) {
              if (index.value() < nodesCount()) {
                return _nodes[index.value()];
              }
              throw std::invalid_argument("Aucun noeud n'est présent à l'index précisé (" + std::to_string(index.value()) + ").");
        }

        /**
         * @brief Récupère le noeud à l'indice donné en paramètre.
         * @param index L'indice du noeud à récupérer.
         * @throw invalid_argument Si l'index passé en paramètre est invalide.
         * @return Une référence constante du noeud dont l'indice est spécifié.
         */
        inline Node const & getNodeFromIndex(Node::Index const & index) const {
              if (index.value() < nodesCount()) {
                return _nodes[index.value()];
              }
              throw std::invalid_argument("Aucun noeud n'est présent à l'index précisé (" + std::to_string(index.value()) + ").");
        }

        /**
         * @throw invalid_argument Si la racine n'a pas été définie.
         * @return Une référence de la racine de l'arbre.
         *
         * @see setRoot
         */
        inline Node & getRoot() {
              if (nodesCount() > 0) {
                return _nodes[0];
              }
              throw std::invalid_argument("L'arbre ne possède pas de racine.");
        }

        /**
         * @throw invalid_argument Si la racine n'a pas été définie.
         * @return Une référence constante de la racine de l'arbre.
         *
         * @see setRoot
         */
        inline Node const & getRoot() const {
              if (nodesCount() > 0) {
                return _nodes[0];
              }
              throw std::invalid_argument("L'arbre ne possède pas de racine.");
        }

        /**
         * @brief Permet de définir la valeur de la racine de l'arbre.
         * @param value La valeur de la racine de l'arbre.
         * @return Une référence de la racine.
         */
        inline Node & setRoot(Value const & value) {
            _nodes.emplace(_nodes.begin(), value, Node::Index{0}, Node::Index{});
            return _nodes[0];
        }

        /**
         * @brief Vérifie que le noeud passé en paramètre est une référence de la racine de l'arbre.
         * @param node  Le noeud en question.
         * @return Vrai si le noeud est la racine de l'arbre, faux sinon.
         */
        inline bool isRoot(Node const & node) const {
            return nodesCount() > 0 && &_nodes[0] == &node;
        }

        /**
         * @brief Vérifie que le noeud passé en paramètre est une une feuille de l'arbre.
         * @param node  Le noeud en question.
         * @return Vrai si le noeud est une feuille de l'arbre, faux sinon.
         */
        inline bool isLeaf(Node const & node) const {
            return node.childrenCount() == 0;
        }

        /**
         * @brief Ajoute en avance une capacité donnée au tableau contenant les noeuds.
         * @param capacity  La capacité à ajouter.
         */
        inline void addCapacity(size_t capacity) {
            _nodes.reserve(_nodes.capacity() + capacity);
        }

        /**
         * @return Le nombre de noeuds de l'arbre.
         */
        inline size_t nodesCount() const {
            return _nodes.size();
        }

        /**
         * @brief Affiche l'arbre sur la sortie standard.
         */
        void display() const {
            if (nodesCount() > 0) {
                displayNode(_nodes[0]);
            } else {
                std::cout << "Arbre vide" << std::endl;
            }
        }

        /**
         * @brief Affiche le noeud et ses enfants sur la sortie standard.
         * @param node  Le noeud qu'il faut afficher.
         * @param k     L'indentation en fonction de la profondeur atteinte.
         */
        void displayNode(Node const & node, int k = -1) const {
            if (k > 0)
                std::cout << "|";
            for (int j = 0; j < k; j++)
                std::cout << "    ";
            if (k >= 0)
                std::cout << "|- ";
            std::cout << node.value() << "\n";
            for (Node::Index const & i : node.indexesChildren()) {
                displayNode(getNodeFromIndex(i), k + 1);
            }
        }

        Tree & operator= (Tree const & tree) {
            if (&tree != this) {
                _nodes.clear();
                _nodes.reserve(tree._nodes.capacity());
                _nodes = tree._nodes;
            }
            return *this;
        }

    private:
        /**
         * @brief Vérifie que le noeud passé en paramètre est présent dans l'arbre.
         * @param node  Le noeud dont il faut vérifier la présence.
         * @return Vrai si le noeud est présent dans cet arbre, faux sinon.
         */
        inline bool isNodePresent(Node const & node) const {
            // On vérifie que l'index du noeud pointe bien sur le noeud avec la même adresse mémoire
            return node.index().value() < nodesCount() && &_nodes[node.index().value()] == &node;
        }

        /// L'ensemble des noeuds de l'arbre.
        std::vector<Node> _nodes;
};
