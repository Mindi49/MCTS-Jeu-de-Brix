#include <iostream>
#include "marynode.hh"
#include "treeutil.hh"


int main() {

    srand(time(nullptr));

    // Lecture du fichier et création de l'arbre associé
    //MaryNode * RandomTree = TreeUtil::fileToMary("save.txt"); // lecture du fichier et création de l'arbre associé



    // Écriture dans le fichier de l'arbre
    // création d'un arbre aléatoire
    MaryNode* RandomTree = new MaryNode();
    std::cout << "- Début génération arbre aléatoire -" << std::endl;
    TreeUtil::generateChildren(RandomTree,15,8);
    std::cout << "- Fin génération arbre aléatoire | Début écriture -" << std::endl;

    // écriture de l'arbre créé dans le fichier
    TreeUtil::maryToFile(RandomTree,"save.txt");
    std::cout << "- Fin écriture -" << std::endl;



    // affichage de l'arbre
    // RandomTree->display();
    return 0;
}
