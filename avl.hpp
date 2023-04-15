//
// Created by juandiego on 4/13/23.
//

#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>
#include <fstream>
#include <utility>

#include "node.hpp"

class AVLFile {
private:

    long root;              //< Physical position of the first node
    std::fstream file;      //< File object used to manage disk accesses
    std::string file_name;  //< File name

    /** Finds and returns the record associated to the key `value`
     *
     * Recursively descends the tree until the record is found
     *     or a null node is reached; in that case, a exception is thrown
     */
    Record search(long record_pos, char value[5]);

    /** Inserts a new record in the disk file
     *
     * Descends the tree until a null node is reached
     *     in order to put the new record information at the end of the file
     *     and recursively reassign the father physical pointer.
     * If a record with the same key already exists in the file, a exception is thrown
     */
    long insert(long record_pos, Record &record);

    long height(long record_pos);

    void update_height(long record_pos, Node &node);

    void balance(long record_pos);

    inline long balancing_factor(long record_pos, Node& node) {
        file.open(file_name, std::ios::in | std::ios::binary);

        file.seekg(record_pos);
        file >> node;
        file.close();

        long lh = this->height(node.left);
        long rh = this->height(node.right);

        std::cout << "lh: " << lh << " rh: " << rh << std::endl;
        return lh - rh;
    }

    void right_rotation(long record_pos, Node &node);

    void left_rotation(long record_pos, Node &node);

public:

    explicit AVLFile(std::string file_name) : file_name(std::move(file_name)) {
        file.open(this->file_name, std::ios::app);
        this->root = (file.tellp() == 0) ? null : initial_record;
        file.close();
    }

    Record search(char value[5]) {
        file.open(file_name, std::ios::in | std::ios::binary);
        Record result = search(this->root, value);
        file.close();
        return result;
    }

    void insert(Record &record) {
        long inserted_position = insert(this->root, record);
        root = ((root == null) ? inserted_position : root);
    }

    void read_all() {
        file.open(file_name, std::ios::in | std::ios::binary);

        Node node;
        while (file >> node) {
            std::cout << node.to_string() << std::endl;
        }

        file.close();
    }
};

Record AVLFile::search(long record_pos, char *value) {
    if (record_pos == null) {
        file.close();
        throw std::runtime_error("The record with code " + std::string(value) + " do not exists");
    }

    Node node;
    file.seekg(record_pos);
    file >> node;

    if (std::string(node.data.cod) > std::string(value)) {
        return search(node.left, value);
    } else if (std::string(node.data.cod) < std::string(value)) {
        return search(node.right, value);
    }

    return node.data;
}

long AVLFile::insert(long record_pos, Record &record) {
    if (record_pos == null) {
        Node node(record);
        file.open(file_name, std::ios::app | std::ios::binary);
        long insertion_position = file.tellp();
        file << node;
        file.close();
        return insertion_position;
    }

    Node node;
    file.open(file_name, std::ios::in | std::ios::binary);
    file.seekg(record_pos);
    file >> node;
    file.close();
    std::cout << node.to_string() << std::endl;

    long inserted_pos;
    if (std::string(record.cod) < std::string(node.data.cod)) {
        inserted_pos = insert(node.left, record);

        if (node.left == null) {
            node.left = inserted_pos;

            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            file.seekp(record_pos);
            file << node;
            file.close();
        }
    } else if (std::string(record.cod) > std::string(node.data.cod)) {
        inserted_pos = insert(node.right, record);

        if (node.right == null) {
            node.right = inserted_pos;

            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            file.seekp(record_pos);
            file << node;
            file.close();
        }
    } else {
        throw std::runtime_error("Repeated primary key: " + std::string(record.cod));
    }

    update_height(record_pos, node);
    balance(record_pos);
    return EXIT_SUCCESS;
}

long AVLFile::height(long record_pos) {
    // If the record position is null, then his height is -1 (empty node)
    if (record_pos == null) {
        return null;
    }

    Node node{};
    // Reads the node information
    file.open(file_name, std::ios::in | std::ios::binary);
    file.seekg(record_pos);
    file >> node;
    file.close();

    // Returns his height
    return node.height;
}

void AVLFile::update_height(long record_pos, Node &node) {
    long lh = this->height(node.left);
    long rh = this->height(node.right);

    // Recalculate the node height
    node.height = std::max(lh, rh) + 1;

    // Writes the new node height in disk
    file.open(file_name, std::ios::out | std::ios::in | std::ios::binary);
    file.seekp(record_pos);
    file << node;
    file.close();
}

void AVLFile::balance(long record_pos) {
    Node node {};
    long bf = this->balancing_factor(record_pos, node);

    if (bf >= 2) {
        Node left_node {};
        if (balancing_factor(node.left, left_node) <= -1) {
            left_rotation(node.left, left_node);
        }
        right_rotation(record_pos, node);
    }

    if (bf <= -2) {
        Node right_node{};
        if (balancing_factor(node.right, right_node) >= 1) {
            right_rotation(node.right, right_node);
        }
        left_rotation(record_pos, node);
    }
}

void AVLFile::right_rotation(long record_pos, Node &node) {
    Node left{};
    file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    long l_pos = node.left;

    file.seekg(l_pos);
    file >> left;

    node.left = left.right;
    left.right = l_pos;

    file.seekp(record_pos);
    file << left;
    file.seekp(l_pos);
    file << node;
    file.close();

    update_height(l_pos, node);
    update_height(record_pos, left);
}

void AVLFile::left_rotation(long record_pos, Node &node) {
    Node right{};
    file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    long r_pos = node.right;

    file.seekg(r_pos);
    file >> right;

    node.right = right.left;
    right.left = r_pos;

    file.seekp(record_pos);
    file << right;
    file.seekp(r_pos);
    file << node;
    file.close();

    update_height(r_pos, node);
    update_height(record_pos, right);
}

#endif //AVL_HPP
