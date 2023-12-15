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
	/*压缩所用*/
	QMap<QChar, int> countCharacters();//读取一个文件，返回频率表
	QMap<QChar, QString> getHuffmanCodeTable();//构建哈夫曼树，返回哈夫曼表
	/*解压缩所用*/


	QString decodeHuffmanString(const QString& huffmanString, HuffmanTree_node* root);


public:
	FileIO(QString FileName);
	FileIO();

	bool compress();//压缩
	bool decompression();//利用哈夫曼树解压缩
	HuffmanTree_node* printTree();//图形化打印树状
	QMap<QChar, QString>  printCodeTable();//打印编码表

	~FileIO();

};
void generateHuffmanCodes(HuffmanTree_node* root, const QString& currentCode, QMap<QChar, QString>& codeTable);//用于创建哈弗曼表的递归辅助函数

#endif