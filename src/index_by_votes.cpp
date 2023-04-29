//
// Created by juandiego on 4/28/23.
//

#include "../inc/avl.hpp"
#include "../inc/record.hpp"

int main() {
    std::string heap_file = "./database/movies_and_series.dat";
    std::string index_file = "./database/avl_indexed_by_votes.dat";

    std::function<int(MovieRecord &)> index = [](MovieRecord &movie) { return movie.votes; };
    AVLFile<int, MovieRecord> avl(heap_file, index_file, false, index);

//    avl.queued_report();
    short to_search = 1500;
    for (MovieRecord &record: avl.search(to_search, heap_file)) {
        std::cout << record.to_string() << std::endl;
    }
}
