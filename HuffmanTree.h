#pragma once
#ifndef HUFFUMANTREE_H
#define HUFFUMANTREE_H
#include<locale.h>
#include <map>
#include <queue>
#include <QtWidgets/QMainWindow>
#include "ui_EventHandler.h"
#include <QChar>
#include<QMap>


// HuffmanTree_node 类声明
class HuffmanTree_node {
public:
    QChar data;
    int frequency;
    HuffmanTree_node* left, * right;
    HuffmanTree_node(QChar data, int frequency);
};


// HuffumanTree 类声明
class HuffmanTree {
private:
    HuffmanTree_node* root;
    // 用于优先队列的比较函数
    struct Compare;
public:
    // 构造函数
    HuffmanTree();
    // 析构函数，释放树节点内存
    ~HuffmanTree();
    // 靠频率表创建哈夫曼树
    HuffmanTree_node* buildTree(QMap<QChar, int> frequencyMap);



private:
    // 递归销毁哈夫曼树节点
    void destroyTree(HuffmanTree_node* node);


};

#endif
