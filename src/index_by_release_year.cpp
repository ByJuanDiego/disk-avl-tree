//
// Created by juandiego on 4/28/23.
//

#include "../inc/avl.hpp"
#include "../inc/record.hpp"

int main() {
    std::string heap_file = "./database/movies_and_series.dat";
    std::string index_file = "./database/avl_indexed_by_releaseYear.dat";

    std::function<short(MovieRecord &)> index = [](MovieRecord &movie) { return movie.releaseYear; };
    AVLFile<short, MovieRecord> avl(heap_file, index_file, false, index);

    short lower_bound {};
    short upper_bound {};
    do {
        std::cout << "lower bound: ";
        std::cin >> lower_bound;
        std::cout << "upper bound: ";
        std::cin >> upper_bound;
        std::cout << std::endl;
    } while (upper_bound <= lower_bound);

    for (MovieRecord &movie: avl.range_search(lower_bound, upper_bound)) {
        std::cout << movie.to_string() << std::endl;
    }
}
