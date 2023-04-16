//
// Created by juandiego on 4/13/23.
//

#ifndef AVL_HPP
#define AVL_HPP

#include <functional>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

#include "node.hpp"

template<typename KeyType,
        typename RecordType,
        typename Greater,
        typename Index
>
class AVLFile {
private:

    long root;              //< Physical position of the first node

    std::fstream file;      //< File object used to manage disk accesses
    std::string file_name;  //< File name

    // Generic behaviour
    bool primary_key;       //< `True` if indexing a primary key and `false` otherwise
    Index index;            //< Receives a `RecordType` and returns his `KeyType` associated
    Greater greater;        //< Returns `true` if the first parameter is greater than the second and `false` otherwise

    /** Finds and returns the record associated to the key `value`
     *
     * Recursively descends the tree until the record is found
     *     or a null node is reached; in that case, a exception is thrown
     */
    void search(long record_pos, KeyType key, std::vector<Record> &result) {
        /* Base case (I): If this condition is true, it means that the `key` do not exist. */
        if (record_pos == null) {
            file.close();
            std::stringstream ss;
            ss << "The record with key: " << key << " do not exists";
            throw std::runtime_error(ss.str());
        }

        /* Recursion: Searches the node to descend in depth */
        Node<RecordType> node;
        file.seekg(record_pos);
        file >> node;

        if (greater(index(node.data), key)) {
            this->search(node.left, key, result);
            return;
        } else if (greater(key, index(node.data))) {
            this->search(node.right, key, result);
            return;
        }

        /* Base case (II): When this part is reached, it means that the node was found. */
        result.push_back(node.data);    //< Puts the data in the vector

        if (primary_key) {              //< If the tree is indexing a primary key, algorithm finishes here
            return;
        }

        long next = node.next;
        while (next != null) {          // If not, looks for all the siblings of the node
            file.seekg(node.next);
            file >> node;
            result.push_back(node.data);
            next = node.next;
        }
    }

    /** Inserts a new record in the disk file
     *
     * Descends the tree until a null node is reached
     *     in order to put the new record information at the end of the file
     *     and recursively reassign the father physical pointer.
     * If a record with the same key already exists in the file, a exception is thrown
     */
    long insert(long record_pos, RecordType &record) {
        /* Base case (I): If this condition is true, a place for the new record was found. */
        if (record_pos == null) {
            // Create the node and open the file in append mode
            Node node(record);
            file.open(file_name, std::ios::app | std::ios::binary);
            long insertion_position = file.tellp();
            file << node;
            file.close();
            // Returns the insertion position to the immediate previous state to reassign the physical pointer
            return insertion_position;
        }

        /* Recursion: Searches the node to descend to insert */
        Node<RecordType> node;
        file.open(file_name, std::ios::in | std::ios::binary);
        file.seekg(record_pos);
        file >> node;
        file.close();

        long inserted_pos;
        if (greater(index(node.data), index(record))) {
            inserted_pos = insert(node.left, record);

            if (node.left == null) {
                node.left = inserted_pos;
            }
        } else if (greater(index(record), index(node.data))) {
            inserted_pos = insert(node.right, record);

            if (node.right == null) {
                node.right = inserted_pos;
            }
        }

        /* Case base (II): If this condition is true, a node with the same key was found. */
        else {
            if (primary_key) {  //< If the tree is indexing a primary key, an exception is thrown
                std::stringstream ss;
                ss << "Repeated primary key: " << index(record);
                throw std::runtime_error(ss.str());
            }

            //< If not, searches the last node to put the new record next to it
            file.open(file_name, std::ios::in | std::ios::binary);
            file.seekg(record_pos);

            // Stores the necessary information in each step
            Node<RecordType> current = node;   //< The current node (initially equals `node`)
            long next = node.next;             //< His next record physical pointer
            long pos = file.tellg();           //< The record position

            while (next != null) {
                file.seekg(next);
                pos = file.tellg();
                file >> current;
                std::cout << current.to_string() << std::endl;
                next = current.next;
            }
            file.close();

            // Append the new record and store the `insertion_pos`
            file.open(file_name, std::ios::app | std::ios::binary);

            Node<RecordType> insertion_node(record);    //< The node to be inserted
            long insertion_pos = file.tellp();          //< The position before its insertion
            file.write((text) &insertion_node, sizeof(Node<RecordType>));
            file.close();

            current.next = insertion_pos;               //< Assign the last node next pointer to `insertion_pos`

            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            file.seekg(pos);
            file << current;                            //< Writes the last node new pointer
            file.close();

            return EXIT_SUCCESS;                        //< Not tree balancing is needed when inserting a repeated key
        }

        update_height(record_pos, node);
        balance(record_pos, node);
        return EXIT_SUCCESS;
    }

    long height(long record_pos) {
        // If the record position is null, then his height is -1 (empty node)
        if (record_pos == null) {
            return null;
        }

        Node<RecordType> node{};
        // Reads the node information
        file.open(file_name, std::ios::in | std::ios::binary);
        file.seekg(record_pos);
        file >> node;
        file.close();

        // Returns his height
        return node.height;
    }

    inline long balancing_factor(Node<RecordType> &node) {
        long lh = this->height(node.left);
        long rh = this->height(node.right);

        return lh - rh;
    }

    void update_height(long record_pos, Node<RecordType> &node) {
        // Calculates the left and right node heights
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

    /// Verifies if a rotation is needed
    void balance(long record_pos, Node<RecordType> &node) {
        long bf = balancing_factor(node);

        if (bf >= 2) {
            Node<RecordType> left_node{};

            // Reads the left node information
            file.open(file_name, std::ios::in | std::ios::binary);
            file.seekg(node.left);
            file >> left_node;
            file.close();

            if (balancing_factor(left_node) <= -1) {
                left_rotation(node.left, left_node);
            }
            right_rotation(record_pos, node);
        }

        if (bf <= -2) {
            Node<RecordType> right_node{};

            // Reads the right node information
            file.open(file_name, std::ios::in | std::ios::binary);
            file.seekg(node.right);
            file >> right_node;
            file.close();

            if (balancing_factor(right_node) >= 1) {
                right_rotation(node.right, right_node);
            }
            left_rotation(record_pos, node);
        }
    }

    void right_rotation(long record_pos, Node<RecordType> &node) {
        Node<RecordType> left{};
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

    void left_rotation(long record_pos, Node<RecordType> &node) {
        Node<RecordType> right{};
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

public:

    explicit AVLFile(std::string file_name, bool pk, Index _index, Greater _greater)
            : file_name(std::move(file_name)), primary_key(pk), index(_index), greater(_greater) {
        file.open(this->file_name, std::ios::app);
        root = (file.tellp() == 0) ? null : initial_record;
        file.close();
    }

    std::vector<RecordType> search(KeyType key) {
        std::vector<RecordType> result;
        file.open(file_name, std::ios::in | std::ios::binary);
        this->search(root, key, result);
        file.close();
        return result;
    }

    void insert(Record &record) {
        long inserted_position = insert(this->root, record);
        root = ((root == null) ? inserted_position : root);
    }

    void read_all() {
        file.open(file_name, std::ios::in | std::ios::binary);
        Node<RecordType> node;
        long pos = file.tellg();

        while ((file >> node)) {
            std::cout << "[" << pos << "]:\t" << node.to_string() << std::endl;
            pos = file.tellg();
        }

        file.close();
    }
};

#endif //AVL_HPP
