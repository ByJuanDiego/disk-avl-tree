//
// Created by juandiego on 4/28/23.
//

#ifndef AVL_FILE_UTILS_HPP
#define AVL_FILE_UTILS_HPP

#include <cstring>
#include <iostream>
#include <string>

/// Console clear command
#if defined(_WIN32) || defined(_WIN64)
inline const char * clear_console = "cls";
#else
inline const char *clear_console = "clear";
#endif

namespace func {

    template<typename T>
    void copy(T &a, T &b) {
        std::memcpy((char *) &a, (char *) &b, sizeof(T));
    }

    template<typename T>
    void copy(T &a, char *&b) {
        std::memcpy((char *) &a, b, sizeof(T));
    }

    void read_buffer(char buffer[], int size) {
        std::string temp;
        std::getline(std::cin >> std::ws, temp, '\n');
        std::cin.clear();

        for (int i = 0; i < size; ++i) {
            buffer[i] = (i < temp.size())? temp[i] : '\0';
        }

        buffer[size - 1] = '\0';
    }

}


#endif //AVL_FILE_UTILS_HPP
