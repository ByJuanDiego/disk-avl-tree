//
// Created by juandiego on 4/29/23.
//

#include "../inc/record.hpp"
#include "../inc/avl.hpp"

int main() {
    std::string heap_file = "./database/movies_and_series.dat";
    std::string index_file = "./database/avl_indexed_by_description.dat";

    std::function<char *(MovieRecord &)> index = [](MovieRecord &movie) { return movie.description; };
    std::function<bool(char[512], char[512])> greater = [](char a[512], char b[512]) -> bool {
        return std::string(a) > std::string(b);
    };

    AVLFile<char[512], MovieRecord, decltype(index), decltype(greater)> avl(heap_file, index_file, false, index, greater);

    char to_search[512];
    std::cout << "Enter the movie description: ";
    func::read_buffer(to_search, 512);

    for (MovieRecord &record: avl.search(to_search, heap_file)) {
        std::cout << record.to_string() << std::endl;
    }
}
