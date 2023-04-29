//
// Created by juandiego on 4/28/23.
//

#include "../inc/avl.hpp"
#include "../inc/record.hpp"

int main() {
    std::string heap_file = "./database/movies_and_series.dat";
    std::string index_file = "./database/avl_indexed_by_rating.dat";

    std::function<float(MovieRecord &)> index = [](MovieRecord &movie) { return movie.rating; };
    AVLFile<float, MovieRecord> avl(heap_file, index_file, false, index);

    float to_search = 6.3f;
    for (MovieRecord &record: avl.search(to_search, heap_file)) {
        std::cout << record.to_string() << std::endl;
    }
}
