# avl-file

Auto-balanced binary search tree implementation that works with secondary memory to store records. Supports insertion,
key-based search, range-based search and remove.

## Member functions

- ```operator bool()```
    
    - Casts as true the class instance if his `index file` already exists and false otherwise.


- ```create index()```

    - Iterates over the `heap file` and insert record per record each key in the `index file`
    - If the index already exists, deletes the existing index in order to re-build a new index.


- ```insert(KeyType key, long pointer)```

    -  Inserts a new `node(key, pointer)` in the `index file`. Such node stores a `reference` to a record in a `heap file` and its correspondent `key` associated.
    -  Internally, descends the `AVL` until a `DISK_NULL` node is reached in order to put the new node at the end of the `index file` and reassign the father pointer.
    -  If a node with the same `key` already exists there are two cases: if the `index file` is indexing a non-repeatable key, in which case an exception is thrown; otherwise, the nodes that shares the same `key` get linked in a `forward list`.


    
- ```search(KeyType key)```
    - Searches all the records such that `record.key = key`
    - If the `index file` is indexing a non-repeatable key, returns one record at most.



- ```range_search(KeyType lower_bound, KeyType upper_bound)```
    - Searches all the records such that `lower_bound <= index(record) <= upper_bound`
    - It makes recursive calls, pruning the states that are not useful for the search.


- ```remove(KeyType key)```
    - Removes (logically) all the records such that `index(record) = key` by marking all the `record.removed` as `true` and removing such nodes of the `AVL`.

## Algorithms Performance (In function of disk accesses)

$n := Number \ of \ nodes \ in \ the \ AVL$

If the AVL is indexing a primary or secondary key:


|                       Member Function                        |       Performance       |                                                           Description                                                           |
|:------------------------------------------------------------:|:-----------------------:|:-------------------------------------------------------------------------------------------------------------------------------:|
|                  ```insert(KeyType key)```                   | $\mathcal{O}(\log_2 n)$ |                           Descends in the tree and inserts below a leaf node (often makes rotations).                           |
|                  ```search(KeyType key)```                   | $\mathcal{O}(\log_2 n)$ |                                          Descends in the tree until the key is found.                                           |
|                  ```remove(KeyType key)```                   | $\mathcal{O}(\log_2 n)$ |   Descends in the tree until the key is found; then, chooses the remove method given the node state (often makes rotations).    |
| ```range_search(KeyType lower_bound, KeyType upper_bound)``` |    $\mathcal{O}(n)$     | Descends the tree searching for the nodes that haves keys in the range and prunes the recursion to visit only the needed nodes. |

