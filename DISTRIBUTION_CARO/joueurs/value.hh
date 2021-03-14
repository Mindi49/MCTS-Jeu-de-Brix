#pragma once
#include "../brix.hh"

#include <ostream>

/**
 * @brief La valeur contenant les informations liées à l'algorithme MCTS.
 */
struct Value {
    int gain = 0;
    int visitCount = 0;
    Brix brix;

    inline friend std::ostream& operator<< (std::ostream & oss, Value const & value){
        oss << value.toString();
        return oss;
    }

    std::string toString() const {
        return std::to_string(gain) + " " + std::to_string(visitCount) + " "
        + std::to_string(brix.getAx()) + " " + std::to_string(brix.getOx()) + " "
        + std::to_string(brix.getAo()) + " " + std::to_string(brix.getOo());

    }
};
