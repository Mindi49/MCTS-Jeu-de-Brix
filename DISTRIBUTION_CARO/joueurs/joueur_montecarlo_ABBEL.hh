#pragma once
#include "joueur.hh"
#include "node_ABBEL.hh"
#include "tree_ABBEL.hh"
#include "treeutil_ABBEL.hh"
#include "value_ABBEL.hh"

#include <thread>

/// Constante de pondération pour le QUBC
#define POND_C 2

class Joueur_MonteCarlo_ABBEL : public Joueur {
    public:
        /**
         * @brief Constructeur qui initalise le joueur et lit le fichier de sauvegarde s'il est déjà créé.
         * Alloue la moitié de la RAM pour l'arbre. Sa capacité ne pourra alors pas excéder cette valeur.
         * @param name      Le nom du joueur.
         * @param player    Vrai si on est le joueur qui commence, faux sinon.
         */
        Joueur_MonteCarlo_ABBEL(std::string name, bool player);

        /**
         * @brief Destructeur qui écrit l'arbre dans le fichier si toutes les actions ont été réalisées.
         */
        virtual ~Joueur_MonteCarlo_ABBEL();

        /**
         * @brief Recherche le meilleur coup à jouer pour ce tour.
         * @param game  L'état du jeu sur lequel on doit jouer.
         * @param move  La brix à modifier par notre prochain coup.
         */
        void recherche_coup(Jeu game, Brix & move) override;



    private:
        /**
         * @brief Met à jour l'arbre pour prendre en compte le dernier coup joué par l'adversaire.
         * @param move  Le coup joué par l'adversaire au dernier tour.
         * @param game  L'état du jeu après que l'adversaire ait joué.
         */
        void playOpponentMove(Brix const & move, Jeu const & game);

        /**
         * @brief Applique le MCTS sur un scénario.
         * @param game  Le jeu courant.
         * @return L'indice du noeud créé (par growth) pendant le MCTS.
         */
        Node_ABBEL::Index processMCTS(Jeu game);

        /**
         * @brief Choisit le meilleur noeud parmi les noeuds enfants.
         * @return L'indice du meilleur noeud.
         */
        Node_ABBEL::Index chooseBestChildNode(int playedMovesCount) const;

        /**
         * @brief Simule en boucle des scénarios de MCTS tant qu'on peut.
         * @param game  Le jeu à partir duquel on simule les scénarios.
         */
        void processLoopingMCTS(Jeu game);

        /**
         * @brief Effectue l'étape de descente de l'algorithme du MCTS.
         * @param currentNodeIndex  L'indice du noeud à partir duquel on fait la descente.
         * @param game              Le jeu sur lequel on effectue la descente.
         * @return L'indice du noeud créé par growth.
         */
        Node_ABBEL::Index descent(Node_ABBEL::Index currentNodeIndex, Jeu & game);

        /**
         * @brief Recherche le noeud enfant ayant le meilleur QUBC pour continuer la descente sur ce noeud.
         * @param nodeIndex L'indice du noeud pour lequel on recherche l'enfant au meilleur QUBC.
         * @param playedMovesCount  Le nombre de coup joué.
         * @return L'indice du noeud enfant ayant le meilleur QUBC.
         */
        Node_ABBEL::Index findBestQUBC(Node_ABBEL::Index nodeIndex, int playedMovesCount) const;

        /**
         * @brief Calcule le QUBC d'un noeud de l'arbre.
         * @param nodeIndex         Le noeud dont on veut calculer le QUBC.
         * @param parentNodeIndex   Le noeud parent du noeud pour lequel on caclule le QUBC.
         * @param playedMovesCount  Le nombre de coup joué.
         * @return Le QUBC du noeud.
         */
        float calculateNodeQUBC(Node_ABBEL::Index nodeIndex, Node_ABBEL::Index parentNodeIndex, int playedMovesCount) const;

        /**
         * @brief Effectue l'étape de growth de l'algorithme du MCTS.
         * @param currentNodeIndex  L'indice du noeud (feuille de l'arbre actuel) auquel on rajoute un enfant.
         * @param move              La brix du coup joué pour atteindre ce noeud.
         * @return L'indice du noeud créé.
         */
        Node_ABBEL::Index growth(Node_ABBEL::Index currentNodeIndex, Brix const & move);

        /**
         * @brief Effectue l'étape de rollout de l'algorithme du MCTS.
         * @param game  Le jeu à partir duquel on lance le rollout.
         * @return Le gain à l'issue de la simulation de partie.
         * -1 si partie perdue, 0 si partie nulle et 1 si partie gagnée.
         */
        int rollout(Jeu game);

        /**
         * @brief Effectue l'étape d'update de l'algorithme du MCTS.
         * @param currentNodeIndex  L'indice du noeud à partir duquel on met à jour les informations.
         * @param gain              Le gain à faire remonter.
         */
        void update(Node_ABBEL::Index currentNodeIndex, int gain);


        /**
         * @brief Trouve l'ensemble des coups légaux à partir d'un état du jeu donné.
         * @param game  L'état du jeu.
         * @return L'ensemble des coups (brix) légaux.
         */
        std::vector<Brix> findLegalMoves(Jeu const & game) const;

        /**
         * @brief Recherche une brix dans un ensemble de brixs.
         * @param brixs Un ensemble de brixs.
         * @param b     La brix que l'on recherche.
         * @return Un iterator pointant sur la brix correspondante.
         */
        std::vector<Brix>::const_iterator findBrix(std::vector<Brix> const & brixs, Brix const & b) const;

        /// L'indice de la racine "courante", à partir duquel faire la recherche
        Node_ABBEL::Index _currentRoot;

        /// L'arbre d'exploration
        static Tree_ABBEL _tree;

        /// Vrai si l'arbre a déjà été créé (lu), faux sinon
        static bool _created;

        /// Vrai tant qu'on peut faire du MCTS, faux sinon
        static bool _canProcessMCTS;

        /// Vrai si on peut écrire dans l'arbre, faux sinon
        static bool _canWrite;

        /// Mutex permettant l'écriture exclusive dans l'arbre
        static std::mutex _treeLock;
};
