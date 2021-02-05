#pragma once
#include "node.hh"

#include <stdexcept>

/**
 * @brief Représentation d'un arbre n-aire.
 * Cette classe contient les informations de la racine de l'arbre.
 */
class Tree {
    public:
        /**
         * @brief Constructeur prenant en paramètre la valeur du noeud racine.
         * Il est également possible de passer le nombre de noeuds que contiendra l'arbre à créer.
         * @param v             La valeur de la racine.
         * @param nodeCount     Le nombre de noeuds qui vont être ajoutés à l'arbre après sa création.
         *                      À préciser uniquement si on connaît le nombre de noeuds et que l'on souhaite
         *                      optimiser la création de l'arbre.
         */
        Tree(size_t nodeCount = 1);

        /**
         * @brief Constructeur par recopie.
        */
        Tree(Tree const & tree);

        /**
         * @brief Crée un noeud enfant dont la valeur est passée en paramètre et dont son parent est le noeud à l'indice spécifié.
         * Peut invalider toutes les références des noeuds de l'arbre sauvegardées avant l'appel de la fonction.
         * @param index Indice du noeud parent.
         * @param v     Valeur du noeud à ajouter.
         * @return Une référence du noeud qui vient d'être créé.
         */
        inline Node & addChildFor(Node::Index const & index, Value const & value) {
            return addChildFor(getNodeFromIndex(index), value);
        }

        /**
         * @brief Crée un noeud enfant dont la valeur est passée en paramètre et dont son parent est le noeud spécifié.
         * Peut invalider toutes les références des noeuds de l'arbre sauvegardées avant l'appel de la fonction.
         * @param node  Noeud parent.
         * @param v     Valeur du noeud à ajouter.
         * @return Une référence du noeud qui vient d'être créé.
         */
        inline Node & addChildFor(Node const & node, Value const & value) {
//            if (!isNodePresent(node)) {
//                throw std::invalid_argument("Le noeud renseigné n'est pas présent dans l'arbre.");
//            }

            Node::Index const indexNode = node.index();
            _nodes.emplace_back(value, Node::Index{nodesCount()});
            getNodeFromIndex(indexNode).addChildIndex(_nodes.back().index());
            return _nodes.back();
        }

        /**
         * @brief Récupère le noeud à l'indice donné en paramètre.
         * @param index L'indice du noeud à récupérer.
         * @return Une référence du noeud dont l'indice est spécifié.
         */
        inline Node & getNodeFromIndex(Node::Index const & index) {
//            if (index.value() < nodesCount()) {
                return _nodes[index.value()];
//            }
//            throw std::invalid_argument("Aucun noeud n'est présent à l'index précisé.");
        }

        /**
         * @brief Récupère le noeud à l'indice donné en paramètre.
         * @param index L'indice du noeud à récupérer.
         * @return Une référence constante du noeud dont l'indice est spécifié.
         */
        inline Node const & getNodeFromIndex(Node::Index const & index) const {
//            if (index.value() < nodesCount()) {
                return _nodes[index.value()];
//            }
//            throw std::invalid_argument("Aucun noeud n'est présent à l'index précisé.");
        }

        inline Node & getRoot() {
//            if (nodesCount() > 0) {
                return _nodes[0];
//            }
//            throw std::invalid_argument("L'arbre ne possède pas de racine.");
        }

        inline Node const & getRoot() const {
//            if (nodesCount() > 0) {
                return _nodes[0];
//            }
//            throw std::invalid_argument("L'arbre ne possède pas de racine.");
        }

        inline Node & setRoot(Value const & value) {
            _nodes.emplace(_nodes.begin(), value, Node::Index{0});
            return _nodes[0];
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
        void display() const;

        /**
         * @brief Affiche le noeud et ses enfants sur la sortie standard.
         * @param node  Le noeud qu'il faut afficher.
         * @param k     L'indentation en fonction de la profondeur atteinte.
         */
        void displayNode(Node const & node, int k = -1) const;

        Tree & operator= (Tree const & tree);

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
