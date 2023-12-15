#include "HuffmanTree.h"
#include <iostream>

// HuffmanTree_node 类实现
HuffmanTree_node::HuffmanTree_node(QChar data, int frequency) {
    this->data = data;
    this->frequency = frequency;
    left = right = nullptr;
}

// HuffmanTree 类实现

// 用于优先队列的比较函数
struct HuffmanTree::Compare {
    bool operator()(HuffmanTree_node* a, HuffmanTree_node* b) {
        return a->frequency > b->frequency;
    }
};

// 构造函数
HuffmanTree::HuffmanTree() : root(nullptr) {}

// 析构函数，释放树节点内存
HuffmanTree::~HuffmanTree() {
    if (root != nullptr) {
        destroyTree(root);
    }
}

// 创建哈夫曼树
HuffmanTree_node* HuffmanTree::buildTree(QMap<QChar, int> frequencyMap) {
    std::priority_queue<HuffmanTree_node*, std::vector<HuffmanTree_node*>, Compare> minHeap;

    // 将频率映射表中的字符和频率转化为节点并加入优先队列
    for (auto it = frequencyMap.begin(); it != frequencyMap.end(); ++it) {
        HuffmanTree_node* node = new HuffmanTree_node(it.key(), it.value());
        minHeap.push(node);
    }

    // 构建哈夫曼树
    while (minHeap.size() > 1) {
        HuffmanTree_node* left = minHeap.top();
        minHeap.pop();
        HuffmanTree_node* right = minHeap.top();
        minHeap.pop();

        HuffmanTree_node* parent = new HuffmanTree_node('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        minHeap.push(parent);
    }

    // 根据优先队列的最后一个节点作为根节点
    root = minHeap.top();
    return root;
}



// 递归销毁哈夫曼树节点
void HuffmanTree::destroyTree(HuffmanTree_node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}
