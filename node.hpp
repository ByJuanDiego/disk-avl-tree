//
// Created by juandiego on 4/14/23.
//

#ifndef AVL_FILE_NODE_HPP
#define AVL_FILE_NODE_HPP

#include <string>
#include <sstream>

/// nullptr file representation
#define null (-1)
#define not_reallocate (-2)
#define initial_record (0)

/// Console clear command
#if defined(_WIN32) || defined(_WIN64)
inline const char * clear_console = "cls";
#else
inline const char *clear_console = "clear";
#endif

void read_from_console(char buffer[], int size) {
    std::string temp;
    std::getline(std::cin >> std::ws, temp, '\n');
    std::cin.clear();

    for (int i = 0; i < size; i++) {
        buffer[i] = (i < temp.size()) ? temp[i] : '\0';
    }

    buffer[size - 1] = '\0';
}

struct MovieRecord {
    int dataId{};
    char contentType[16]{'\0'};
    char title[256]{'\0'};
    short length{};
    short releaseYear{};
    short endYear{};
    int votes{};
    float rating{};
    int gross{};
    char certificate[16]{'\0'};
    char description[512]{'\0'};
    bool removed{};

    std::string to_string() {
        std::stringstream ss;
        ss << "("
           << dataId << ", " << contentType << ", " << title << ", " << length << ", " << releaseYear << ", "
           << endYear << ", " << votes << ", " << rating << ", " << gross << ", " << certificate
           << ", " << std::boolalpha << removed << ")";
        return ss.str();
    }
};

template<typename KeyType>
struct Node {
    KeyType key{};
    long data_pointer = null;
    long left = null;
    long right = null;
    long height = 0;
    long next = null;
    bool removed = false;

    explicit Node() = default;

    explicit Node(KeyType key, long physical_position) : key(key), data_pointer(physical_position) {}

    std::string to_string() {
        std::stringstream ss;
        ss << "<key: " << key << ", pointer: " << data_pointer << ", height: " << height << ", left: " << left
           << ", right: " << right << ", next: " << next << ", removed: " << std::boolalpha << removed << ">";
        return ss.str();
    }
};

template<typename RecordType>
std::ostream &operator<<(std::ostream &os, Node<RecordType> &node) {
    os.write((char *) &node, sizeof(Node<RecordType>));
    return os;
}

template<typename RecordType>
std::istream &operator>>(std::istream &is, Node<RecordType> &node) {
    is.read((char *) &node, sizeof(Node<RecordType>));
    return is;
}

#endif //AVL_FILE_NODE_HPP
