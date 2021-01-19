#include "jeu.h"

int __chiffre_succ(int chiffre, int index, int puissance, int * tab) {
  int i;
  for (i=1 ; i <= chiffre ; i++) {
      tab[i+index] = i*puissance;
  }
  return chiffre;
}

int __succ(int etat, int index, int puissance, int * tab) {
  if (etat < 10) return __chiffre_succ(etat, index, puissance, tab);
  
  int nb = __chiffre_succ(etat%10, index, puissance, tab);
  return nb+__succ((etat - etat%10) / 10, index+nb, puissance*10, tab);
  
}

Jeu::Jeu(int graine) {
  _graine = graine;
  _etat = graine;
  _nombre = __succ(graine, -1, 1, _tab);
}

void Jeu::reset() {
  _etat = _graine;
  _nombre = __succ(_graine, -1, 1, _tab);
}

bool Jeu::coup_licite(int coup) {
  return !((coup < 0) || (coup >= _nombre));
}
  
void Jeu::joue(int coup) {
  _etat = _etat - _tab[coup];
  int __etat = _etat;
  _nombre = __succ(__etat, -1, 1, _tab);
}

int Jeu::nb_coups() {
  return _nombre;
}

bool Jeu::fini() {
  return _etat == 0;
}

int& Jeu::operator[](int i) {
  if ((i < 0) || (i > _nombre)) {
    std::cout << std::endl << "Erreur de limite."<<'\n';}
  return _tab[i];
}


void Jeu::print_coups() {
  
  int i;
  if (_nombre == 1) {
    printf("%d coup à partir de l'état %d : ", _nombre, _etat);
  }
  else {
    printf("%d coups à partir de l'état %d : ", _nombre, _etat);
  }
  for (i=0 ; i < _nombre; i++) {
    printf("%d ", _tab[i]);
  }
  printf("\n");

}

int Jeu::etat() {
  return _etat;
}
