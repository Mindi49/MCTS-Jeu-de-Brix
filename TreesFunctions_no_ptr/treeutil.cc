#include "treeutil.hh"
#include <stack>
#include <cstdlib>


void TreeUtil::writeNode (Node const & node, Tree const & tree, std::ofstream & file) {
    file << node.value().toString() << "\n";  // Écriture de la valeur du noeud
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


void TreeUtil::fileToTree(std::string const & filename, Tree & tree) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::stack<Node::Index> stack;
        // Le sommet de pile représente l'index du noeud courant
        std::string line;
        // Lecture du nombre de noeuds
        if (std::getline(file,line)) {
            tree = Tree(static_cast<size_t>(std::stoi(line)));
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
                        stack.push(tree.setRoot(Value{std::stoi(line)}).index());
                    } else {
                        // Création du noeud associé et ajout du noeud créé en tant que fils du noeud courant
                        Node & child = tree.addChildFor(stack.top(), Value{std::stoi(line)});
                        stack.push(child.index()); // Traitement du noeud lu en tant noeud courant
                    }
                }
            }
        }
    }
}

void TreeUtil::generateRandomTree(Tree & tree, int maxDepth, int maxChildrenCount) {
    generateChildren(tree, tree.setRoot(Value{rand() % 100}), maxDepth, maxChildrenCount);
}

void TreeUtil::generateChildren(Tree & tree, Node & node, int maxDepth, int maxChildrenCount) {
    Node::Index index = node.index();
    if (maxDepth > 1) {     // Permet de stopper à une profondeur donnée
        int childrenCount = rand() % maxChildrenCount + 1;    // Choix d'un nombre aléatoire d'enfants pour le noeud courant
        for (int i(0); i < childrenCount; i++) {
            // Création des noeuds fils
            Node & child = tree.addChildFor(index, Value{rand() % 100}); // Valeur aléatoire du noeud
            TreeUtil::generateChildren(tree, child, maxDepth - (rand() % 2 + 1), maxChildrenCount);
        }
    }
}
