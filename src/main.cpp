#include <utility>
#include <unordered_map>
#include <any>
#include <cstring>

#include "../inc/avl.hpp"
#include "../inc/record.hpp"

std::unordered_map<std::string, std::function<short(MovieRecord &)>> short_getter;
std::unordered_map<std::string, std::any> type;
std::unordered_map<std::string, bool> is_key;

void set_global_variables() {
    type["dataId"] = int(12);
    type["releaseYear"] = short();
    type["endYear"] = short();
    type["contentType"] = new char[16];
    type["title"] = new char[256];
    type["length"] = short();
    type["rating"] = float();
    type["gross"] = int();
    type["certificate"] = new char[16];
    type["votes"] = int();
    type["description"] = new char[512];

//    getter["dataId"] = std::function([](MovieRecord &movie) -> int { return movie.dataId; });
    short_getter["releaseYear"] = std::function([](MovieRecord &movie) -> short { return movie.releaseYear; });
    short_getter["endYear"] = std::function([](MovieRecord &movie) -> short { return movie.endYear; });
    short_getter["length"] = std::function([](MovieRecord &movie) -> short { return movie.length; });
//    getter["contentType"] = std::function([](MovieRecord &movie) -> char * { return movie.contentType; });
//    getter["title"] = std::function([](MovieRecord &movie) -> char * { return movie.title; });
//    getter["rating"] = std::function([](MovieRecord &movie) -> float { return movie.rating; });
//    getter["gross"] = std::function([](MovieRecord &movie) -> int { return movie.gross; });
//    getter["certificate"] = std::function([](MovieRecord &movie) -> char * { return movie.certificate; });
//    getter["votes"] = std::function([](MovieRecord &movie) -> int { return movie.votes; });
//    getter["description"] = std::function([](MovieRecord &movie) -> char * { return movie.description; });

    is_key["dataId"] = true;
    is_key["releaseYear"] = false;
    is_key["endYear"] = false;
    is_key["contentType"] = false;
    is_key["title"] = false;
    is_key["length"] = false;
    is_key["rating"] = false;
    is_key["gross"] = false;
    is_key["certificate"] = false;
    is_key["votes"] = false;
    is_key["description"] = false;
}

struct AVLManager {

    std::string heap_file_name;

    inline static std::string prefix = "./database/avl_indexed_by_";
    inline static std::string suffix = ".dat";

    explicit AVLManager(std::string heap) : heap_file_name(std::move(heap)) {
        set_global_variables();
    }

    void create_index(const std::string &attribute) const {
        auto it = type.find(attribute);
        if (it == type.end()) {
            throw std::runtime_error("Cannot create index in this attribute");
        }

        std::string index_file_name = (prefix + it->first + suffix);

        if (!std::strcmp(type[attribute].type().name(), "s")) {
            AVLFile<short, MovieRecord> avl(heap_file_name, index_file_name, is_key[attribute],
                                            short_getter[attribute]);
        }
    }

    [[nodiscard]] std::vector<MovieRecord> search(const std::string &attribute, const std::any &any) const {
        if (!std::strcmp(any.type().name(), "s")) {
            auto it = type.find(attribute);
            if (it == type.end()) {
                throw std::runtime_error("Cannot create index in this attribute");
            }

            std::string index_file_name = (prefix + it->first + suffix);
            AVLFile<short, MovieRecord> avl(heap_file_name, index_file_name, is_key[attribute],
                                            short_getter[attribute]);
            return avl.search(std::any_cast<short>(any), heap_file_name);
        }

        return {};
    }

    void remove(const std::string &attribute, const std::any &any) const {
        if (!std::strcmp(any.type().name(), "s")) {
            auto it = type.find(attribute);
            if (it == type.end()) {
                throw std::runtime_error("Cannot create index in this attribute");
            }

            std::string index_file_name = (prefix + it->first + suffix);
            AVLFile<short, MovieRecord> avl(heap_file_name, index_file_name, is_key[attribute],
                                            short_getter[attribute]);
            avl.remove(std::any_cast<short>(any), heap_file_name);
        }
    }

    void report(const std::string &attribute) const {
        auto it = type.find(attribute);
        if (it == type.end()) {
            throw std::runtime_error("Cannot create index in this attribute");
        }

        std::string index_file_name = (prefix + it->first + suffix);
        AVLFile<short, MovieRecord> avl(heap_file_name, index_file_name, is_key[attribute], short_getter[attribute]);
        return avl.queued_report();
    }
};

int main() {
    AVLManager manager("./database/movies_and_series.dat");
    manager.create_index("releaseYear");
    manager.create_index("endYear");
    manager.create_index("length");

//    std::any to_delete = (short)1895;
//    manager.remove("releaseYear", to_delete);

    std::any to_search;
    std::vector<MovieRecord> result;
    to_search = short(1895);
    result = manager.search("releaseYear", to_search);
    for (MovieRecord &movie: result) {
        std::cout << movie.to_string() << std::endl;
    }

//    std::cout << std::string(140, '#') << std::endl;
//    to_search = short(1996);
//    result = manager.search("endYear", to_search);
//    for (MovieRecord &movie: result) {
//        std::cout << movie.to_string() << std::endl;
//    }

    std::cout << std::string(140, '#') << std::endl;
    to_search = short(7);
    result = manager.search("length", to_search);
    for (MovieRecord &movie: result) {
        std::cout << movie.to_string() << std::endl;
    }

    return EXIT_SUCCESS;
}
