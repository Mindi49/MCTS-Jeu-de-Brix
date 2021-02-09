#pragma once

#include <cmath>
#include <iostream>



class Brix {
private :
    int _abscisseX;
    int _ordonneeX;
    int _abscisseO;
    int _ordonneeO;
    bool _definie;


public:

    Brix();//constructeur d'une Brix non definie (i.e. vide)
    Brix(int aX, int oX, int aO, int oO);//constructeur d'une piece, d'une Brix
    int getAx() const{
        return _abscisseX;
    };//accesseur de l'abscisse de la partie x de la pièce
    int getAo() const{
        return _abscisseO;
    };//accesseur de l'abscisse de la partieo de la pièce
    int getOx() const{
        return _ordonneeX;
    };//accesseur de l'ordonnée de la partie x de la pièce
    int getOo() const{
        return _ordonneeO;
    };//accesseur de l'ordonnée de la partie o de la pièce
    bool getDefinie()const{
        return _definie;
    }//accesseur de _definie, retourne false si la piece est considéré comme vide
    void setAx(int aX); //mutateur de l'abscisse de la partie x de la pièce
    void setOx(int oX);//mutateur de l'ordonnée de la partie x de la pièce
    void setAo(int aO); //mutateur de l'abscisse de la partie o de la pièce
    void setOo(int oO);//mutateur de l'ordonnée de la partie o de la pièce
    void setAllCoord(int aX, int oX, int aO, int oO);//mutateur de toutes les coordonnées de la pièce, met _definie à true
    void setDefinie(bool def);//mutateur _definie
    bool horizontale() const;//retourne vraie si la piece est posee horizontalement et que les 2 cases se suivent
    bool verticale() const;//retourne vraie si la piece est posee verticalement et que les 2 cases se suivent
    bool bienformee() const;//retourne vraie si la piece est bien formée (i.e. horizontale ou verticale)
};

std::ostream& operator<<( std::ostream &flux, Brix const& b);
