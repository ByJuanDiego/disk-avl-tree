#include <utility>

#include "../inc/avl.hpp"
#include "../inc/record.hpp"

using fun = std::function<bool(short, short)>;
using idx = std::function<short(MovieRecord &)>;

class AVLManager {

    std::string heap_file_name;
    AVLFile<short, MovieRecord, idx, fun, false> *avl_indexed_by_release_year = nullptr;

public:

    explicit AVLManager(std::string heap) : heap_file_name(std::move(heap)) {
    }

    void create_index(const std::string &index_file_name, const idx &index, const fun &greater) {
        avl_indexed_by_release_year = new AVLFile<short, MovieRecord, idx, fun, false>(heap_file_name, index_file_name,
                                                                                       index, greater);
    }

    void initialize_existing_index(const std::string &index_file_name, const idx &index, const fun &greater) {
        avl_indexed_by_release_year = new AVLFile<short, MovieRecord, idx, fun, false>(index_file_name, index, greater);
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

    void remove(short release_year) {
        if (avl_indexed_by_release_year != nullptr) {
            avl_indexed_by_release_year->remove(release_year, heap_file_name);
        }
    }
};

int main() {
    fun greater = [](short a, short b) {
        return a > b;
    };
    idx index = [](MovieRecord &record) {
        return record.releaseYear;
    };

    AVLManager avl_manager("./database/movies_and_series.dat");
    // avl_manager.create_index("./database/avl_indexed_by_release_year.dat", index, greater);
    avl_manager.initialize_existing_index("./database/avl_indexed_by_release_year.dat", index, greater);

//    for (short i = 1874; i <= 2017; ++i) {
//        if (!avl_manager.query(i).empty()) {
//            avl_manager.remove(i);
//        }
//    }

//    avl_manager.remove(2020);
//    avl_manager.remove(2021);
//    avl_manager.remove(2023);
//    avl_manager.remove(2018);
//    avl_manager.remove(-1);
//    avl_manager.remove(2022);
//    avl_manager.remove(2019);
    avl_manager.report();
    return EXIT_SUCCESS;
}
