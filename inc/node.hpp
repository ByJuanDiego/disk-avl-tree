//
// Created by juandiego on 4/14/23.
//

#ifndef AVL_FILE_NODE_HPP
#define AVL_FILE_NODE_HPP

#include <string>
#include <sstream>

/// nullptr file representation
#define DISK_NULL (-1)
#define INITIAL_RECORD (0)

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

template<typename KeyType>
struct Node {
    KeyType key{};
    long data_pointer = DISK_NULL;
    long left = DISK_NULL;
    long right = DISK_NULL;
    long height = 0;
    long next = DISK_NULL;

    explicit Node() = default;

    explicit Node(KeyType key, long physical_position) : key(key), data_pointer(physical_position) {}

    Node<KeyType> &operator=(const Node<KeyType> &other) {
        key = other.key;
        next = other.next;
        data_pointer = other.data_pointer;
        return *this;
    }

    std::string to_string() {
        std::stringstream ss;
        ss << "<key: " << key << ", pointer: " << data_pointer << ", height: " << height << ", left: " << left
           << ", right: " << right << ", next: " << next << ">";
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
