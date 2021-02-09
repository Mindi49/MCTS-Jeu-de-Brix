#pragma once

#include "joueur.hh"
#include "../../TreesFunctions_no_ptr/treeutil.hh"
#include "../../TreesFunctions_no_ptr/tree.hh"
#include "../../TreesFunctions_no_ptr/node.hh"

#define POND_C 2

class Joueur_MonteCarlo_ : public Joueur {
    public:
      Joueur_MonteCarlo_(std::string nom, bool joueur);
      //char nom_abbrege() const override;

      void recherche_coup(Jeu, Brix & coup) override;
      Node & descent(Node & currentNode);
      Node & growth(Node & currentNode, Brix & move);
      float calculateQUBC (Node const & node, Node const & parentNode) const;
      Node & findBestQUBC(Node & node) const;

    private:
        Tree _tree;
};
