#include "joueur_manuel.hh"


Joueur_Manuel::Joueur_Manuel(std::string nom, bool joueur)
    :Joueur(nom,joueur)
{}

/*char Joueur_Manuel::nom_abbrege() const
{
    return nom()[0];
}
*/
void Joueur_Manuel::recherche_coup(Jeu jeu, Brix &coup)
{
  int aX, oX, aO, oO;
  std::cout<<nom_abbrege()<<" donnez abscisse de X, ordonnee de X, abscisse de O, ordonne de O :";
  std::cin>>aX>>oX>>aO>>oO;
  coup.setAllCoord(aX, oX, aO, oO);

}
