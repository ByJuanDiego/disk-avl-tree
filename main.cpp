#include "avl.hpp"

template<typename Greater, typename Index>
void menu(int key_size, const std::string &key_name, Greater greater, Index index, const std::string &file_name) {

    AVLFile<text, Record, decltype(greater), decltype(index)> bstFile(file_name, index, greater);
    int option;

    do {
        std::cout << "+===========================================+\n"
                     "║                                           ║\n"
                     "║            __  __                         ║\n"
                     "║           |  \\/  |___ _ _ _  _            ║\n"
                     "║           | |\\/| / -_) ' \\ || |           ║\n"
                     "║           |_|  |_\\___|_||_\\_,_|           ║\n"
                     "║                                           ║\n"
                     "║                                           ║\n"
                     "║     Options:                              ║\n"
                     "║                                           ║\n"
                     "║     [0]: Insert a record                  ║\n"
                     "║     [1]: Search a record                  ║\n"
                     "║     [2]: Read the AVL-Tree File           ║\n"
                     "║     [3]: Exit                             ║\n"
                     "║                                           ║\n"
                     "+===========================================+" << std::endl;

        std::cout << "Enter option: ";
        std::cin >> option;

        switch (option) {
            case 0 : {
                Record record{};
                std::cout << std::endl << std::endl;
                std::cout << "------------ Record Information ------------" << std::endl;
                std::cout << "Code: ";
                read_from_console(record.code, 5);
                std::cout << "Name: ";
                read_from_console(record.name, 20);
                std::cout << "Cycle: ";
                std::cin >> record.cycle;
                bstFile.insert(record);
                std::cout << "The record with code: " << record.code << " was successfully inserted" << std::endl;

                break;
            }
            case 1: {
                char key[key_size];
                std::cout << std::endl;
                std::cout << key_name << ": ";
                read_from_console(key, key_size);

                Record record = bstFile.search(key);
                std::cout << "The record was found: " << record.to_string() << std::endl;
                break;
            }
            case 2: {
                std::cout << std::endl;
                bstFile.read_all();
                break;
            }
            default : {
                break;
            }
        }

        if (option == 2) {
            std::cin.ignore();
        }
        std::cout << "\nPress Enter to continue ...";
        std::cin.get();
        std::system(clear_console);

    } while (option != 3);
}

int main() {
    std::function<bool(text, text)> greater = [](text a, text b) {
        return std::string(a) > std::string(b);
    };

    std::function<text(Record &)> index = [=](Record &record) {
        return record.name;
    };

    menu(20, "Name", greater, index, "./assets/data_indexed_by_name.dat");
    return EXIT_SUCCESS;
}
