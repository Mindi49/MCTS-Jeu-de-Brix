#include <iostream>
#include "marynode.hh"
#include "treeutil.hh"


int main() {
/*
    srand(time(nullptr));

    // Lecture du fichier et création de l'arbre associé
    MaryNode* RandomTree;
    std::cout << "- Début lecture du fichier -" << std::endl;
    auto start_reading = clock();
    RandomTree = TreeUtil::fileToMary("save.txt");
    auto end_reading = clock();
    std::cout << "- Fin lecture du fichier -" << std::endl;




    // Écriture dans le fichier de l'arbre d'un arbre généré aléatoirement
    MaryNode* RandomTree = new MaryNode();
    std::cout << "- Début génération arbre aléatoire -" << std::endl;
    auto start_generating = clock();
    rand();
    TreeUtil::generateChildren(RandomTree,15,8);
    auto end_generating = clock();
    std::cout << "- Fin génération arbre aléatoire -" << std::endl;

    // écriture de l'arbre créé dans le fichier
    std::cout << "- Début écriture -" << std::endl;
    auto start_writing = clock();
    TreeUtil::maryToFile(RandomTree,"save.txt");
    auto end_writing = clock();
    std::cout << "- Fin écriture -" << std::endl;



    // affichage de l'arbre
    std::cout << "- Début affichage de l'arbre -" << std::endl;
    auto start_displaying = clock();
    RandomTree->display();
    auto end_displaying = clock();
    std::cout << "- Fin affichage de l'arbre -" << std::endl;

    std::cout << "Lecture du fichier    : " << (float(end_reading)-start_reading)/CLOCKS_PER_SEC << " s." << std::endl;
    std::cout << "Génération aléatoire  : " << (float(end_generating)-start_generating)/CLOCKS_PER_SEC << " s." << std::endl;
    std::cout << "Écriture du fichier   : " << (float(end_writing)-start_writing)/CLOCKS_PER_SEC << " s." << std::endl;
    std::cout << "Affichage de l'arbre  : " << (float(end_displaying)-start_displaying)/CLOCKS_PER_SEC << " s." << std::endl;
*/



    // TESTS
    float avg_lecture = 0;
    float avg_ecriture = 0;
    int it = 10;

    for (int i(0); i < it; i++) {
        std::cout << "***** ITÉRATION " << i+1 << " *****\n";
        MaryNode* RandomTree;
        auto start_reading = clock();
        RandomTree = TreeUtil::fileToMary("save.txt");
        auto end_reading = clock();

        auto time_lecture = (float(end_reading)-start_reading)/CLOCKS_PER_SEC;
        std::cout << "Lecture du fichier    : " << time_lecture << " s." << std::endl;
        avg_lecture += time_lecture;

        auto start_writing = clock();
        TreeUtil::maryToFile(RandomTree,"save.txt");
        auto end_writing = clock();

        auto time_ecriture = (float(end_writing)-start_writing)/CLOCKS_PER_SEC;
        std::cout << "Écriture du fichier   : " << time_ecriture << " s." << std::endl;
        avg_ecriture += time_ecriture;
        std::cout << std::endl;
    }

    std::cout << "Moyenne lecture   : " << float(avg_lecture)/float(it) << " s." << std::endl;
    std::cout << "Moyenne écriture  : " << float(avg_ecriture)/float(it) << " s." << std::endl;

    return 0;
}
