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


// HuffmanTree_node ������
class HuffmanTree_node {
public:
    QChar data;
    int frequency;
    HuffmanTree_node* left, * right;
    HuffmanTree_node(QChar data, int frequency);
};


// HuffumanTree ������
class HuffmanTree {
private:
    HuffmanTree_node* root;
    // �������ȶ��еıȽϺ���
    struct Compare;
public:
    // ���캯��
    HuffmanTree();
    // �����������ͷ����ڵ��ڴ�
    ~HuffmanTree();
    // ��Ƶ�ʱ�����������
    HuffmanTree_node* buildTree(QMap<QChar, int> frequencyMap);



private:
    // �ݹ����ٹ��������ڵ�
    void destroyTree(HuffmanTree_node* node);


};

#endif
