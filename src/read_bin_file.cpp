//
// Created by juandiego on 4/28/23.
//

#include <fstream>
#include <iostream>
#include <cstring>

#include "../inc/record.hpp"

int main() {
    MovieRecord record{};
    std::ifstream file("./database/movies_and_series.dat", std::ios::in | std::ios::binary);

    while (file.read((char *) &record, sizeof(MovieRecord))) {
        std::cout << record.description << std::endl;
    }

    file.close();
    return EXIT_SUCCESS;
}
