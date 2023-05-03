//
// Created by juandiego on 4/28/23.
//


#include "../inc/avl.hpp"
#include "../inc/record.hpp"

int main() {
    std::string heap_file = "./database/movies_and_series.dat";
    std::string index_file = "./database/avl_indexed_by_dataId.dat";

    std::function<int(MovieRecord &)> index = [](MovieRecord &movie) { return movie.dataId; };
    AVLFile<int, MovieRecord> avl(heap_file, index_file, true, index); // The dataId is a Primary Key

    func::clock clock;
    clock([&](){
        if (!avl) {
            avl.create_index();
        } else {
            std::cout << "AVL Index already exists" << std::endl;
        }
    }, "Create AVL Index by data id");

    int lower_bound{};
    int upper_bound{};
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
