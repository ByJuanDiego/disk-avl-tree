//
// Created by juandiego on 4/28/23.
//

#include "../inc/avl.hpp"
#include "../inc/record.hpp"

int main() {
    std::string heap_file = "./database/movies_and_series.dat";
    std::string index_file = "./database/avl_indexed_by_length.dat";

    std::function<short(MovieRecord &)> index = [](MovieRecord &movie) { return movie.length; };
    AVLFile<short, MovieRecord> avl(heap_file, index_file, false, index);

    short to_search;
    std::cout << "Enter the movie length in minutes: " ;
    std::cin >> to_search;
    for (MovieRecord &record: avl.search(to_search, heap_file)) {
        std::cout << record.to_string() << std::endl;
    }
}
