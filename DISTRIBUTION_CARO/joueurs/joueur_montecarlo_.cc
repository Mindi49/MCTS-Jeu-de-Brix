#include "joueur_montecarlo_.hh"
#include <math.h>
#include <limits>

Joueur_MonteCarlo_::Joueur_MonteCarlo_(std::string nom, bool joueur) : Joueur(nom,joueur), _tree() {
    _tree.setRoot(Value{});
}



/*char Joueur_MonteCarlo_::nom_abbrege() const
{
    return 'M';
}
*/



Node & Joueur_MonteCarlo_::descent(Node & currentNode){
    if (!currentNode.value().terminal) {
        if (true) {// TODO : s'il reste des coups licites non explorés (pas exploré 1 fois, noeud pas créé)
            Brix legalMove; // TODO : trouver la valeur en fonctions des coups dispos
            return growth(currentNode, legalMove);
        }
        else {
            return descent(findBestQUBC(currentNode));
        }
    }
    return currentNode;
}


Node & Joueur_MonteCarlo_::growth(Node & currentNode, Brix & move) {
    return _tree.addChildFor(currentNode, Value{}); // TODO : renseigner la BRIX à la valeur pour pouvoir choisir ce coup à la fin

}



// on passe le noeud courant et son parent
float Joueur_MonteCarlo_::calculateQUBC (Node const & node, Node const & parentNode) const {
    return (node.value().gain / node.value().visitCount) + sqrtf(POND_C * (logf(parentNode.value().visitCount / node.value().visitCount)));
    // gain / visit + racine(c * ln(visit parent) / visit)
}


Node & Joueur_MonteCarlo_::findBestQUBC(Node & node) const {
    float max = std::numeric_limits<float>::min();
    Node & bestNode = node;
    for (auto const & i : node.indexesChildren()) {
        Node const & child = _tree.getNodeFromIndex(i);
        float qubc = calculateQUBC(child,node);
        if (qubc > max) {
            max = qubc;
            bestNode = child;
        }
    }
    return bestNode;
}



int rollout (Node const & grownNode) {

}






void Joueur_MonteCarlo_::recherche_coup(Jeu jeu, Brix &coup) {
    Node & current = _tree.getRoot();

    while(true) { // TODO
        Node & grownNode = descent(current); // renvoie le noeud à partir duquel faire le rollout
        int gain = rollout(grownNode);// rollout
        // update
    }

    return; // arrete la fonction pour rendre le mutex
}
