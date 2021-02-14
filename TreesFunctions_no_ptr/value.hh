#pragma once
#include <ostream>

/**
 * @brief La valeur contenant les informations liées à l'algorithme MCTS.
 */
struct Value {
    int gain = 0;
    int visitCount = 0;
    bool terminal = false;

    inline friend std::ostream& operator<< (std::ostream & oss, Value const & value){
        oss << value.gain << "/" << value.visitCount << "/" << value.terminal;
        return oss;
    }
};
