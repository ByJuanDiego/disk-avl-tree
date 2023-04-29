//
// Created by juandiego on 4/28/23.
//

#include "../inc/avl.hpp"
#include "../inc/record.hpp"

// Este indice no tiene mucho sentido ya que
// contentType solo es o "movie" o "tvSeries"

int main() {
    std::string heap_file = "./database/movies_and_series.dat";
    std::string index_file = "./database/avl_indexed_by_contentType.dat";

    std::function<char *(MovieRecord &)> index = [](MovieRecord &movie) { return movie.contentType; };
    std::function<bool(char[16], char[16])> greater = [](char a[16], char b[16]) -> bool {
        return std::string(a) > std::string(b);
    };

    AVLFile<char[16], MovieRecord, decltype(index), decltype(greater)> avl(heap_file, index_file, false, index, greater);

    char to_search[16];
    std::cout << "Enter the content type: ";
    func::read_buffer(to_search, 16);
    for (MovieRecord& record : avl.search(to_search, heap_file)) {
        std::cout << record.to_string() << std::endl;
    }
}
