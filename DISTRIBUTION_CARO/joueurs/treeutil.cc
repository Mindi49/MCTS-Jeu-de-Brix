#include "treeutil.hh"
#include "../jeu.hh"
#include "../arbitre.hh"

#include <cstdlib>
#include <regex>
#include <stack>

void TreeUtil::writeNode (Node const & node, Tree const & tree, std::ofstream & file) {
    file << node.value() << "\n";  // Écriture de la valeur du noeud
    for (Node::Index const & i : node.indexesChildren()) {
        writeNode(tree.getNodeFromIndex(i), tree, file); // Écrit chaque noeud fils
    }
    file << "<\n";  // Le noeud courant n'a plus d'enfants, signal de backtrack
}

void TreeUtil::treeToFile (Tree const & tree, std::string const & filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << tree.nodesCount() << "\n";
        writeNode(tree.getRoot(), tree, file); // Écriture de l'arbre
        file.close();
    }
}

void TreeUtil::fileToTree(std::string const & filename, Tree & tree, size_t nodeCountAllocation) {
    if(!std::ifstream(filename.c_str()).good()) {
        tree = Tree(nodeCountAllocation);
        tree.setRoot(Value{0,0,Brix(0,0,0,0)});
        return;
    }
    std::ifstream file(filename);
    if (file.is_open()) {
        std::stack<Node::Index> stack;
        // Le sommet de pile représente l'index du noeud courant
        std::string line;
        // Lecture du nombre de noeuds
        if (std::getline(file,line)) {
            if (nodeCountAllocation == 1) {
                nodeCountAllocation = static_cast<size_t>(std::stoi(line));
            }
            tree = Tree(nodeCountAllocation);
        }

        // Lecture de chaque ligne du fichier
        while (std::getline(file,line)) {
            if (!line.empty()) {
                if (line[0] == '<') {   // Symbole de backtrack, le noeud courant (sommet de pile) n'a plus de fils
                    if (stack.size() >= 2)
                        stack.pop();    // Retire le noeud courant de la pile pour traiter le suivant
                    else {              // Le noeud racine n'a plus de fils à traiter
                        file.close();
                        return;
                    }
                }
                else {  // Un nouveau noeud à traiter
                    if (stack.empty()) {    // Si la pile est vide, on n'a pas encore ajouté la racine de l'arbre
                        stack.push(tree.setRoot(lineToValue(line)).index());
                    } else {
                        // Création du noeud associé et ajout du noeud créé en tant que fils du noeud courant
                        Node & child = tree.addChildFor(stack.top(), lineToValue(line));
                        stack.push(child.index()); // Traitement du noeud lu en tant noeud courant
                    }
                }
            }
        }
        throw std::invalid_argument("La structure du fichier " + filename + " est invalide.");
    } else {
        throw std::invalid_argument("Le fichier " + filename + " n'a pas pu être ouvert ou n'existe pas.");
    }
}

Value TreeUtil::lineToValue(std::string const& line) {
    try {
        Value value;
        std::istringstream stm(line) ;
        int word ;
        stm >> word;
        value.gain = word;
        stm >> word;
        value.visitCount = word;
        stm >> word;
        value.brix.setAx(word);
        stm >> word;
        value.brix.setOx(word);
        stm >> word;
        value.brix.setAo(word);
        stm >> word;
        value.brix.setOo(word);
        value.brix.setDefinie(true);
        return value;
    }
    catch (std::exception const &) {
        throw std::invalid_argument("Une ligne du fichier est invalide : '" + line + "'.");
    }
}

void TreeUtil::generateRandomTree(Tree & tree, int maxDepth, int maxChildrenCount) {
    srand(static_cast<unsigned int>(time(nullptr)));
    generateChildren(tree, tree.setRoot(generateRandomValue()), maxDepth, maxChildrenCount);
}

void TreeUtil::generateChildren(Tree & tree, Node & node, int maxDepth, int maxChildrenCount) {
    Node::Index index = node.index();
    if (maxDepth > 1) {     // Permet de stopper à une profondeur donnée
        int childrenCount = rand() % maxChildrenCount + 1;    // Choix d'un nombre aléatoire d'enfants pour le noeud courant
        for (int i(0); i < childrenCount; i++) {
            // Création des noeuds fils
            Node & child = tree.addChildFor(index, generateRandomValue()); // Valeur aléatoire du noeud
            TreeUtil::generateChildren(tree, child, maxDepth - (rand() % 2 + 1), maxChildrenCount);
        }
    }
}

Value TreeUtil::generateRandomValue() {
    int gain = rand() % 50;
    int lose = rand() % 50;
    return Value{gain, gain + lose, Brix(1,1,1,1)};
}
