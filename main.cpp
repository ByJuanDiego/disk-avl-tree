#include <iomanip>
#include <any>
#include <utility>

#include "avl.hpp"

class AVLManager {

    std::string heap_file_name;
    AVLFile<short, MovieRecord, std::function<short(MovieRecord &)>, std::function<bool(short,
                                                                                        short)>, false> *avl_indexed_by_release_year = nullptr;

public:

    explicit AVLManager(std::string heap) : heap_file_name(std::move(heap)) {
    }

    void create_index(const std::string &index_file_name, const std::function<short(MovieRecord &)> &index,
                      const std::function<bool(short, short)> &greater) {
        avl_indexed_by_release_year = new AVLFile<short, MovieRecord, std::function<short(
                MovieRecord &)>, std::function<bool(short, short)>, false>(heap_file_name, index_file_name, index,
                                                                           greater);
    }

    void initialize_existing_index(const std::string &index_file_name, const std::function<short(MovieRecord &)> &index,
                                   const std::function<bool(short, short)> &greater) {
        avl_indexed_by_release_year = new AVLFile<short, MovieRecord, std::function<short(
                MovieRecord &)>, std::function<bool(short, short)>, false>(index_file_name, index, greater);
    }

    std::vector<MovieRecord> query(short release_year) {
        if (avl_indexed_by_release_year != nullptr) {
            return avl_indexed_by_release_year->search(release_year, heap_file_name);
        }

        return {};
    }

    void report() {
        if (avl_indexed_by_release_year != nullptr) {
            avl_indexed_by_release_year->queued_report();
        }
    }
};

int main() {
    std::function<bool(short, short)> greater = [](short a, short b) {
        return a > b;
    };
    std::function<short(MovieRecord &)> index = [](MovieRecord &record) {
        return record.releaseYear;
    };

    AVLManager avl_manager("./database/movies_and_series.dat");
//    avl_manager.create_index("./database/avl_indexed_by_release_year.dat", index, greater);
    avl_manager.initialize_existing_index("./database/avl_indexed_by_release_year.dat", index, greater);
//    avl_manager.report();

    for (MovieRecord& record : avl_manager.query(1999)) {
        std::cout << record.to_string() << std::endl;
    }

//
//    size_t total = 0;
//    for (short i=1874; i <= 2023; ++i) {
//        std::vector<MovieRecord> records = avl_manager.query(i);
//        if (!records.empty()) {
//            total += records.size();
//        }
//    }
//
//    total += avl_manager.query(-1).size();
//    std::cout << "total of records: " << total << std::endl;

    return EXIT_SUCCESS;
}
