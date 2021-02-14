#include "tree.hh"
#include "treeutil.hh"
#include "node.hh"

#include <iostream>

void iterationFileTest(int iterationCount) {
    float readingAverage = 0;
    float writingAverage = 0;
    for (int i(0); i < iterationCount; i++) {
        std::cout << "***** ITÉRATION " << i+1 << " *****" << std::endl;
        Tree tree;

        // Lecture
        float startReading = clock();
        TreeUtil::fileToTree("save.txt", tree);
        float endReading = clock();

        float readingTime = (endReading-startReading)/CLOCKS_PER_SEC;
        std::cout << "Lecture du fichier    : " << readingTime << " s." << std::endl;
        readingAverage += readingTime;

        // Écriture
        float startWriting = clock();
        TreeUtil::treeToFile(tree,"save.txt");
        float endWriting = clock();

        float writingTime = (endWriting-startWriting)/CLOCKS_PER_SEC;
        std::cout << "Écriture du fichier   : " << writingTime << " s." << std::endl << std::endl;
        writingAverage += writingTime;
    }

    if (iterationCount != 0) {
        std::cout << "Moyenne de lecture   : " << readingAverage/iterationCount << " s." << std::endl;
        std::cout << "Moyenne d'écriture  : " << writingAverage/iterationCount << " s." << std::endl;
    }
}

void generateRandom() {
    Tree randomTree;
    TreeUtil::generateRandomTree(randomTree, 7, 3);
    TreeUtil::treeToFile(randomTree, "brixTree.txt");

    Tree fileTree;
    TreeUtil::fileToTree("brixTree.txt", fileTree);
    fileTree.display();
}

int main() {
    //iterationFileTest(10); // Invalide avec la nouvelle structure
    generateRandom();
    return 0;
}
