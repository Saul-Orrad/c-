#ifndef FILEIO_H
#define FILEIO_H

#include<iostream>
#include<string>
#include<map>
#include<fstream>
#include "HuffmanTree.h"
#include <QString>
#include<QMap>
class FileIO {
private:
	QString fileName;
	/*ѹ������*/
	QMap<QChar, int> countCharacters();//��ȡһ���ļ�������Ƶ�ʱ�
	QMap<QChar, QString> getHuffmanCodeTable();//�����������������ع�������
	/*��ѹ������*/


	QString decodeHuffmanString(const QString& huffmanString, HuffmanTree_node* root);


public:
	FileIO(QString FileName);
	FileIO();

	bool compress();//ѹ��
	bool decompression();//���ù���������ѹ��
	HuffmanTree_node* printTree();//ͼ�λ���ӡ��״
	QMap<QChar, QString>  printCodeTable();//��ӡ�����

	~FileIO();

};
void generateHuffmanCodes(HuffmanTree_node* root, const QString& currentCode, QMap<QChar, QString>& codeTable);//���ڴ�����������ĵݹ鸨������

#endif