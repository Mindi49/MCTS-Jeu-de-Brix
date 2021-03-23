#include "treeutil_ABBEL.hh"
#include "../jeu.hh"
#include "../arbitre.hh"

#include <cstdlib>
#include <regex>
#include <stack>

void TreeUtil_ABBEL::writeNode (Node_ABBEL const & node, Tree_ABBEL const & tree, std::ofstream & file) {
    file << node.value() << "\n";  // Écriture de la valeur du noeud
    for (Node_ABBEL::Index const & i : node.indexesChildren()) {
        writeNode(tree.getNodeFromIndex(i), tree, file); // Écrit chaque noeud fils
    }
    file << "<\n";  // Le noeud courant n'a plus d'enfants, signal de backtrack
}

void TreeUtil_ABBEL::treeToFile (Tree_ABBEL const & tree, std::string const & filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << tree.nodesCount() << "\n";
        writeNode(tree.getRoot(), tree, file); // Écriture de l'arbre
        file.close();
    }
}

void TreeUtil_ABBEL::fileToTree(std::string const & filename, Tree_ABBEL & tree, size_t nodeCountAllocation) {
    if(!std::ifstream(filename.c_str()).good()) {
        tree = Tree_ABBEL(nodeCountAllocation);
        tree.setRoot(Value_ABBEL{0,0,Brix(0,0,0,0)});
        return;
    }

    FILE *file;
    file = fopen(filename.c_str(), "r");
    if (file != NULL) {
        std::stack<Node_ABBEL::Index> stack;
        // Le sommet de pile représente l'index du noeud courant
        bool backtrack;
        int treeNodesCount = TreeUtil_ABBEL::readNextInt(file,backtrack);
        if (nodeCountAllocation == 1) {
            nodeCountAllocation = static_cast<size_t>(treeNodesCount);
        }
        tree = Tree_ABBEL(nodeCountAllocation);

        // Lecture de chaque ligne du fichier
        while (!feof(file)) {
            int item = TreeUtil_ABBEL::readNextInt(file,backtrack);
            if (backtrack) { // Symbole de backtrack, le noeud courant (sommet de pile) n'a plus de fils
                if (stack.size() >= 2) {
                    stack.pop();    // Retire le noeud courant de la pile pour traiter le suivant
                }
                else {              // Le noeud racine n'a plus de fils à traiter
                    fclose(file);
                    return;
                }
            }
            else { // Un nouveau noeud à traiter
                Value_ABBEL value;
                value.gain = item;
                value.visitCount = TreeUtil_ABBEL::readNextInt(file,backtrack);
                value.brix.setAx(TreeUtil_ABBEL::readNextInt(file,backtrack));
                value.brix.setOx(TreeUtil_ABBEL::readNextInt(file,backtrack));
                value.brix.setAo(TreeUtil_ABBEL::readNextInt(file,backtrack));
                value.brix.setOo(TreeUtil_ABBEL::readNextInt(file,backtrack));

                if (stack.empty()) {    // Si la pile est vide, on n'a pas encore ajouté la racine de l'arbre
                    stack.push(tree.setRoot(value).index());
                }
                else {
                    // Création du noeud associé et ajout du noeud créé en tant que fils du noeud courant
                    Node_ABBEL & child = tree.addChildFor(stack.top(), value);
                    stack.push(child.index()); // Traitement du noeud lu en tant noeud courant
                }
            }
        }
        throw std::invalid_argument("La structure du fichier " + filename + " est invalide.");
    }
    else {
        throw std::invalid_argument("Le fichier " + filename + " n'a pas pu être ouvert ou n'existe pas.");
    }
}

void TreeUtil_ABBEL::generateRandomTree(Tree_ABBEL & tree, int maxDepth, int maxChildrenCount) {
    srand(static_cast<unsigned int>(time(nullptr)));
    generateChildren(tree, tree.setRoot(generateRandomValue()), maxDepth, maxChildrenCount);
}

void TreeUtil_ABBEL::generateChildren(Tree_ABBEL & tree, Node_ABBEL & node, int maxDepth, int maxChildrenCount) {
    Node_ABBEL::Index index = node.index();
    if (maxDepth > 1) {     // Permet de stopper à une profondeur donnée
        int childrenCount = rand() % maxChildrenCount + 1;    // Choix d'un nombre aléatoire d'enfants pour le noeud courant
        for (int i(0); i < childrenCount; i++) {
            // Création des noeuds fils
            Node_ABBEL & child = tree.addChildFor(index, generateRandomValue()); // Valeur aléatoire du noeud
            TreeUtil_ABBEL::generateChildren(tree, child, maxDepth - (rand() % 2 + 1), maxChildrenCount);
        }
    }
}

Value_ABBEL TreeUtil_ABBEL::generateRandomValue() {
    int gain = rand() % 50;
    int lose = rand() % 50;
    return Value_ABBEL{gain, gain + lose, Brix(1,1,1,1)};
}
