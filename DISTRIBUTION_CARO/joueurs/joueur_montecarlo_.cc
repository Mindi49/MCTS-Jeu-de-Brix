#include "joueur_montecarlo_.hh"
#include <math.h>
#include <limits>
#include <cstdlib>
#include <algorithm>
Joueur_MonteCarlo_::Joueur_MonteCarlo_(std::string nom, bool joueur) : Joueur(nom,joueur), _tree() {
    _tree.setRoot(Value{});
}



/*char Joueur_MonteCarlo_::nom_abbrege() const
{
    return 'M';
}
*/

void Joueur_MonteCarlo_::recherche_coup(Jeu jeu, Brix & coup) {
    // à voir...
    if (!joueur()) { // on est pas le premier à jouer
        // trouver le coup joué par l'adversaire
        Brix opponentMove = findOpponentMove(jeu);

        // faire le MCTS à partir du coup joué par l'adversaire...
    }
    MCTS(jeu);
    // renvoyer le meilleur coup dans ceux que l'on peut alors jouer
    chooseBestNode(coup); // arrete la fonction pour rendre le mutex
}


void Joueur_MonteCarlo_::MCTS(Jeu & jeu) {
    Node & currentNode = _tree.getRoot(); // à remplacer ptet par un node qui serait la racine de l'arbre sur lequel on travaille
    while(true) { // TODO : time left
        Jeu currentGame = jeu;
        Node & grownNode = descent(currentNode, currentGame); // renvoie le noeud à partir duquel faire le rollout
        int gain = rollout(grownNode, currentGame);// rollout
        update(currentNode, gain);
    }
    // arrete la fonction pour rendre le mutex
}


Node & Joueur_MonteCarlo_::descent(Node & currentNode, Jeu & jeu){
    if (!currentNode.value().terminal) { // feuille de l'arbre de recherche
// SELECTION DES COUPS SUR L'ARBRE DE RECHERCHE ACTUEL
        std::vector<Brix> legalMoves = findLegalMoves(jeu);
        //if (true) {// TODO : s'il reste des coups licites non explorés (pas exploré 1 fois, noeud pas créé)
// CHOIX D'UN CANDIDAT NON EPLORÉ
        if (currentNode.childrenCount() < legalMoves.size()) {
            // pour trouver pour chaque legal move s'il est déjà exploré (pas opti)
            std::vector<Brix> exploredMoves;
            for (auto const & child : currentNode.indexesChildren()) {
                exploredMoves.push_back(_tree.getNodeFromIndex(child).value().brix);
            }
            auto it = std::find_if(legalMoves.begin(), legalMoves.end(), [&exploredMoves](Brix const & legalMove){
                return std::find(exploredMoves.begin(), exploredMoves.end(), legalMove) == exploredMoves.end();
            });
            if (it != legalMoves.end())
                Brix move = *it;
            // à voir ce qu'on fait sinon

            // on prend le premier non exploré (ou on le choisit au pif si on est courageux)
             // TODO : trouver la valeur en fonctions des coups dispos
// FEUILLE DE L'ARBRE DE RECHERCHE ACTUEL
// CHOIX DU NOUVEAU NOEUD À CREER PARMI LES SUCCESSEURS
            jeu.joue(move);
            return growth(currentNode, move);
        }
        else {
// FONCTION DE VALEUR DE COUP
// CHOIX DU MEILLEUR CANDIDAT
            Node & bestNode = findBestQUBC(currentNode);
            jeu.joue(Brix(bestNode.value().brix));
            return descent(bestNode, jeu);
        }
    }
    return currentNode;
}


Node & Joueur_MonteCarlo_::growth(Node & currentNode, Brix const & move) {
    currentNode.value().terminal = false;
    return _tree.addChildFor(currentNode, Value{0,0,true,move}); // TODO : renseigner la BRIX à la valeur pour pouvoir choisir ce coup à la fin, ax, ao, ox, oo
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



int Joueur_MonteCarlo_::rollout (Node const & grownNode, Jeu jeu) {
    while (!jeu.fini()) {
         std::vector<Brix> legalMoves = findLegalMoves(jeu);
         Brix move = legalMoves[rand() % legalMoves.size()];
         jeu.joue(move);
    }
    if (jeu.partie_nulle()) return 0;
    if ((joueur() && jeu.partie_X()) || (!joueur() && jeu.partie_O())) return 1;
    else return -1;
}


void Joueur_MonteCarlo_::update (Node & currentNode, int gain) {
    if (_tree.isRoot(currentNode)) {
        currentNode.value().gain += gain;
        currentNode.value().visitCount += 1;
    }
    else {
        currentNode.value().gain += gain;
        currentNode.value().visitCount += 1;
        update(_tree.getNodeFromIndex(currentNode.parentIndex()), gain);
    }
}

Jeu constructGameFromCurrentNode() {

}

Brix Joueur_MonteCarlo_::findOpponentMove(Jeu jeu) {
    std::vector<Brix> legalMoves = findLegalMoves(jeu);
    for (auto const & i : legalMoves) {
        Jeu tmp = constructGameFromCurrentNode();
        tmp.joue(i);
        if (tmp.plateau() == jeu.plateau())
            return i;
    }
}

void Joueur_MonteCarlo_::chooseBestNode(Brix & coup) {
    int maxValue = 0;

    for (auto const & i : _tree.getRoot().indexesChildren()) {
        Node & node = _tree.getNodeFromIndex(i);
        if (node.value().gain > maxValue) {
            Brix const &  brix = node.value().brix;
            coup.setAllCoord(brix.getAo(),brix.getAx(),brix.getOx(),brix.getOo());
            maxValue = node.value().gain;
        }
    }
}

std::vector<Brix> Joueur_MonteCarlo_::findLegalMoves(Jeu jeu) {
    std::vector<Brix> legalMoves;
    Brix b_canditate;
    int turn = jeu.nbCoupJoue() + 1; //la b_candidate devra être valide au tour auquel on va la jouer,i.e. au tour suivant
    int moveIndex, size;
    int aX, oX, aO, oO; //coordonnees de la Brix que l'on va joué

    for (int i = 0; i < MAX_LARGEUR;i++) {//i abscisse donc numero de colonne
            int j = 0; //j ordonnee donc numero de ligne
            while(j < MAX_HAUTEUR && jeu.plateau()[j][i] != '0') { //dans plateau l'ordonnee en premier
                j++;
            }
            if (j < MAX_HAUTEUR) { //On est tombé sur une case vide, i.e. contenant '0'
                    //Cherchons des coups valides à partir de cette case vide.
                    b_canditate.setAllCoord(i,j,i,j+1); //brix verticale dont le bottom est X
                    if (jeu.coup_licite(b_canditate,turn))
                        legalMoves.push_back(b_canditate);

                    b_canditate.setAllCoord(i,j+1,i,j); //brix verticale dont le bottom est O
                    if (jeu.coup_licite(b_canditate,turn))
                        legalMoves.push_back(b_canditate);

                    b_canditate.setAllCoord(i,j,i+1,j); //brix horizontale commençant par X
                    if (jeu.coup_licite(b_canditate,turn))
                        legalMoves.push_back(b_canditate);

                    b_canditate.setAllCoord(i+1,j,i,j); //brix terminant commençant par X
                    if (jeu.coup_licite(b_canditate,turn))
                        legalMoves.push_back(b_canditate);
                }
        }
    return legalMoves;
    //std::cout<<legalMoves<<std::endl;
    //On choisit un coup au hazard
    size = legalMoves.size();
    moveIndex = size == 1 ?  0 : rand() % (size-1);
    aO = legalMoves[moveIndex].getAo();
    oO = legalMoves[moveIndex].getOo();
    aX = legalMoves[moveIndex].getAx();
    oX = legalMoves[moveIndex].getOx();
    //coup.setAllCoord(aX, oX, aO, oO);

    //std::this_thread::sleep_for (std::chrono::milliseconds(rand() % (TEMPS_POUR_UN_COUP+1)));

}






