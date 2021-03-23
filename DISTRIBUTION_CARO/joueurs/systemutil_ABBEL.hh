#pragma once
#include <fstream>
#include <limits>
#include <string>

#define DEFAULT_MEMORY 16000000

/**
 * @brief Utilitaire pour la récupérations d'informations sur la RAM.
 */
class SystemUtil_ABBEL {
    public:
        /**
         * @brief Récupère la taille de la RAM en octets.
         * @return La taille de la RAM en octets.
         */
        static unsigned long getMemorySize() {
            std::string token;
            std::ifstream file("/proc/meminfo");
            while (file >> token) {
                if (token == "MemTotal:") {
                    unsigned long mem;
                    if (file >> mem) {
                        return mem;
                    }
                    else {
                        return DEFAULT_MEMORY;
                    }
                }
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            return DEFAULT_MEMORY;
        }

    private:
        SystemUtil_ABBEL() = delete;
        SystemUtil_ABBEL(SystemUtil_ABBEL const & s) = delete;
        SystemUtil_ABBEL & operator=(SystemUtil_ABBEL const & s) = delete;
};
