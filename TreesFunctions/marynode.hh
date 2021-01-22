#pragma once
#include <vector>

/**
 * @brief Informations conservées dans le noeud racine
 */
struct value {
    int eval;
};


/**
 * @brief Arbre n-aire utilisé pour la représentation de l'arbe dans le Monte-Carlo
 */
class MaryNode {
    public:
        MaryNode()=default;

        /**
         * @brief Constructeur prenant les informations sur le noeud racine de l'abre
         */
        MaryNode(value v);
        ~MaryNode();

        /**
         * @return La valeur du noeud racine
         */
        value const & getValue() const;

        /**
         * @brief Définit la valeur du noeud racine
         */
        void setValue(value const & v);

        /**
         * @return L'ensemble des noeuds fils du noeud racine
         */
        std::vector<MaryNode*> const & getNodeSet() const;

        /**
         * @brief Ajoute un arbre n-aire à l'ensemble des fils
         * @param child l'arbre n-aire à ajouter
         */
        void addChild(MaryNode* child);

        /**
         * @brief Affiche l'arbre n-aire sur la sortie standard
         * @param k définit l'indentation en fonction de la profondeur atteinte
         */
        void display(int k=-1) const;


    private:
        /// La valeur du noeud racine
        value _value;
        /// L'ensemble des noeuds fils
        std::vector<MaryNode*> _nodeSet;
};
