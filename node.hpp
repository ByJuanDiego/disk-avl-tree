//
// Created by juandiego on 4/14/23.
//

#ifndef AVL_FILE_NODE_HPP
#define AVL_FILE_NODE_HPP

#include <string>
#include <sstream>

/// nullptr file representation
#define null (-1)
#define initial_record (0)

/// Console clear command
#if defined(_WIN32) || defined(_WIN64)
inline const char * clear_console = "cls";
#else
inline const char *clear_console = "clear";
#endif

using text = char *;

void read_from_console(char buffer[], int size) {
    std::string temp;
    std::getline(std::cin >> std::ws, temp, '\n');
    std::cin.clear();

    for (int i = 0; i < size; i++) {
        buffer[i] = (i < temp.size()) ? temp[i] : ' ';
    }

    buffer[size - 1] = '\0';
}

struct Record {
    char code[5];
    char name[20];
    int cycle;

    std::string to_string() {
        std::stringstream ss;
        ss << "(" << code << ", " << name << ", " << cycle << ")";
        return ss.str();
    }
};

template<typename RecordType>
struct Node {
    RecordType data{};
    long left = null;
    long right = null;
    long height = 0;

    explicit Node() = default;

    explicit Node(RecordType record) : data(record) {}

    std::string to_string() {
        std::stringstream ss;
        ss << "<" << data.to_string() << ", height: " << height << ", left: " << left << ", right: " << right << ">";
        return ss.str();
    }
};

template<typename RecordType>
std::ostream &operator<<(std::ostream &os, Node<RecordType> &node) {
    os.write((text) &node, sizeof(Node<RecordType>));
    return os;
}

template<typename RecordType>
std::istream &operator>>(std::istream &is, Node<RecordType> &node) {
    is.read((text) &node, sizeof(Node<RecordType>));
    return is;
}

#endif //AVL_FILE_NODE_HPP
