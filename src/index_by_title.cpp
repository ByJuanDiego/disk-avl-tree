//
// Created by juandiego on 4/29/23.
//

#include "../inc/avl.hpp"
#include "../inc/record.hpp"
#include "../inc/utils.hpp"

int main() {
    std::string heap_file = "./database/movies_and_series.dat";
    std::string index_file = "./database/avl_indexed_by_title.dat";

    std::function<char *(MovieRecord &)> index = [](MovieRecord &movie) { return movie.title; };
    std::function<bool(char[256], char[256])> greater = [](char a[256], char b[256]) -> bool {
        return std::string(a) > std::string(b);
    };

    AVLFile<char[256], MovieRecord, decltype(index), decltype(greater)> avl(heap_file, index_file, false, index, greater);

//    avl.queued_report();
    char to_search[256];

    std::cout << "Enter the movie title: ";
    func::read_buffer(to_search, 256);

    for (MovieRecord& record : avl.search(to_search, heap_file)) {
        std::cout << record.to_string() << std::endl;
    }
}
