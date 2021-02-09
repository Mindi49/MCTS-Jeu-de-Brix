#include "jeu.hh"



Jeu::Jeu() {
    reset();
}

void Jeu::reset(){
    // Initialisation du plateau
    for (auto & ligne : _plateau)
        for (auto & colonne : ligne)
            colonne = '0';

    _alignementX = false;
    _alignementO = false;
    //_nombre = NB_TOUR_MAX;
    _etat = Etat::PARTIE_NON_TERMINEE;
    _nb_tours = 0;
}

bool Jeu::coordValide(Brix const & b) const{
    return ((b.getAo() >= 0) && (b.getAo() < MAX_LARGEUR)
            &&(b.getAx() >= 0) && (b.getAx() < MAX_LARGEUR)
            && (b.getOo() >= 0) && (b.getOo() < MAX_HAUTEUR)
            && (b.getOx() >= 0) && (b.getOx() < MAX_HAUTEUR));
}

//On suppose b est verticale ou horizontale.
bool Jeu::voisin(Brix const & b) const{
    int min;
    if (b.horizontale())
        {if (b.getAx()<b.getAo())

                {if ((b.getAx() == 0) && _plateau[b.getOx()][2]== '0')
                        return false;
                    else if ((b.getAo()== MAX_LARGEUR -1) && _plateau[b.getOx()][MAX_LARGEUR - 3]== '0')
                        return false;
                    else return (_plateau[b.getOx()][b.getAx()-1] !='0') || (_plateau[b.getOx()][b.getAo()+1] !='0');
                }
            else {if ((b.getAo() == 0) && _plateau[b.getOx()][2]== '0')
                        return false;
                    else if ((b.getAx()== MAX_LARGEUR -1) && _plateau[b.getOx()][MAX_LARGEUR - 3]== '0')
                        return false;
                    return (_plateau[b.getOx()][b.getAo()-1] !='0') || (_plateau[b.getOx()][b.getAx()+1] !='0');
                };
        }
    else if (b.verticale())
        {   min=b.getOo()<b.getOx()?b.getOo():b.getOx();
            if ((b.getAx() == 0) && _plateau[min][1]== '0')
                return false;
            else if ((b.getAx()== MAX_LARGEUR -1) && _plateau[min][MAX_LARGEUR - 2]== '0')
                return false;
            else return (_plateau[min][b.getAx() -1] !='0') || (_plateau[min][b.getAx() +1] !='0');
        }
    else return false;
    return true;
}

int Jeu::nbCoupJoue() const
{
    return _nb_tours;
}

bool Jeu::coup_licite(Brix const & coup, int nb) const {
    if (!coup.getDefinie()
          ||  !coordValide(coup)
          || !coup.bienformee()
          || _plateau[coup.getOo()][coup.getAo()]!='0'
          || _plateau[coup.getOx()][coup.getAx()] != '0')
        return false;
    if  (coup.verticale())
        {
            int min ;
            min=coup.getOo()<coup.getOx()?coup.getOo():coup.getOx();
            if (min != 0 && _plateau[min-1][coup.getAo()] == '0')
                return false;
        };
    if (coup.horizontale())
        {
        if ((coup.getOx()!=0 && _plateau[coup.getOx()-1][coup.getAx()] == '0')||
                (coup.getOo()!=0 && _plateau[coup.getOo()-1][coup.getAo()] == '0'))
        return false;
        };
    if ((nb !=1) && (coup.getOx() == 0 || coup.getOo() == 0) && !voisin(coup))
        return false;
    return true;
}

void Jeu::joue(Brix const & coup) {
    _nb_tours++;
    _plateau[coup.getOo()][coup.getAo()]= 'o';
    _plateau[coup.getOx()][coup.getAx()] ='x';
    int min;



    //doit déterminer la valeur de _alignementX , _alignementO ET changer l'etat en fonction,

    //recherche d'alignements horizontaux ou verticaux si la piece est verticale
    if (coup.verticale())
        {
            min=coup.getOo()<coup.getOx()?coup.getOo():coup.getOx();

            //recherche d'alignement verticale en extension
            if (min>=3)
                {
                    if ((_plateau[min][coup.getAx()]==_plateau[min-1][coup.getAx()])
                            && (_plateau[min][coup.getAx()]==_plateau[min-2][coup.getAx()])
                            && (_plateau[min][coup.getAx()]==_plateau[min-3][coup.getAx()]))
                        {
                            if (_plateau[min][coup.getAx()]=='x')
                                _alignementX = true;
                            else _alignementO = true;
                        }
                }

            //recherche d'alignements horizontaux en intension

            //etage min
            //on regarde à droite
            int cpt = 0,i =coup.getAx()+1;
            while(i<MAX_LARGEUR && (_plateau[min][coup.getAx()]==_plateau[min][i])){
                    cpt ++;
                    i++;
                };
            //on regarde à gauche
            i =coup.getAx()-1;
            while(i>=0 && (_plateau[min][coup.getAx()]==_plateau[min][i])){
                    cpt ++;
                    i--;
                };
            if (cpt >= 3)
                {
                    if (_plateau[min][coup.getAx()]=='x')
                        _alignementX = true;
                    else _alignementO = true;
                }
            //etage min +1
            cpt = 0;i =coup.getAx()+1;
            //on regarde à droite
            while(i<MAX_LARGEUR && (_plateau[min+1][coup.getAx()]==_plateau[min+1][i])){
                    cpt ++;
                    i++;
                };
            //on regarde à gauche
            i =coup.getAx()-1;
            while(i>=0 && (_plateau[min+1][coup.getAx()]==_plateau[min+1][i])){
                    cpt ++;
                    i--;
                };
            if (cpt >= 3)
                {
                    if (_plateau[min+1][coup.getAx()]=='x')
                        _alignementX = true;
                    else _alignementO = true;
                }
        }
//recherche d'alignements horizontaux ou verticaux si la piece est horizontale
else if (coup.horizontale())
{
    //recherche des alignements verticaux en extension
    if(coup.getOx()>=3)
        { if ((_plateau[coup.getOx()][coup.getAx()]==_plateau[coup.getOx()-1][coup.getAx()])
                    && (_plateau[coup.getOx()][coup.getAx()]==_plateau[coup.getOx()-2][coup.getAx()])
                    && (_plateau[coup.getOx()][coup.getAx()]==_plateau[coup.getOx()-3][coup.getAx()]))
                _alignementX=true;
            if ((_plateau[coup.getOo()][coup.getAo()]==_plateau[coup.getOo()-1][coup.getAo()])
                    && (_plateau[coup.getOo()][coup.getAo()]==_plateau[coup.getOo()-2][coup.getAo()])
                    && (_plateau[coup.getOo()][coup.getAo()]==_plateau[coup.getOo()-3][coup.getAo()]))
                _alignementO=true;
        }
    //recherche des alignements horizontaux en extension
    min=coup.getAo()<coup.getAx()?coup.getAo():coup.getAx();
    if (min>=3)
        {
            //recherche d'un alignement à gauche
            if ((_plateau[coup.getOx()][min]==_plateau[coup.getOx()][min-1])
                    && (_plateau[coup.getOx()][min]==_plateau[coup.getOx()][min-2])
                    && (_plateau[coup.getOx()][min]==_plateau[coup.getOx()][min-3]))
                {
                    if (_plateau[coup.getOx()][min]=='x')
                        _alignementX = true;
                    else _alignementO = true;
                }
        }
    if ((min+1)<MAX_LARGEUR-3)
        {
            //recherche d'un alignement à droite
            if ((_plateau[coup.getOx()][min+1]==_plateau[coup.getOx()][min+2])
                    && (_plateau[coup.getOx()][min+1]==_plateau[coup.getOx()][min+3])
                    && (_plateau[coup.getOx()][min+1]==_plateau[coup.getOx()][min+4]))
                {
                    if (_plateau[coup.getOx()][min+1]=='x')
                        _alignementX = true;
                    else _alignementO = true;
                }
        }
}

//On recherche les alignements en diagonale
//On s'occupe des x
    //on regarde en haut à droite
    int cpt = 0,i =coup.getAx()+1, j = coup.getOx()+1;
    while(i<MAX_LARGEUR && j<MAX_HAUTEUR && (_plateau[coup.getOx()][coup.getAx()]==_plateau[j][i])){
            cpt ++;
            i++;
            j++;
        };
    //on regarde en bas à gauche
    i =coup.getAx()-1;j = coup.getOx()-1;
    while(i>=0 && j>=0 && (_plateau[coup.getOx()][coup.getAx()]==_plateau[j][i])){
            cpt ++;
            i--;
            j--;
        };
    if (cpt >= 3)
        {
            _alignementX = true;
        }

    //on regarde en haut à gauche
    cpt = 0;i =coup.getAx()-1; j = coup.getOx()+1;
    while(i>=0 && j<MAX_HAUTEUR && (_plateau[coup.getOx()][coup.getAx()]==_plateau[j][i])){
            cpt ++;
            i--;
            j++;
        };
    //on regarde en bas à droite
    i =coup.getAx()+1,j = coup.getOx()-1;
    while(i<MAX_LARGEUR && j>=0 && (_plateau[coup.getOx()][coup.getAx()]==_plateau[j][i])){
            cpt ++;
            i++;
            j--;
        };
    if (cpt >= 3)
        {
            _alignementX = true;
        }


    //On s'occuppe des o
    //on regarde en haut à droite
    cpt = 0,i =coup.getAo()+1, j = coup.getOo()+1;
    while(i<MAX_LARGEUR && j<MAX_HAUTEUR && (_plateau[coup.getOo()][coup.getAo()]==_plateau[j][i])){
            cpt ++;
            i++;
            j++;
        };
    //on regarde en bas à gauche
    i =coup.getAo()-1;j = coup.getOo()-1;
    while(i>=0 && j>=0 && (_plateau[coup.getOo()][coup.getAo()]==_plateau[j][i])){
            cpt ++;
            i--;
            j--;
        };
    if (cpt >= 3)
        {
            _alignementO = true;
        }

    //on regarde en haut à gauche
    cpt = 0;i =coup.getAo()-1; j = coup.getOo()+1;
    while(i>=0 && j<MAX_HAUTEUR && (_plateau[coup.getOo()][coup.getAo()]==_plateau[j][i])){
            cpt ++;
            i--;
            j++;
        };
    //on regarde en bas à droite
    i =coup.getAo()+1,j = coup.getOo()-1;
    while(i<MAX_LARGEUR && j>=0 && (_plateau[coup.getOo()][coup.getAo()]==_plateau[j][i])){
            cpt ++;
            i++;
            j--;
        };
    if (cpt >= 3)
        {
            _alignementO = true;
        }


//Je mets à jour état
if (_alignementO && !_alignementX)
_etat = Etat::ALIGNEMENT_O;

if (_alignementX && !_alignementO)
_etat = Etat::ALIGNEMENT_X;

if ((_nb_tours == NB_PIECE_MAX) || (_alignementX && _alignementO))
_etat = Etat::PARTIE_NULLE;

}


bool Jeu::fini() const {
    return (_etat != Etat::PARTIE_NON_TERMINEE);
}


bool Jeu::partie_nulle() const {
    return (_etat == Etat::PARTIE_NULLE);
}

bool Jeu::partie_O() const {
    return _etat == Etat::ALIGNEMENT_O;
}

bool Jeu::partie_X() const{
    return _etat ==Etat::ALIGNEMENT_X;
}

std::ostream& operator<<( std::ostream &flux, Jeu const& jeu ){
//void Jeu::afficher( std::ostream &flux) const {
    int j;
    for(int i=MAX_HAUTEUR -1; i>=0; i--){
            flux<<'|';
            auto ligne =jeu.plateau()[i];
            for (auto colonne : ligne)
                if (colonne == '0')
                    flux<< " |";
                else if (colonne == 'x')
                    flux<< "x|";
                else flux<< "o|";
            flux<<"  "<<i<<std::endl;
        }
    flux<<std::endl << '|';
    for (j=0 ; j<MAX_LARGEUR ; j++) {
            flux << j << '|';
        }
    flux << std::endl;
    return flux;
}

