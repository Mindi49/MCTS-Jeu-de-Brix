#include "joueur_random.hh"

Joueur_Random::Joueur_Random(std::string nom, bool joueur)
    :Joueur(nom,joueur)
{}



/*char Joueur_Random::nom_abbrege() const
{
    return 'R';
}
*/

void Joueur_Random::recherche_coup(Jeu jeu, Brix &coup)
{
    std::vector<Brix> coupValide;
    Brix b_canditate;
    int tour = jeu.nbCoupJoue()+1;//la b_candidate devra être valide au tour auquel on va la jouer,i.e. au tour suivant
    int place_coup_joue, taille;
    int aX, oX, aO, oO; //coordonnees de la Brix que l'on va joué

    for (int i = 0; i<MAX_LARGEUR;i++) //i abscisse donc numero de colonne
        {
            int j=0; //j ordonnee donc numero de ligne
            while(j<MAX_HAUTEUR && jeu.plateau()[j][i]!='0') //dans plateau l'ordonnee en premier
                {
                    j++;
                }
            if (j<MAX_HAUTEUR)//On est tombé sur une case vide, i.e. contenant '0'
                {
                    //Cherchons des coups valides à partir de cette case vide.
                    b_canditate.setAllCoord(i,j ,i,j+1); //brix verticale dont le bottom est X
                    if (jeu.coup_licite(b_canditate,tour))
                        {
                            coupValide.push_back(b_canditate);
                        }

                    b_canditate.setAllCoord(i,j+1,i,j); //brix verticale dont le bottom est O
                    if (jeu.coup_licite(b_canditate,tour))
                        {
                            coupValide.push_back(b_canditate);
                        }

                    b_canditate.setAllCoord(i,j ,i+1,j); //brix horizontale commençant par X
                    if (jeu.coup_licite(b_canditate,tour))
                        {
                            coupValide.push_back(b_canditate);
                        }

                    b_canditate.setAllCoord(i+1,j,i,j); //brix terminant commençant par X
                    if (jeu.coup_licite(b_canditate,tour))
                        {
                            coupValide.push_back(b_canditate);
                        }
                    //fin des brix valide à cette abscisse et cette ordonnee
                }
        }//fin du for.
    //std::cout<<coupValide<<std::endl;
    //On choisit un coup au hazard
    taille =coupValide.size();
    place_coup_joue= taille == 1 ?  0 : rand() % (taille-1);
    aO= coupValide[place_coup_joue].getAo();
    oO= coupValide[place_coup_joue].getOo();
    aX= coupValide[place_coup_joue].getAx();
    oX= coupValide[place_coup_joue].getOx();
    coup.setAllCoord(aX, oX, aO, oO);

    //std::this_thread::sleep_for (std::chrono::milliseconds(rand() % (TEMPS_POUR_UN_COUP+1)));

}


