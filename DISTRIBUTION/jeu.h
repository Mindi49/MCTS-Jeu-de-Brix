#pragma once

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class Jeu {
 private :
  int _graine;
  int _etat;
  int _tab[100];
  int _nombre;

 public :
  Jeu(int graine);
  void reset(); // Remet à la graine le jeu
  bool coup_licite(int); // Vérifie si le coup est licite
  void joue(int); // Joue le coup (sans vérifier qu'il est licite)
  int nb_coups(); // Le nombre de coups possibles
  int& operator[](int i); // Les coups possibles
  bool fini(); // Vérifie si un état final a été atteint (dans ce cas le jeu prend fin)

  void print_coups();
  int etat();
};
