//
// Created by juandiego on 4/13/23.
//

#ifndef AVL_HPP
#define AVL_HPP

#define SEEK_ALL(file, pos) \
    file.seekg(pos);        \
    file.seekp(pos);

#define SEEK_ALL_RELATIVE(file, pos, relative) \
    file.seekg(pos, relative);                 \
    file.seekp(pos, relative);

#define DETACH (-2)
#define NOT_DETACH (-3)

#include <functional>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <queue>

#include "node.hpp"

template<
        typename KeyType,
        typename RecordType,
        typename Index = std::function<KeyType(RecordType &)>,
        typename Greater = std::greater<KeyType>
>
class AVLFile {
private:

    long root;              //< Physical position of the first node
    bool primary_key;       //< Is `true` when indexing a primary key and `false` otherwise

    std::fstream file;      //< File object used to manage disk accesses
    std::string file_name;  //< File name

    std::ios_base::openmode flags = (std::ios::out | std::ios::in | std::ios::binary);

    /* Generic purposes member variables */
    Index index;            //< Receives a `RecordType` and returns his `KeyType` associated
    Greater greater;        //< Returns `true` if the first parameter is greater than the second and `false` otherwise

    /*******************************************************************************
    * Finds the record(s) associated to the `key`                                  *
    *                                                                              *
    * Recursively descends the tree until the record is found or a DISK_NULL node       *
    * is reached; in that case, a exception is thrown                              *
    ********************************************************************************/
    void search(long record_pos, KeyType key, std::vector<long> &pointers) {
        /* Base case (I): If this condition is true, it means that the `key` do not exist. */
        if (record_pos == DISK_NULL) {
            return;
        }

        /* Recursion: Searches the node to descend in depth */
        Node<KeyType> node;
        file.seekg(record_pos);
        file >> node;

        if (greater(node.key, key)) {
            this->search(node.left, key, pointers);
            return;
        } else if (greater(key, node.key)) {
            this->search(node.right, key, pointers);
            return;
        }

        /* Base case (II): When this part is reached, it means that the node was found. */
        pointers.push_back(node.data_pointer); //< Puts the data pointer in the vector

        //< If the tree is indexing a primary key, algorithm finishes here.
        if (primary_key) {
            return;
        }

        // If not, looks for all the siblings of the node and includes them in the search.
        long next = node.next;
        while (next != DISK_NULL) {
            file.seekg(node.next);
            file >> node;
            pointers.push_back(node.data_pointer);
            next = node.next;
        }
    }

    /*******************************************************************************
    * Inserts a new record in the disk file.                                       *
    *                                                                              *
    * Descends the tree until a DISK_NULL node is reached in order to put the new       *
    * record information at the end of the file and recursively reassign the       *
    * father physical pointer.                                                     *
    ********************************************************************************/
    long insert(long node_pos, KeyType key, long pointer) {
        /* Base case (I): If this condition is true, a place for the new record was found. */
        if (node_pos == DISK_NULL) {
            // Creates the node and open the file in append mode
            Node<KeyType> node(key, pointer);
            SEEK_ALL_RELATIVE(file, 0, std::ios::end)
            long insertion_position = file.tellp();
            file << node;
            // Returns the insertion position to the immediate previous state to reassign the physical pointer
            return insertion_position;
        }

        /* Recursion: Searches the node to descend to insert */
        Node<KeyType> node;
        SEEK_ALL(file, node_pos)
        file >> node;

        long inserted_pos;
        if (greater(node.key, key)) {
            inserted_pos = insert(node.left, key, pointer);

            if (node.left == DISK_NULL) {
                node.left = inserted_pos;
            }
        } else if (greater(key, node.key)) {
            inserted_pos = insert(node.right, key, pointer);

            if (node.right == DISK_NULL) {
                node.right = inserted_pos;
            }
        }
            /* Base case (II): If `else` is reached, a node with the same key was found */
        else {
            // If the tree is indexing a primary key, an exception is thrown
            if (primary_key) {
                std::stringstream ss;
                ss << "Repeated primary key: " << key;
                file.close();
                throw std::runtime_error(ss.str());
            }

            // If not, the repeated-key new record is stored next to `node` (LIFO).
            // Append the new record at the end of the file and store the `insertion_pos`
            SEEK_ALL_RELATIVE(file, 0, std::ios::end)
            Node<KeyType> insertion_node(key, pointer); //< The node to be inserted
            insertion_node.next = node.next;            //< Moves the pointer to the new record node
            long insertion_pos = file.tellp();          //< Stores the position where the new record begins
            file << insertion_node;                     //< Inserts the record

            SEEK_ALL(file, node_pos)
            node.next = insertion_pos;                  //< Update the new `next`
            file << node;                               //< Writes the last node new pointer

            return EXIT_SUCCESS;                        //< Not tree balancing is needed when inserting a repeated key
        }

        /*******************************************************************************
        * AVL behaviour.                                                               *
        *                                                                              *
        * Note that both actions below are done recursively while the recursion        *
        * goes up from the most depth state to the first call.                         *
        ********************************************************************************/

        // First updates the height of the current node.
        update_height(node_pos, node);
        // After ensure the correctness of the subtree nodes heights, `balance` takes place
        balance(node_pos, node);

        return EXIT_SUCCESS; //< After reading this line, the algorithm goes to the previous state
    }

    //< Seeks and returns the height of the node located at `record_pos`
    long height(long record_pos) {
        // If the record position is DISK_NULL, then his height is -1 (empty node)
        if (record_pos == DISK_NULL) {
            return -1;
        }

        // Reads the node information
        Node<KeyType> node{};
        SEEK_ALL(file, record_pos);
        file >> node;

        return node.height; //< Returns his height
    }

    /// Returns the balancing factor of a `node`
    inline long balancing_factor(Node<KeyType> &node) {
        long lh = this->height(node.left);
        long rh = this->height(node.right);

        return lh - rh;
    }

    /// Updates the `node` height after a new record is inserted in his subtree
    void update_height(long node_pos, Node<KeyType> &node) {
        // Calculates the left and right node heights
        long lh = this->height(node.left);
        long rh = this->height(node.right);

        // Recalculates the node height
        node.height = std::max(lh, rh) + 1;

        // Overwrites node height in disk
        SEEK_ALL(file, node_pos);
        file << node; //< Updates the height
    }

    /// Verifies if a rotation is needed
    void balance(long node_pos, Node<KeyType> &node) {
        long bf = balancing_factor(node); //< Calculates the balancing factor

        // If the balancing factor is greater or equal than 2, then the tree is unbalanced to the left
        if (bf >= 2) {
            Node<KeyType> left_node{};
            SEEK_ALL(file, node.left);
            file >> left_node; //< Reads the left node information

            if (balancing_factor(left_node) <= -1) {
                left_rotation(node.left, left_node);
            }
            right_rotation(node_pos, node); //< Makes the right rotation
        }

        // If the balancing factor is lesser or equal than -2, then the tree is unbalanced to the right
        if (bf <= -2) {
            Node<KeyType> right_node{};
            SEEK_ALL(file, node.right);
            file >> right_node; //< Reads the right node information

            if (balancing_factor(right_node) >= 1) {
                right_rotation(node.right, right_node);
            }
            left_rotation(node_pos, node); //< Makes the left rotation
        }
    }

    void right_rotation(long node_pos, Node<KeyType> &node) {
        Node<KeyType> left{};
        long l_pos = node.left;

        SEEK_ALL(file, l_pos);
        file >> left;

        node.left = left.right;
        left.right = l_pos;

        SEEK_ALL(file, node_pos);
        file << left;

        SEEK_ALL(file, l_pos);
        file << node;

        update_height(l_pos, node);
        update_height(node_pos, left);
    }

    void left_rotation(long node_pos, Node<KeyType> &node) {
        Node<KeyType> right{};
        long r_pos = node.right;

        SEEK_ALL(file, r_pos);
        file >> right;

        node.right = right.left;
        right.left = r_pos;

        SEEK_ALL(file, node_pos);
        file << right;

        SEEK_ALL(file, r_pos);
        file << node;

        update_height(r_pos, node);
        update_height(node_pos, right);
    }

    int remove(long node_pos, KeyType key, std::vector<long> &pointers) {
        if (node_pos == DISK_NULL) {
            std::stringstream ss;
            ss << "The record with the key: " << key << " do not exists";
            file.close();
            throw std::runtime_error(ss.str());
        }

        Node<KeyType> node;
        SEEK_ALL(file, node_pos)
        file >> node;

        int reallocate = NOT_DETACH;
        if (greater(node.key, key)) {
            reallocate = this->remove(node.left, key, pointers);

            if (reallocate == DETACH) {
                node.left = DISK_NULL;
            }
        } else if (greater(key, node.key)) {
            reallocate = this->remove(node.right, key, pointers);

            if (reallocate == DETACH) {
                node.right = DISK_NULL;
            }
        } else {
            if (node.left == DISK_NULL && node.right == DISK_NULL) {
                if (pointers.empty()) {
                    push_all(node_pos, pointers);
                }

                return DETACH;
            } else if (node.left != DISK_NULL && node.right == DISK_NULL) {
                if (pointers.empty()) {
                    push_all(node_pos, pointers);
                }

                Node<KeyType> left_node;
                SEEK_ALL(file, node.left)
                file >> left_node;
                node = left_node;
                node.left = left_node.left;
                node.right = left_node.right;

            } else if (node.left == DISK_NULL && node.right != DISK_NULL) {
                if (pointers.empty()) {
                    push_all(node_pos, pointers);
                }

                Node<KeyType> right_node;
                SEEK_ALL(file, node.right)
                file >> right_node;
                node = right_node;
                node.left = right_node.left;
                node.right = right_node.right;

            } else {
                if (pointers.empty()) {
                    push_all(node_pos, pointers);
                }

                Node<KeyType> successor = this->find_successor(node.right);
                reallocate = this->remove(node.right, successor.key, pointers);
                node = successor;
                if (reallocate == DETACH) {
                    node.right = DISK_NULL;
                }
            }
        }

        // First updates the height of the current node.
        update_height(node_pos, node);
        // After ensure the correctness of the subtree nodes heights, `balance` takes place
        balance(node_pos, node);

        return NOT_DETACH;
    }

    inline void push_all(long node_pos, std::vector<long> &pointers) {
        while (node_pos != DISK_NULL) {
            Node<KeyType> current;
            SEEK_ALL(file, node_pos)
            file >> current;
            pointers.push_back(current.data_pointer);
            if (primary_key) {
                break;
            }
            node_pos = current.next;
        }
    }

    Node<KeyType> find_successor(long right_ref) {
        Node<KeyType> node;

        SEEK_ALL(file, right_ref)
        file >> node;
        long leftmost = node.left;

        while (leftmost != DISK_NULL) {
            file.seekg(leftmost);
            file >> node;
            leftmost = node.left;
        }

        return node;
    }

public:

    // This constructor creates the AVL index in disk, `heap_file_name` contains the path to the `crude data` file
    // that stores all the records in a `heap file`.
    explicit AVLFile(const std::string &heap_file_name, std::string index_file_name,
                     bool is_key, Index _index, Greater _greater = Greater())
            : root(DISK_NULL), index(_index),
              greater(_greater), primary_key(is_key),
              file_name(std::move(index_file_name)) {

        // Verifies if the index already exists
        file.open(file_name, std::ios::app);
        long size = file.tellp();
        file.close();
        if (size > 0) {
            root = INITIAL_RECORD;
            return;
        }

        // If not, then creates it
        std::fstream heap_file(heap_file_name, std::ios::in | std::ios::binary);
        file.open(file_name, std::ios::out | std::ios::binary);
        file.close();

        RecordType record;
        long seek = 0;
        while (heap_file.read((char *) &record, sizeof(RecordType))) {
            if (!record.removed) {
                this->insert(index(record), seek);
            }
            seek = heap_file.tellg();
        }

        heap_file.close();
    }

    ~AVLFile() = default;

    void insert(KeyType key, long pointer) {
        file.open(file_name, flags);
        long inserted_position = this->insert(root, key, pointer);
        file.close();

        root = ((root == DISK_NULL) ? inserted_position : root);
    }

    std::vector<RecordType> search(KeyType key, const std::string &heap_file_name) {
        std::vector<long> pointers;
        file.open(file_name, std::ios::in | std::ios::binary);
        this->search(root, key, pointers);
        file.close();

        std::vector<RecordType> records;
        records.reserve(pointers.size());

        std::fstream heap_file(heap_file_name, std::ios::in | std::ios::binary);
        for (long pointer: pointers) {
            RecordType record;
            heap_file.seekg(pointer);
            heap_file.read((char *) &record, sizeof(RecordType));
            if (record.removed) {
                continue;
            }
            records.push_back(record);
        }

        heap_file.close();
        return records;
    }

    void remove(KeyType key, const std::string &heap_file_name) {
        std::vector<long> pointers;
        file.open(file_name, flags);
        int detach_root = this->remove(this->root, key, pointers);
        file.close();

        std::fstream heap_file(heap_file_name, flags);
        for (long pointer: pointers) {
            RecordType record{};
            SEEK_ALL(heap_file, pointer);
            heap_file.read((char *) &record, sizeof(RecordType));
            record.removed = true;
            SEEK_ALL(heap_file, pointer);
            heap_file.write((char *) &record, sizeof(RecordType));
        }
        heap_file.close();

        if (detach_root == DETACH) {
            file.open(file_name, std::ios::out);
            file.close();
            root = DISK_NULL;
        }
    }

    void queued_report() {
        file.open(file_name, flags);
        Node<KeyType> input_node;
        file.seekg(0);

        if (!(file >> input_node)) {
            throw std::runtime_error("No records to display");
        }

        std::queue<std::pair<long, Node<KeyType>>> queue;
        queue.push({0, input_node});

        while (!queue.empty()) {
            auto &[pos, node] = queue.front();
            queue.pop();

            std::cout << "[" << pos << "]: " << node.to_string() << std::endl;

            if (node.left != DISK_NULL) {
                file.seekg(node.left);
                file >> input_node;
                queue.push({node.left, input_node});

            }

            if (node.right != DISK_NULL) {
                file.seekg(node.right);
                file >> input_node;
                queue.push({node.right, input_node});
            }
        }

        file.close();
    }
};

#endif //AVL_HPP
