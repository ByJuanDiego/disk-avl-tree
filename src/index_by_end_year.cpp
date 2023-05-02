//
// Created by juandiego on 4/28/23.
//

#include "../inc/avl.hpp"
#include "../inc/record.hpp"

int main() {
    std::string heap_file = "./database/movies_and_series.dat";
    std::string index_file = "./database/avl_indexed_by_endYear.dat";

    std::function<short(MovieRecord &)> index = [](MovieRecord &movie) { return movie.endYear; };
    AVLFile<short, MovieRecord> avl(heap_file, index_file, false, index);

    short year;
    std::cout << "Enter the movie end year: ";
    std::cin >> year;
    for (MovieRecord &record: avl.search(year)) {
        std::cout << record.to_string() << std::endl;
    }
}
