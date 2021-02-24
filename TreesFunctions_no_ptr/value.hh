#pragma once
#include <ostream>
#include "../DISTRIBUTION_CARO/brix.hh"

/**
 * @brief La valeur contenant les informations liées à l'algorithme MCTS.
 */
struct Value {
    int gain = 0;
    int visitCount = 0;
    bool terminal = false;
    Brix brix;

    inline friend std::ostream& operator<< (std::ostream & oss, Value const & value){
        oss << value.gain << "/" << value.visitCount << "/" << value.terminal;
        return oss;
    }
};
