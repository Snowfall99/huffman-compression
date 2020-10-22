#include "huffman.h"

void huffmanTree::create_huffman_tree() {
    if (heap.empty()) return;
    while (heap.size() > 1) {
        huffmanNode *node1 = heap.top();
        heap.pop();
        huffmanNode *node2 = heap.top();
        heap.pop();
        huffmanNode *node = new huffmanNode();
        node->freq = node1->freq + node2->freq;
        node1->parent = node;
        node2->parent = node;
        if (node1->freq > node2->freq) {
            node->left = node2;
            node->right = node1;
        } else {
            node->left = node1;
            node->right = node2;
        }
        heap.push(node);
    }
    root = heap.top();
    heap.pop();
}

void huffmanTree::dfs(huffmanNode *root, std::string code) {
    if (root == nullptr) return;
    if (root->info != nullptr) 
        root->info->word = code;
    if (root->left != nullptr) dfs(root->left, code + '0');
    if (root->right != nullptr) dfs(root->right, code + '1'); 
}

void huffmanTree::destruction(huffmanNode *&root) {
    if (root == nullptr) return;
    if (root->left != nullptr) destruction(root->left);
    if (root->right != nullptr) destruction(root->right);
    delete root;
}

void huffmanTree::insert(ByteInfo &n) {
    huffmanNode *node = new huffmanNode(&n);
    node->freq = n.prob;
    node->isLeaf = true;
    heap.push(node);
}

void huffmanTree::encode(std::vector<ByteInfo> &v) {
    for (auto &i: v) {
        insert(i);
    }

    create_huffman_tree();
    dfs(root, "");
}

void huffmanTree::clear() {
    while(!heap.empty()) heap.pop();
    destruction(root);
}

huffmanTree::~huffmanTree() {
    destruction(root);
}