#pragma once
#include "marynode.hh"
#include <string>
#include <fstream>


/**
 * @brief Utilitaire pour le stockage d'arbres n-aires dans des fichiers
 */
class TreeUtil {
    public:
        /**
         * @brief Crée un arbre n-aire à partir d'un fichier texte
         * @param filename  nom du fichier à lire
         * @return L'arbre n-aire construit à partir du fichier
         */
        static MaryNode* fileToMary (std::string const & filename);

        /**
         * @brief Écrit dans un fichier un arbre n-aire
         * @param M         l'arbre à écrire
         * @param filename  le nom du fichier dans lequel est réalisée l'écriture
         */
        static void maryToFile (MaryNode const * M, std::string const & filename);

        /**
         * @brief Génère à partir d'un noeud racine un arbre aléatoire de manière récursive
         * @param M             le noeud racine
         * @param maxdepth      la profondeur maximale de l'abre
         * @param maxNbChildren le nombre maximum d'enfants que peut avoir chaque noeur
         */
        static void generateChildren(MaryNode* M, int maxdepth, int maxNbChildren);

    private:
        /**
         * @brief Écrit dans le fichier le noeud courant.
         * Écrit de manière récursive chacun de ses noeuds fils.
         * A la fin de l'écriture d'un noeud, écrit le symbole '<' pour signaler que le noeud
         * courant n'a plus aucun fils. On fait ici un système de backtrack.
         *
         * Chaque noeud va consulter un fils, qui consultera a son tour un fils etc.
         * S'il a n'a pas de fils, alors il rendra la main à son père. Celui-ci continuera de
         * consulter ses autres fils s'il en a, et ainsi de suite.
         *
         * @param M     le noeud courant
         * @param file  le fichier dans lequel on est en train d'écrire
         */
        static void writeNode (MaryNode const * M, std::ofstream & file);
        TreeUtil()=delete;
};
