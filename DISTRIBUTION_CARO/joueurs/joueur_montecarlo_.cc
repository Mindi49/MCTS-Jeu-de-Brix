#include "joueur_montecarlo_.hh"
#include "systemutil.hh"
#include "../arbitre.hh"

#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>
#include <cstdlib>
#include <future>
#include <limits>
#include <math.h>


Tree Joueur_MonteCarlo_::_tree;
bool Joueur_MonteCarlo_::_created(false);
bool Joueur_MonteCarlo_::_canProcessMCTS(true);
bool Joueur_MonteCarlo_::_canWrite(false);
std::mutex Joueur_MonteCarlo_::_treeLock;


Joueur_MonteCarlo_::Joueur_MonteCarlo_(std::string name, bool player) : Joueur(name,player) {
    std::ios_base::sync_with_stdio(false);
    if(!_created) {
        size_t nodeCountAllocation = SystemUtil::getMemorySize()/(2*sizeof(Node))*1000;
        TreeUtil::fileToTree("ABBEL_save.MCTS", _tree, nodeCountAllocation);
        _created = true;
    }
    _currentRoot = _tree.getRoot().index();
}


Joueur_MonteCarlo_::~Joueur_MonteCarlo_() {
    _canProcessMCTS = false;
    _treeLock.lock();
    _treeLock.unlock();
    // À commenter lors du tournoi
//    if (_canWrite) {
//        TreeUtil::treeToFile(_tree,"ABBEL_save.MCTS");
//    }
}


void Joueur_MonteCarlo_::recherche_coup(Jeu game, Brix & move) {
    _canProcessMCTS = false;
    _canWrite = false;

    Brix newMove = move;
    newMove.setDefinie(true);

    if (!_tree.isFull()) {
        if (game.nbCoupJoue() != 0) { // L'adversaire vient de jouer
            Brix opponentMove(newMove);
            playOpponentMove(opponentMove,game);
        }

        // Choix du coup à jouer
        if (_tree.getNodeFromIndex(_currentRoot).childrenCount() > 0) { // On a déjà des enfants
            _currentRoot = chooseBestChildNode();
        }
        else { // Pas d'enfant : Crée un noeud parmi les coups légaux
            _currentRoot = processMCTS(game);
        }
        newMove = _tree.getNodeFromIndex(_currentRoot).value().brix;
    }
    else { // L'arbre a atteint sa capacité maximale (moitié de la RAM)
        // Joue un coup random
        std::vector<Brix> const & legalMoves = findLegalMoves(game);
        newMove = legalMoves[static_cast<size_t>(rand())%legalMoves.size()];
    }

    move = newMove;
    move.setDefinie(true);

    // Lance le thread qui réalise le MCTS en boucle
    game.joue(newMove);
    std::thread(&Joueur_MonteCarlo_::processLoopingMCTS,this,game).detach();
}


void Joueur_MonteCarlo_::playOpponentMove(Brix const & opponentMove, Jeu const & game) {
    // Cherche le noeud fils correspondant au coup joué par l'adversaire
    std::vector<Node::Index> const & indexesChildren(_tree.getNodeFromIndex(_currentRoot).indexesChildren());
    std::vector<Node::Index>::const_iterator const & it(std::find_if(indexesChildren.begin(), indexesChildren.end(), [&opponentMove](Node::Index const & child) {
        Brix const & brix(_tree.getNodeFromIndex(child).value().brix);
        return opponentMove.getAx() == brix.getAx() && opponentMove.getOx() == brix.getOx() && opponentMove.getAo() == brix.getAo() && opponentMove.getOo() == brix.getOo();
    }));

    if (it == indexesChildren.end()) { // Le noeud n'est pas déjà présent dans l'arbre
        _currentRoot = growth(_currentRoot, opponentMove);
        update(_currentRoot, rollout(game));
    }
    else {
        _currentRoot = *it;
    }
}


Node::Index Joueur_MonteCarlo_::processMCTS(Jeu game) {
    Jeu currentGame = game;
    Node::Index const & grownNodeIndex(descent(_currentRoot,currentGame));
    int gain = rollout(currentGame);
    update(grownNodeIndex, gain);
    return grownNodeIndex;
}


Node::Index Joueur_MonteCarlo_::chooseBestChildNode() const {
    std::vector<Node::Index> const & indexesChildren(_tree.getNodeFromIndex(_currentRoot).indexesChildren());
    return *std::max_element(indexesChildren.begin(), indexesChildren.end(), [](Node::Index const & nodeIndex1, Node::Index const & nodeIndex2) {
        Value const & nodeValue1(_tree.getNodeFromIndex(nodeIndex1).value());
        Value const & nodeValue2(_tree.getNodeFromIndex(nodeIndex2).value());
        return float(nodeValue1.gain)/nodeValue1.visitCount < float(nodeValue2.gain)/nodeValue2.visitCount;
    });
}


void Joueur_MonteCarlo_::processLoopingMCTS(Jeu game) {
    _treeLock.lock();
    _canProcessMCTS = true;
    while (_canProcessMCTS && !_tree.isFull()) {
        processMCTS(game);
    }
    _treeLock.unlock();
    _canWrite = true;
}


Node::Index Joueur_MonteCarlo_::descent(Node::Index currentNodeIndex, Jeu & game){
    std::vector<Brix> const & legalMoves(findLegalMoves(game));
    if (!legalMoves.empty()) {
        Node & currentNode(_tree.getNodeFromIndex(currentNodeIndex));
        size_t currentNodeChildrenCount(currentNode.childrenCount());

        if (currentNodeChildrenCount < legalMoves.size()) { // Il reste des coups licites non explorés
            std::vector<Brix> exploredMoves;
            for (Node::Index const & childIndex : currentNode.indexesChildren()) {
                exploredMoves.push_back(_tree.getNodeFromIndex(childIndex).value().brix);
            }

            std::vector<Brix> unexploredMoves;
            std::for_each(legalMoves.begin(), legalMoves.end(), [this,&exploredMoves,&unexploredMoves](Brix const & legalMove){
                if(findBrix(exploredMoves,legalMove) == exploredMoves.end()) {
                    unexploredMoves.push_back(legalMove);
                }
            });

            Brix move = unexploredMoves[static_cast<size_t>(rand()) % unexploredMoves.size()];

            game.joue(move);
            return growth(currentNodeIndex,move);
        }
        else { // On a exploré au moins une fois tous les coups licites à partir de ce noeud
            Node::Index const & bestNodeIndex(findBestQUBC(currentNodeIndex));
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
    return (nodeValue.gain / nodeValue.visitCount) + sqrtf(POND_C * (logf(parentNodeValue.visitCount) / nodeValue.visitCount));
}


Node::Index Joueur_MonteCarlo_::findBestQUBC(Node::Index nodeIndex) const {
    std::vector<Node::Index> const & indexesChildren(_tree.getNodeFromIndex(nodeIndex).indexesChildren());
    return *std::max_element(indexesChildren.begin(), indexesChildren.end(),
    [this,&nodeIndex](Node::Index const & childNodeIndex1, Node::Index const & childNodeIndex2) {
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
    currentNode.value().visitCount++;
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
        return brix.getAx() == b.getAx() && brix.getOx() == b.getOx() && brix.getAo() == b.getAo() && brix.getOo() == b.getOo();
    });
}
