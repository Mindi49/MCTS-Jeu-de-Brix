#include "joueur_montecarlo_ABBEL.hh"
#include "systemutil_ABBEL.hh"
#include "../arbitre.hh"

#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>
#include <cstdlib>
#include <future>
#include <limits>
#include <math.h>


Tree_ABBEL Joueur_MonteCarlo_ABBEL::_tree;
bool Joueur_MonteCarlo_ABBEL::_created(false);
bool Joueur_MonteCarlo_ABBEL::_canProcessMCTS(true);
bool Joueur_MonteCarlo_ABBEL::_canWrite(false);
std::mutex Joueur_MonteCarlo_ABBEL::_treeLock;

using Index = Node_ABBEL::Index;

Joueur_MonteCarlo_ABBEL::Joueur_MonteCarlo_ABBEL(std::string name, bool player) : Joueur(name,player) {
    std::ios_base::sync_with_stdio(false);
    if(!_created) {
        size_t nodeCountAllocation = SystemUtil_ABBEL::getMemorySize()/(2*sizeof(Node_ABBEL))*1000;
        TreeUtil_ABBEL::fileToTree("ABBEL_save.MCTS", _tree, nodeCountAllocation);
        _created = true;
    }
    _currentRoot = _tree.getRoot().index();
}


Joueur_MonteCarlo_ABBEL::~Joueur_MonteCarlo_ABBEL() {
    _canProcessMCTS = false;
    _treeLock.lock();
    _treeLock.unlock();
    // À commenter lors du tournoi
//    if (_canWrite) {
//        TreeUtil_ABBEL::treeToFile(_tree,"ABBEL_save.MCTS");
//    }
}


void Joueur_MonteCarlo_ABBEL::recherche_coup(Jeu game, Brix & move) {
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
            _currentRoot = chooseBestChildNode(game.nbCoupJoue());
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
    std::thread(&Joueur_MonteCarlo_ABBEL::processLoopingMCTS,this,game).detach();
}


void Joueur_MonteCarlo_ABBEL::playOpponentMove(Brix const & opponentMove, Jeu const & game) {
    // Cherche le noeud fils correspondant au coup joué par l'adversaire
    std::vector<Index> const & indexesChildren(_tree.getNodeFromIndex(_currentRoot).indexesChildren());
    std::vector<Index>::const_iterator const & it(std::find_if(indexesChildren.begin(), indexesChildren.end(), [&opponentMove](Index const & child) {
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


Index Joueur_MonteCarlo_ABBEL::processMCTS(Jeu game) {
    Jeu currentGame = game;
    Index const & grownNodeIndex(descent(_currentRoot,currentGame));
    int gain = rollout(currentGame);
    update(grownNodeIndex, gain);
    return grownNodeIndex;
}


Index Joueur_MonteCarlo_ABBEL::chooseBestChildNode(int playedMovesCount) const {
    int op = playedMovesCount % 2 == 0 ? 1 : -1;

    std::vector<Index> const & indexesChildren(_tree.getNodeFromIndex(_currentRoot).indexesChildren());
    return *std::max_element(indexesChildren.begin(), indexesChildren.end(), [op](Index const & nodeIndex1, Index const & nodeIndex2) {
        Value_ABBEL const & nodeValue1(_tree.getNodeFromIndex(nodeIndex1).value());
        Value_ABBEL const & nodeValue2(_tree.getNodeFromIndex(nodeIndex2).value());
        return float(nodeValue1.gain * op)/nodeValue1.visitCount < float(nodeValue2.gain * op)/nodeValue2.visitCount;
       // return nodeValue1.visitCount < nodeValue2.visitCount;
    });
}


void Joueur_MonteCarlo_ABBEL::processLoopingMCTS(Jeu game) {
    _treeLock.lock();
    _canProcessMCTS = true;
    while (_canProcessMCTS && !_tree.isFull()) {
        processMCTS(game);
    }
    _treeLock.unlock();
    _canWrite = true;
}


Index Joueur_MonteCarlo_ABBEL::descent(Index currentNodeIndex, Jeu & game){
    std::vector<Brix> const & legalMoves(findLegalMoves(game));
    if (!legalMoves.empty()) {
        Node_ABBEL & currentNode(_tree.getNodeFromIndex(currentNodeIndex));
        size_t currentNodeChildrenCount(currentNode.childrenCount());

        if (currentNodeChildrenCount < legalMoves.size()) { // Il reste des coups licites non explorés
            std::vector<Brix> exploredMoves;
            for (Index const & childIndex : currentNode.indexesChildren()) {
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
            Index const & bestNodeIndex(findBestQUBC(currentNodeIndex, game.nbCoupJoue()));
            game.joue(_tree.getNodeFromIndex(bestNodeIndex).value().brix);
            return descent(bestNodeIndex,game);
        }
    }
    game.joue(_tree.getNodeFromIndex(currentNodeIndex).value().brix);
    return currentNodeIndex;
}


Index Joueur_MonteCarlo_ABBEL::findBestQUBC(Index nodeIndex, int playedMovesCount) const {
    std::vector<Index> const & indexesChildren(_tree.getNodeFromIndex(nodeIndex).indexesChildren());
    return *std::max_element(indexesChildren.begin(), indexesChildren.end(),
    [this,&nodeIndex,&playedMovesCount](Index const & childNodeIndex1, Index const & childNodeIndex2) {
        return calculateNodeQUBC(childNodeIndex1,nodeIndex,playedMovesCount) < calculateNodeQUBC(childNodeIndex2,nodeIndex, playedMovesCount);
    });
}


float Joueur_MonteCarlo_ABBEL::calculateNodeQUBC(Index nodeIndex, Index parentNodeIndex, int playedMovesCount) const {
    int op = playedMovesCount % 2 == 0 ? 1 : -1;
    Value_ABBEL const & nodeValue(_tree.getNodeFromIndex(nodeIndex).value());
    Value_ABBEL const & parentNodeValue(_tree.getNodeFromIndex(parentNodeIndex).value());
    return (nodeValue.gain * op / nodeValue.visitCount) + sqrtf(POND_C * (logf(parentNodeValue.visitCount) / nodeValue.visitCount));
}


Index Joueur_MonteCarlo_ABBEL::growth(Index currentNodeIndex, Brix const & move) {
    return _tree.addChildFor(currentNodeIndex, Value_ABBEL{0,0,move}).index();
}


int Joueur_MonteCarlo_ABBEL::rollout (Jeu game) {
    while (!game.fini()) {
        std::vector<Brix> const & legalMoves = findLegalMoves(game);
        game.joue(legalMoves[static_cast<size_t>(rand()) % legalMoves.size()]);
    }
    if (game.partie_nulle()) {
        return 0;
    }
    if (game.partie_X()) {
        return 1;
    }
    return -1;
}


void Joueur_MonteCarlo_ABBEL::update (Index currentNodeIndex, int gain) {
    Node_ABBEL & currentNode(_tree.getNodeFromIndex(currentNodeIndex));
    currentNode.value().gain += gain;
    currentNode.value().visitCount++;
    if (!_tree.isRoot(currentNode)) {
        update(currentNode.parentIndex(), gain);
    }
}


std::vector<Brix> Joueur_MonteCarlo_ABBEL::findLegalMoves(Jeu const & game) const{
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


std::vector<Brix>::const_iterator Joueur_MonteCarlo_ABBEL::findBrix(std::vector<Brix> const & brixs, Brix const & b) const {
    return std::find_if(brixs.begin(),brixs.end(),[&b](Brix const & brix) {
        return brix.getAx() == b.getAx() && brix.getOx() == b.getOx() && brix.getAo() == b.getAo() && brix.getOo() == b.getOo();
    });
}
