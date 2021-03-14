#pragma once
#include "node.hh"
#include "tree.hh"
#include "value.hh"

#include <fstream>

/**
 * @brief Utilitaire pour le stockage d'arbres n-aires dans des fichiers.
 */
class TreeUtil {
    public:
        /**
         * @brief Crée un arbre n-aire à partir d'un fichier texte.
         * @param filename  Nom du fichier à lire.
         * @param tree      L'arbre n-aire qui sera construit à partir du fichier.
         * @throw invalid_argument Si le fichier n'existe pas, n'a pas pu être ouvert ou
         * possède une syntaxe invalide.
         */
        static void fileToTree (std::string const & filename, Tree & tree, size_t nodeCountAllocation=1);

        /**
         * @brief Écrit dans un fichier un arbre n-aire.
         * @param tree      L'arbre à écrire.
         * @param filename  Le nom du fichier dans lequel est réalisée l'écriture.
         */
        static void treeToFile (Tree const & tree, std::string const & filename);

        /**
         * @brief Génère l'arbre aléatoire avec les spécificités passées en paramètre.
         * @param tree              L'arbre dont il faut générer les noeuds.
         * @param maxDepth          La profondeur maximale de l'abre.
         * @param maxChildrenCount  Le nombre maximum d'enfants que peut avoir chaque noeud.
         */
        static void generateRandomTree(Tree & tree, int maxDepth, int maxChildrenCount);

    private:
        /**
         * @brief Écrit dans le fichier le noeud courant.
         * Écrit de manière récursive chacun de ses noeuds fils.
         * À la fin de l'écriture d'un noeud, écrit le symbole '<' pour signaler que le noeud
         * courant n'a plus aucun enfant. On fait ici un système de backtrack.
         *
         * Chaque noeud va consulter un de ses enfants, qui consultera à son tour un enfant etc.
         * S'il n'a pas de noeud fils, alors il rendra la main à son parents. Celui-ci continuera de
         * consulter ses autres enfants s'il en a, et ainsi de suite.
         *
         * @param node  Le noeud courant.
         * @param tree  L'arbre dont il faut écrire les noeuds.
         * @param file  Le fichier dans lequel on est en train d'écrire.
         */
        static void writeNode (Node const & node, Tree const & tree, std::ofstream & file);

        /**
         * @brief Génère à partir d'un noeud racine un arbre aléatoire de manière récursive.
         * @param tree              L'arbre dont il faut générer les noeuds.
         * @param node              Le noeud auquel on souhaite ajouter les enfants.
         * @param maxDepth          La profondeur maximale de l'abre.
         * @param maxChildrenCount  Le nombre maximum d'enfants que peut avoir chaque noeud.
         */
        static void generateChildren(Tree & tree, Node & node, int maxDepth, int maxChildrenCount);

        /**
         * @brief Génère une valeur aléatoire.
         * @return La valeur aléatoire créée.
         */
        static Value generateRandomValue();

        /**
         * @brief Convertit une ligne d'un fichier contenant un arbre en une valeur.
         * @param line La ligne contenant les informations d'une valeur à convertir.
         * @throw invalid_argument Si la ligne est mal formatée.
         * @return La valeur correspondant à la ligne.
         */
        static Value lineToValue(std::string const& line);

        TreeUtil() = delete;

        TreeUtil(TreeUtil const & t) = delete;

        TreeUtil & operator=(TreeUtil const & t) = delete;
};
