#include "HuffmanTree.h"
#include <iostream>

// HuffmanTree_node ��ʵ��
HuffmanTree_node::HuffmanTree_node(QChar data, int frequency) {
    this->data = data;
    this->frequency = frequency;
    left = right = nullptr;
}

// HuffmanTree ��ʵ��

// �������ȶ��еıȽϺ���
struct HuffmanTree::Compare {
    bool operator()(HuffmanTree_node* a, HuffmanTree_node* b) {
        return a->frequency > b->frequency;
    }
};

// ���캯��
HuffmanTree::HuffmanTree() : root(nullptr) {}

// �����������ͷ����ڵ��ڴ�
HuffmanTree::~HuffmanTree() {
    if (root != nullptr) {
        destroyTree(root);
    }
}

// ������������
HuffmanTree_node* HuffmanTree::buildTree(QMap<QChar, int> frequencyMap) {
    std::priority_queue<HuffmanTree_node*, std::vector<HuffmanTree_node*>, Compare> minHeap;

    // ��Ƶ��ӳ����е��ַ���Ƶ��ת��Ϊ�ڵ㲢�������ȶ���
    for (auto it = frequencyMap.begin(); it != frequencyMap.end(); ++it) {
        HuffmanTree_node* node = new HuffmanTree_node(it.key(), it.value());
        minHeap.push(node);
    }

    // ������������
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

    // �������ȶ��е����һ���ڵ���Ϊ���ڵ�
    root = minHeap.top();
    return root;
}



// �ݹ����ٹ��������ڵ�
void HuffmanTree::destroyTree(HuffmanTree_node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}
