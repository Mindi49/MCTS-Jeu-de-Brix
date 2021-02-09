#pragma once


#include <vector>
#include "joueur.hh"
#include "../arbitre.hh"



class Joueur_Random : public Joueur
{
public:

    Joueur_Random(std::string nom,bool joueur);


    void recherche_coup(Jeu jeu, Brix & coup) override;
};


