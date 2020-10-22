#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "dataType.h"
#include <vector>
#include <queue>
#include <string>

class huffmanTree {
private:
    struct huffmanNode {
        ByteInfo *info;
        int freq;
        huffmanNode *right, *left, *parent;
        bool isLeaf;

        huffmanNode (ByteInfo *p = nullptr) {
            info = p;
            freq = 0;
            isLeaf = false;
            right = NULL; left = NULL; parent = NULL;
        }
    };
    struct compare {
        bool operator() (huffmanNode* &node1, huffmanNode* &node2) const {
            return (node1->freq) > (node2->freq);
        }
    };

    huffmanNode *root;
    std::priority_queue<huffmanNode*, std::vector<huffmanNode*>, compare> heap;
    
    void create_huffman_tree();
    void dfs(huffmanNode *root, std::string code);
    void destruction(huffmanNode *&root);
    void insert(ByteInfo &n);
public:
    huffmanTree() : root(nullptr) {}
    void encode(std::vector<ByteInfo> &v);
    void clear();
    ~huffmanTree();
};

#endif