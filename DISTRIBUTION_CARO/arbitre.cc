#include "arbitre.hh"


Arbitre::Arbitre(player player1, player player2, int nombre_parties):
    _coups(nombre_parties,Brix()),
    _coups_mutex(nombre_parties),
    _nombre_parties(nombre_parties),
    _numero_partie(1),
    _player1(player1),
    _player2(player2)
{}

void Arbitre::initialisation()
{
    _joueur1=nullptr;
    _joueur2=nullptr;

    _jeu.reset();


    //si le numero de partie est pair, c'est _joueur1 qui commence
    switch (((_numero_partie%2)? _player1 : _player2)) {
        case player::M_1:
            _joueur1 = std::make_shared<Joueur_MonteCarlo_> ("MonteCarlo",true);
            break;
        case player::M_2:
            _joueur1 = std::make_shared<Joueur_MonteCarlo_> ("StoMonteCarlo",true);
            break;
        case player::MANUEL:
            _joueur1 = std::make_shared<Joueur_Manuel> ("Caro",true);
            break;
        case player::MANUEL2:
            _joueur1 = std::make_shared<Joueur_Manuel> ("Vio",true);
            break;
        case player::RAND:
            _joueur1 = std::make_shared<Joueur_Random> ("Random",true);
            break;
        case player::RAND2:
            _joueur1 = std::make_shared<Joueur_Random> ("Aleatoire",true);
            break;
        default:
            break;
        }

    //si le numero de partie est impair, c'est _joueur2 qui commence
    switch (((!(_numero_partie%2))? _player1 : _player2)) {
        case player::M_1:
            _joueur2 = std::make_shared<Joueur_MonteCarlo_> ("MonteCarlo",false);
            break;
        case player::M_2:
            _joueur2 = std::make_shared<Joueur_MonteCarlo_> ("StoMonteCarlo",false);
            break;
        case player::MANUEL:
            _joueur2 = std::make_shared<Joueur_Manuel> ("Caro",false);
            break;
        case player::MANUEL2:
            _joueur2 = std::make_shared<Joueur_Manuel> ("Vio",false);
            break;
        case player::RAND:
            _joueur2 = std::make_shared<Joueur_Random> ("Random",false);
            break;
        case player::RAND2:
            _joueur2 = std::make_shared<Joueur_Random> ("Aleatoire",false);
            break;
        default:
            break;
        }

}

int Arbitre::challenge()
{
    initialisation(); // Au moins une fois pour que les objets de la ligne qui suit soient définis
    std::cout << "Le challenge de " << _nombre_parties << " parties "
              <<"entre " << _joueur1->nom() << " et " << _joueur2->nom()
             << " commence. " << std::endl;
    int victoire_joueur_1 = 0;
    int victoire_joueur_2 = 0;
    for(int i=0 ; i < _nombre_parties ; i++)
        {
            std::cout << "\n" << "Partie n°" << _numero_partie << " : "<<std::endl<<_jeu;
            result resultat = partie();
            if (resultat == result::ERREUR)
                {
                    std::cerr << "Alerte bug. Sauvez votre terminal et prévenez Me Devred. Merci. " << std::endl;
                    return 1;
                } else if (resultat != result::NULLE)
                (resultat==result::X?
                        ((_numero_partie%2)?
                             victoire_joueur_1++
                           :
                             victoire_joueur_2++ )
                      :
                        (!(_numero_partie%2)?
                             victoire_joueur_1++
                           :
                             victoire_joueur_2++ ));

            std::this_thread::sleep_for (std::chrono::milliseconds(2500)); // temps de latence entre deux parties
            _numero_partie++;
            initialisation();
        }
    std::cout << "FIN DU CHALLENGE\n\t"
              << _joueur1->nom()<< " gagne " << ((_numero_partie%2)? victoire_joueur_1 : victoire_joueur_2)
              << "\n\t"<< _joueur2->nom()<< " gagne " << ((_numero_partie%2) ? victoire_joueur_2 : victoire_joueur_1) << std::endl;
    return 0;
}

result Arbitre::partie()
{
    int tour = 0;
    while(!_jeu.fini())
        {
            bool try_lock = false;
            _coups[_numero_partie-1].setDefinie(false);
            tour++;
            std::cout << "tour : " << tour << std::endl;
            _coups_mutex[_numero_partie-1].unlock();

            std::thread thread_joueur(&Joueur::jouer,
                                      ((tour%2)? (_joueur1) :(_joueur2) ),
                                      _jeu,
                                      std::ref(_coups[_numero_partie-1]),
                    std::ref(_coups_mutex[_numero_partie-1]));

            std::this_thread::sleep_for (std::chrono::milliseconds(TEMPS_POUR_UN_COUP));
            //        std::this_thread::sleep_for (std::chrono::seconds(TEMPS_POUR_UN_COUP));

            if (!_coups_mutex[_numero_partie-1].try_lock()) {
                    std::cerr <<  std::endl << "mutex non rendu " << std::endl;
                    try_lock = true;
                }
            else if(_coups[_numero_partie-1].getDefinie() == false) {
                    std::cerr << "coup invalide Brix non définie" << std::endl;
                }
            else if(!_jeu.coup_licite(_coups[_numero_partie-1],tour)) {
                    std::cerr << "coup invalide " << _coups[_numero_partie-1] << std::endl;
                }

            thread_joueur.detach();

            if(try_lock ||
                    (_coups[_numero_partie-1].getDefinie() == false) ||
                    !_jeu.coup_licite(_coups[_numero_partie-1],tour))
                {
                    if(_jeu.partie_nulle())
                        {
                            std::cout << "partie nulle (" << tour << ")" << std::endl;
                            return result::NULLE;
                        }
                    if(tour%2)
                        {
                            std::cout << _joueur2->nom() <<" gagne ! Nombre de tours : " << tour << std::endl;
                            return result::O; // joueur jouant en 2eme gagne
                        }
                    else
                        {
                            std::cout << _joueur1->nom() <<" gagne ! Nombre de tours : " << tour << std::endl;
                            return result::X; // joueur jouant en 1er gagne
                        }
                }
            //On joue le coup, on l'affiche et on affiche le plateau
            _jeu.joue(_coups[_numero_partie-1]);
            std::cout << ((tour%2) ? _joueur1->nom_abbrege() : _joueur2->nom_abbrege())<<" "<< _coups[_numero_partie-1]
                      << std::endl << _jeu << std::endl;
        }


    if (_jeu.partie_nulle())
        {
            std::cout << std::endl << "Partie nulle" << std::endl;
            return result::NULLE;
        }
    else if (_jeu.partie_X())
        {
            std::cout << std::endl << _joueur1->nom()  <<" gagne. Nombre de tours : " << tour << std::endl;
            return result::X;
        }
    else if (_jeu.partie_O())
        {
            std::cout << std::endl << _joueur2->nom()  <<" gagne. Nombre de tours : " << tour << std::endl;
            return result::O;
        }

    return result::ERREUR;

}

