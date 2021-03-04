#include "joueur_montecarlo_.hh"
#include "../arbitre.hh"
#include <math.h>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <future>
#include <chrono>

Tree Joueur_MonteCarlo_::_tree;
bool Joueur_MonteCarlo_::_created(false);

Joueur_MonteCarlo_::Joueur_MonteCarlo_(std::string name, bool player) : Joueur(name,player), _canWrite(true) {
    if(!_created) {
        _tree.setRoot(Value{0,0,Brix(0,0,0,0)});
        TreeUtil::fileToTree("ABBEL_save.MCTS",_tree);
        _created = true;
    }
    _currentRoot = _tree.getRoot().index();
}


Joueur_MonteCarlo_::~Joueur_MonteCarlo_() {
    if (_canWrite) {
        //TreeUtil::treeToFile(_tree,"ABBEL_save.MCTS");
    }
    _tree.addCapacity(50*NB_PIECE_MAX*TEMPS_POUR_UN_COUP);
}


/*
char Joueur_MonteCarlo_::nom_abbrege() const {
    return 'M';
}
*/


void Joueur_MonteCarlo_::recherche_coup(Jeu game, Brix & move) {
    _canWrite = false;
    if (game.nbCoupJoue() != 0) { // Si l'adversaire vient de jouer
        Brix opponentMove(move);
        playOpponentMove(opponentMove,game);
    }
    // Choix du coup à jouer
    if (_tree.getNodeFromIndex(_currentRoot).childrenCount() > 0) { // Si on a déjà des enfants
        _currentRoot = chooseBestChildNode();
    }
    else { // Crée un noeud parmi les coups légaux
        _currentRoot = processMCTS(game);
    }
    move = _tree.getNodeFromIndex(_currentRoot).value().brix;
    game.joue(move);
    // Lance un thread qui réalise le MCTS en boucle selon le temps écoulé
    std::thread(&Joueur_MonteCarlo_::processLoopingMCTS,this,game).detach();
}


void Joueur_MonteCarlo_::playOpponentMove(Brix const & opponentMove, Jeu const & game) {
    // Cherche le noeud fils correspondant au coup joué par l'adversaire
    std::vector<Node::Index> const & indexesChildren(_tree.getNodeFromIndex(_currentRoot).indexesChildren());
    std::vector<Node::Index>::const_iterator it(std::find_if(indexesChildren.begin(), indexesChildren.end(), [&opponentMove, this](Node::Index child) {
        Brix const & brix(_tree.getNodeFromIndex(child).value().brix);
        return opponentMove.getAo() == brix.getAo() && opponentMove.getAx() == brix.getAx() && opponentMove.getOo() == brix.getOo() && opponentMove.getOx() == brix.getOx();
    }));

    if (it == indexesChildren.end()) { // Si le noeud n'est pas déjà présent dans l'arbre
        _currentRoot = growth(_currentRoot, opponentMove);
        update(_currentRoot, rollout(game));
    }
    else {
        _currentRoot = *it;
    }
}


Node::Index Joueur_MonteCarlo_::processMCTS(Jeu game) {
    Jeu currentGame = game;
    Node::Index grownNodeIndex(descent(_currentRoot,currentGame));
    int gain = rollout(currentGame);
    update(grownNodeIndex, gain);
    return grownNodeIndex;
}


Node::Index Joueur_MonteCarlo_::chooseBestChildNode() const {
    std::vector<Node::Index> const & indexesChildren(_tree.getNodeFromIndex(_currentRoot).indexesChildren());
    return *std::max_element(indexesChildren.begin(), indexesChildren.end(), [this](Node::Index nodeIndex1, Node::Index nodeIndex2) {
        Value const & nodeValue1(_tree.getNodeFromIndex(nodeIndex1).value());
        Value const & nodeValue2(_tree.getNodeFromIndex(nodeIndex2).value());
        return float(nodeValue1.gain)/nodeValue1.visitCount < float(nodeValue2.gain)/nodeValue2.visitCount;
    });
}


void Joueur_MonteCarlo_::processLoopingMCTS(Jeu game) {
    float time(0);
    std::chrono::high_resolution_clock::time_point start(std::chrono::high_resolution_clock::now());
    while(TEMPS_POUR_UN_COUP - time > 0) {
        processMCTS(game);
        std::chrono::high_resolution_clock::time_point loopEnd(std::chrono::high_resolution_clock::now());
        time = std::chrono::duration_cast<std::chrono::milliseconds>(loopEnd - start).count();
    }
    // On ne peut pas écrire tant qu'on a pas réalisé toutes les étapes à la suite
    _canWrite = true;
}


Node::Index Joueur_MonteCarlo_::descent(Node::Index currentNodeIndex, Jeu & game){
    std::vector<Brix> legalMoves(findLegalMoves(game));
    if (!legalMoves.empty()) {
        Node & currentNode(_tree.getNodeFromIndex(currentNodeIndex));

        if (currentNode.childrenCount() < legalMoves.size()) { // S'il reste des coups licites non explorés
            Brix const & move = legalMoves[currentNode.childrenCount()];
            game.joue(move);
            return growth(currentNodeIndex,move);
        }
        else { // Si on a exploré au moins une fois tous les coups licites à partir de ce noeud
            Node::Index bestNodeIndex(findBestQUBC(currentNodeIndex));
            game.joue(_tree.getNodeFromIndex(bestNodeIndex).value().brix);
            return descent(bestNodeIndex,game);
        }
    }
    game.joue(_tree.getNodeFromIndex(currentNodeIndex).value().brix);
    return currentNodeIndex;
}


float Joueur_MonteCarlo_::calculateNodeQUBC(Node::Index nodeIndex, Node::Index parentNodeIndex) const {
    Value const & nodeValue(_tree.getNodeFromIndex(nodeIndex).value());
    Value const & parentNodeValue(_tree.getNodeFromIndex(parentNodeIndex).value());
    return (nodeValue.gain / nodeValue.visitCount) + sqrtf(POND_C * (logf(parentNodeValue.visitCount / nodeValue.visitCount)));
}


Node::Index Joueur_MonteCarlo_::findBestQUBC(Node::Index nodeIndex) const {
    std::vector<Node::Index> indexesChildren(_tree.getNodeFromIndex(nodeIndex).indexesChildren());
    return *std::max_element(indexesChildren.begin(), indexesChildren.end(), [this,nodeIndex](Node::Index childNodeIndex1, Node::Index childNodeIndex2) {
        return calculateNodeQUBC(childNodeIndex1,nodeIndex) < calculateNodeQUBC(childNodeIndex2,nodeIndex);
    });
}


Node::Index Joueur_MonteCarlo_::growth(Node::Index currentNodeIndex, Brix const & move) {
    return _tree.addChildFor(currentNodeIndex, Value{0,0,move}).index();
}


int Joueur_MonteCarlo_::rollout (Jeu game) {
    while (!game.fini()) {
        std::vector<Brix> const & legalMoves = findLegalMoves(game);
        game.joue(legalMoves[static_cast<size_t>(rand()) % legalMoves.size()]);
    }
    if (game.partie_nulle()) {
        return 0;
    }
    if ((joueur() && game.partie_X()) || (!joueur() && game.partie_O())) {
        return 1;
    }
    return -1;
}


void Joueur_MonteCarlo_::update (Node::Index currentNodeIndex, int gain) {
    Node & currentNode(_tree.getNodeFromIndex(currentNodeIndex));
    currentNode.value().gain += gain;
    currentNode.value().visitCount += 1;
    if (!_tree.isRoot(currentNode)) {
        update(currentNode.parentIndex(), gain);
    }
}


std::vector<Brix> Joueur_MonteCarlo_::findLegalMoves(Jeu const & game) const{
    std::vector<Brix> legalMoves;
    legalMoves.reserve(20);
    Brix b_canditate;
    int turn(game.nbCoupJoue() + 1);

    for (int i(0); i < MAX_LARGEUR; i++) {
        int j(0);
        while(j < MAX_HAUTEUR && game.plateau()[static_cast<size_t>(j)][static_cast<size_t>(i)] != '0') {
            j++;
        }
        if (j < MAX_HAUTEUR) {
            b_canditate.setAllCoord(i,j,i,j+1);
            if (game.coup_licite(b_canditate,turn)) {
                legalMoves.push_back(b_canditate);
            }

            b_canditate.setAllCoord(i,j+1,i,j);
            if (game.coup_licite(b_canditate,turn)) {
                legalMoves.push_back(b_canditate);
            }

            b_canditate.setAllCoord(i,j,i+1,j);
            if (game.coup_licite(b_canditate,turn)) {
                legalMoves.push_back(b_canditate);
            }

            b_canditate.setAllCoord(i+1,j,i,j);
            if (game.coup_licite(b_canditate,turn)) {
                legalMoves.push_back(b_canditate);
            }
        }
    }
    return legalMoves;
}


std::vector<Brix>::const_iterator Joueur_MonteCarlo_::findBrix(std::vector<Brix> const & brixs, Brix const & b) const {
    return std::find_if(brixs.begin(),brixs.end(),[&b](Brix const & brix) {
        return brix.getAo() == b.getAo() && brix.getAx() == b.getAx() && brix.getOo() == b.getOo() && brix.getOx() == b.getOx();
    });
}
