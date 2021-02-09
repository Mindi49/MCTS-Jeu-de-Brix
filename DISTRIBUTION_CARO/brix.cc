#include "brix.hh"

Brix::Brix(){
    _definie=false;
}
Brix::Brix (int aX, int oX, int aO, int oO)
    :
      _abscisseX(aX),
      _ordonneeX(oX),
      _abscisseO(aO),
      _ordonneeO(oO),
      _definie(true)
{}


void Brix::setAx(int aX){
_abscisseX=aX;
}
void Brix::setOx(int oX){
    _ordonneeX=oX;
}

void Brix::setAo(int aO){
    _abscisseO=aO;
}
void Brix::setOo(int oO){
    _ordonneeO=oO;
}
void Brix::setAllCoord(int aX, int oX, int aO, int oO){
    _abscisseX=aX;
    _ordonneeX=oX;
    _abscisseO=aO;
    _ordonneeO=oO;
    _definie=true;

}
void Brix::setDefinie(bool def){
    _definie=def;
}

bool Brix::horizontale() const {
    return ((abs(_abscisseO - _abscisseX) == 1) && (_ordonneeO == _ordonneeX));
}

bool Brix::verticale() const {
    return ((abs (_ordonneeO - _ordonneeX)== 1) && (_abscisseO == _abscisseX));

}

bool Brix::bienformee() const {
    return Brix::verticale() || Brix::horizontale();

}

std::ostream& operator<<( std::ostream &flux, Brix const & b){
    if (!b.getDefinie())
        {
            flux<<"La Brix n'est pas définie";
        }
    else
        {
            flux<<"Brix : x("<< b.getAx()<<" , " <<b.getOx() <<") "
            <<" O("<< b.getAo()<<" , " <<b.getOo() <<")" ;}
    return flux;
}
