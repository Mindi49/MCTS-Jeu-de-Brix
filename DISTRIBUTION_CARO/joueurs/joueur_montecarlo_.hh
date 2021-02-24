#pragma once

#include "joueur.hh"
#include "../../TreesFunctions_no_ptr/treeutil.hh"
#include "../../TreesFunctions_no_ptr/tree.hh"
#include "../../TreesFunctions_no_ptr/node.hh"
#include "../../TreesFunctions_no_ptr/value.hh"

#define POND_C 2

class Joueur_MonteCarlo_ : public Joueur {
    public:
        Joueur_MonteCarlo_(std::string nom, bool joueur);
        //char nom_abbrege() const override;

        void recherche_coup(Jeu, Brix & coup) override;

        void MCTS(Jeu & jeu);
        Node & descent(Node & currentNode, Jeu & jeu);
        Node & growth(Node & currentNode, Brix const & move);
        int rollout (Node const & grownNode, Jeu jeu);
        void update (Node & currentNode, int gain);

        float calculateQUBC (Node const & node, Node const & parentNode) const;
        Node & findBestQUBC(Node & node) const;

        void setBestNode(Brix & coup);
        Brix findOpponentMove(Jeu jeu);
        std::vector<Brix> findLegalMoves(Jeu jeu);
        void chooseBestNode(Brix & coup);


    private:
        Tree _tree;
};
