//
// Created by juandiego on 4/28/23.
//

#include <fstream>
#include <iostream>
#include <cstring>

#include "../inc/record.hpp"
#include "../inc/utils.hpp"

int main() {
    MovieRecord record{};
    std::ifstream file("./database/movies_and_series.dat", std::ios::in | std::ios::binary);

    func::clock clock;

    clock([&](){
        while (file.read((char *) &record, sizeof(MovieRecord))) {
            if (record.votes == -1) {
                std::cout << record.to_string() << std::endl;
            }
        }
    }, "\"read bin file\"");

    file.close();
    return EXIT_SUCCESS;
}
