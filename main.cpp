#include "avl.hpp"
#include <iomanip>

template<typename Greater, typename Index>
void menu(int key_size, const std::string &key_name, Greater greater, Index index, const std::string &file_name) {

    AVLFile<text, Record, decltype(greater), decltype(index)> bstFile(file_name, false, index, greater);
    std::string command;
    std::cout << "Server [localhost]:" << std::endl;
    std::cout << "Database [cpp]: database" << std::endl;
    std::cout << "Username [cpp]: " << std::endl;
    std::cout << "c++ (17)" << std::endl;
    std::cout << "Type \"help\" for help" << std::endl << std::endl;

    do {
        std::cout << "database=# ";
        std::getline(std::cin, command);
        std::cin.clear();

        if (command == "help") {
            std::cout << "Query\n"
                         "  \\0                 Insert a record\n"
                         "  \\1                 Search a record\n"
                         "  \\2                 Read the AVL-Tree File\n\n"
                         "General\n"
                         "  \\!cls              Clear screen\n"
                         "  \\q                 Exit\n\n";
            continue;
        } else if (command == "\\q") {
            break;
        } else if (command == "\\!cls") {
            std::system(clear_console);
        } else if (command == "\\0") {
            Record record{};
            std::cout << std::endl << std::endl;
            std::cout << "------------ Record Information ------------" << std::endl;
            std::cout << "code: ";
            read_from_console(record.code, 5);
            std::cout << "name: ";
            read_from_console(record.name, 20);
            std::cout << "cycle: ";
            std::cin >> record.cycle;

            bstFile.insert(record);
            std::cout << "The record with " << key_name << ": " << index(record) << " was successfully inserted\n\n";
            std::cin.ignore();
        } else if (command == "\\1") {
            char key[key_size];
            std::cout << key_name << ": ";
            read_from_console(key, key_size);

            std::vector<Record> records = bstFile.search(key);
            for (Record& record : records) {
                std::cout << record.to_string() << std::endl;
            }
            std::cout << std::endl;

        } else if (command == "\\2") {
            bstFile.read_all();
            std::cout << std::endl;
        } else {
            std::cout << "ERROR: syntax error at or near " << std::quoted(command) << std::endl;
            continue;
        }
    } while (true);
}

int main() {
    std::function<bool(text, text)> greater = [](text a, text b) {
        return std::string(a) > std::string(b);
    };

    std::function<text(Record &)> index = [=](Record &record) {
        return record.name;
    };

    menu(20, "name", greater, index, "./database/data_indexed_by_name.dat");
    return EXIT_SUCCESS;
}
