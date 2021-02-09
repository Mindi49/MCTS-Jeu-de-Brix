#pragma once
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <utility>
#include <functional>
#include <vector>
#include "brix.hh"
#include "jeu.hh"
#include "joueurs/joueur.hh"
#include "joueurs/joueur_random.hh"
#include "joueurs/joueur_manuel.hh"
#include "joueurs/joueur_montecarlo_.hh"
const int TEMPS_POUR_UN_COUP(15000); // millisecondes
//const int TEMPS_POUR_UN_COUP(10);//millisecondes -- temps lors de la compétition
/**********************************************************************************
 *             classe lançant la partie et qui se sert des                        *
 *             mutex pour s'assurer que les joueurs joue                          *
 *             dans les temps                                                     *
 **********************************************************************************/

/*Pour appeler les joueurs
 * M_1 et M_2 instancieront la classe Joueur_monte_carlo
 * M_1 s'appelle MonteCarlo (M) ; M_2 s'appelle StoMonteCarlo (S)
 * MANUEL et MANUEL2 instancieront la classe Joueur_manuel
 * RAND et RAND2 instancieront la classe Joueur_Random
 * RAND s'appelle Random (R) et RAND2 s'appelle Aleatoire (A)
 * MANUEL s'appelle Caro (C) et MANUEL2 s'appelle Vio (V)
 * */
enum class player {M_1, M_2 , MANUEL , MANUEL2, RAND, RAND2} ;
enum class result {NULLE , X , O, ERREUR } ;

/**
 * @brief The Arbitre class
 * classe gerant la partie et les tours des joueurs,
 * et s'ils commencent ou s'ils jouent en deuxième.
 * Le joueur qui commence la partie joue les 'x'
 * Le joueur qui joue en deuxième joue les 'o'.
 * Si un joueur ne joue pas dans les temps, il perd,
 * si un joueur ne joue pas un coup correct (coup correct : brix horizontales ou verticales, brix sur le pateau et vide), il perd.
 */
class Arbitre
{
private:
    std::shared_ptr<Joueur> _joueur1;
    std::shared_ptr<Joueur> _joueur2;

    Jeu _jeu;
    std::vector<Brix> _coups;

    std::vector<std::mutex> _coups_mutex;

    int _nombre_parties;
    int _numero_partie;

    player _player1;
    player _player2;

public:
    /**
     * @brief Arbitre
     * @param player1
     * @param player2
     * @param nombre_parties
     */
    Arbitre(player player1 , player player2, int nombre_parties);


    void initialisation();


    /**
     * @brief challenge
     * @param nombre_parties
     * methode lançant un challenge avec un nombre de parties
     * affiche le nombre de parties gagnées pour chaque participant
     * @return 0 si tout c'est bien passé, 1 sinon
     */
    int challenge();

    /**
     * @brief partie
     * @return le gagnant de la partie (result::X pour le joueur 1, result::O pour joueur 2, result::NULLE en cas de partie nulle, result::ERREUR en cas de problème)
     * lance une partie,
     *
     */
    result partie();
};
