#include "treeutil.hh"
#include <iostream>
#include <stack>
#include <cstdlib>


void TreeUtil::writeNode (MaryNode const * M, std::ofstream & file) {
    file << M->getValue().eval << "\n";             // écriture de la valeur du noeud
    for (auto const & child : M->getNodeSet()) {    // écrit chaque noeud fils
        writeNode(child,file);
    }
    file << "<\n";  // le noeud courant n'a plus de fils, signal de backtrack
}



void TreeUtil::maryToFile (MaryNode const * M, std::string const & filename) {
    std::ofstream file(filename);

    if (file.is_open()) {
        if (M)
            writeNode(M,file); // écriture de l'arbre
        file.close();
    }
}



MaryNode* TreeUtil::fileToMary(std::string const & filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        std::stack<MaryNode*> s;
        // le sommet de pile représente le noeud courant
        std::string line;

        while (std::getline(file,line)) { // pour chaque ligne du fichier
            if (!line.empty()) {
                if (line[0] == '<') {   // symbole de backtrack, le noeud courant (sommet de pile) n'a plus de fils
                    if (s.size() >= 2)
                        s.pop();        // retire le noeud courant de la pile pour traiter le suivant
                    else {              // le noeud racine n'a plus de fils à traiter
                        file.close();
                        return s.top(); // renvoie le noeud racine
                    }
                }
                else {                  // un nouveau noeud à traiter
                    MaryNode* node = new MaryNode({std::stoi(line)});   // création du noeud associé
                    if (!s.empty())                 // si la pile est vide, on n'a pas encore ajouté la racine de l'arbre
                        s.top()->addChild(node);    // ajout du noeud créé en tant que fils du noeud courant
                    s.push(node);   // traitement du noeud lu en tant noeud courant
                }
            }
        }
    }
    return nullptr;
}



void TreeUtil::generateChildren(MaryNode* M, int maxdepth, int maxNbChildren) {
    M->setValue({rand()%100});  // valeur aléatoire du noeud

    if (maxdepth > 1) {         // permet de stopper à une profondeur donnée
        int nbChildren = rand()%maxNbChildren+1;    // choix d'un nombre aléatoire de fils pour le noeud courant

        for (int i = 0; i < nbChildren; i++) {      // création des noeuds fils
            MaryNode* tmp = new MaryNode();
            M->addChild(tmp);
            TreeUtil::generateChildren(tmp, maxdepth-(rand()%2+1), maxNbChildren);
        }
    }
}
