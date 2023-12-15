#include "FileIO.h"
#include "HuffmanTree.h"
#include<Windows.h>
#include<locale>
#include <iostream>
#include <fstream>
#include <map>
#include <codecvt>
#include <sstream>
#include <string>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QDataStream>
#include<QBitArray>
//构造函数和析构函数
FileIO::FileIO(QString FileName)
{
    this->fileName = FileName;

}

FileIO::FileIO()
{
    this->fileName = "";
}



//压缩模块
QMap<QChar, int> FileIO::countCharacters()
{
    QMap<QChar, int> characterCount;

    // 打开文件
    QFile file(fileName);

    // 检查文件是否成功打开
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // 创建一个 QTextStream 对象，用于读取文件内容
        QTextStream in(&file);

        // 设置文件的编码方式为UTF-8
        in.setCodec("UTF-8");

        // 逐字符读取文件内容
        
        
            QString line = in.readAll();

            for (const QChar& character : line)
            {
           
                if (characterCount.contains(character))
                {
                    // 如果在映射表中，增加其计数
                    characterCount[character]++;
                }
                else
                {
                    // 如果不在映射表中，添加到映射表，并设置计数为1
                    characterCount[character] = 1;
                }
            }
       

        // 关闭文件
        file.close();
    }
    else
    {
        qDebug() << "无法打开文件！" << endl;
        file.close();
    }

    return characterCount;
}

QMap<QChar, QString> FileIO::getHuffmanCodeTable()
{
    QMap<QChar, QString> codeTable;

    HuffmanTree tree;
    // 构建哈夫曼树
    HuffmanTree_node* root = tree.buildTree(countCharacters());

    // 生成哈夫曼编码
    generateHuffmanCodes(root, "", codeTable);

    // 返回字符与哈夫曼编码的映射表

    return codeTable;
}




bool FileIO::compress()
{
    /* 打开待压缩文件 */
    QFile rawFile(fileName);
    if (!rawFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << u8"无法打开文件：";
        return false;
    }

    /* 构建压缩后的文件 */
    QString compressedFileName = fileName.left(fileName.length()-4) + ".huff";
    QFile resultFile(compressedFileName);
    if (!resultFile.open(QIODevice::WriteOnly))
    {
        qDebug() << u8"无法打开被压缩的文件1 " << compressedFileName;
        rawFile.close(); // 失败，关闭 rawfile
        return false;
    }

    /* 以字符流方式写入频率表表到压缩文件头部 */
    QMap<QChar, int> temp_characters = countCharacters();
    QDataStream resultDataStream(&resultFile);
    resultDataStream << temp_characters; // 将频率表写入二进制文件

    /* 将内容转为01字符串 */
    QMap<QChar, QString> temp_code = getHuffmanCodeTable(); // 获得编码表
    QString rawString = rawFile.readAll();                  // 读取文件所有内容
    QString resultString = "";                              // 构造01字符串，
    for (int i = 0; i < rawString.length(); i++)
    {
        resultString += temp_code.value(rawString[i]); // 01字符串被写进resultString
    }

    /* 处理01字符串，准备写入 */
    QBitArray compressedBits(resultString.size());
    for (int i = 0; i < resultString.size(); ++i) {
        compressedBits.setBit(i, resultString.at(i) == '1');
    }
    // 将位数据写入输出文件
    resultDataStream << compressedBits;

    resultFile.close();
    rawFile.close();
    qDebug() << u8"压缩成功，生成文件：";
    return true;
}

bool FileIO::decompression()
{
    /* 打开解压缩的文件 */
    QFile rawFile(fileName);
    if (!rawFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "不能打开文件！";
        return false;
    }

    /* 读取频率表 */
    QMap<QChar, int> dataMap;
    QDataStream in(&rawFile);
    in >> dataMap; // 从二进制文件中读取频率表

    /* 读入编码 */
    QBitArray compressedBits;
    in >> compressedBits;
    // 将 QBitArray 转换为位字符串
    QString binaryString;
    for (int i = 0; i < compressedBits.size(); ++i) {
        binaryString.append(compressedBits.testBit(i) ? '1' : '0');
    }

    /* 处理读入的数据，进行解压缩 */
    QString resultString = "";                // 最终写进文件的字符串
    HuffmanTree tree;                         // 构建哈夫曼树
    HuffmanTree_node* root = tree.buildTree(dataMap);
    resultString = decodeHuffmanString(binaryString, root); // 获得最终字符串

    /* 写入操作 */
    QString decompressedFileName; // 构建解压缩后的文件名
    if (fileName.contains(".huff"))
    {
        decompressedFileName = fileName.left(fileName.length() - 5)+"_dehuff.txt";
    }
    else
    {
        return false;
    }
    QFile resultFile(decompressedFileName); // 打开解压缩的文件
    if (!resultFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "无法打开被压缩的文件 " << decompressedFileName;

        return false;
    }
    QTextStream decompressedFileStream(&resultFile);
    decompressedFileStream.setCodec("UTF-8");
    decompressedFileStream << resultString; // 写入文件

    resultFile.close();
    return true;
}

QString FileIO::decodeHuffmanString(const QString& huffmanString, HuffmanTree_node* root) {
    QString decodedString;
    HuffmanTree_node* current = root;

    for (int i = 0; i < huffmanString.length(); ++i) {
        QChar bit = huffmanString.at(i);
        if (bit == '0' && current->left) {
            current = current->left;
        }
        else if (bit == '1' && current->right) {
            current = current->right;
        }

        if (current->left == nullptr && current->right == nullptr) {
            // 当前节点是叶子节点，添加对应的字符到解码字符串，并重新从根节点开始
            decodedString.append(current->data);
            current = root;
        }
    }

    return decodedString;
}


 QMap<QChar,QString> FileIO::printCodeTable()
{
     return getHuffmanCodeTable();
 

}

FileIO::~FileIO()
{

}


void generateHuffmanCodes(HuffmanTree_node* root, const QString& currentCode, QMap<QChar, QString>& codeTable) {
    if (root == nullptr) 
    {
        return;
    }

    // 如果是叶子节点，则将字符和对应的编码加入映射表
    if (!root->left && !root->right)
    {
        codeTable[root->data] = currentCode;
    }

    // 递归处理左子树，编码添加 '0'
    generateHuffmanCodes(root->left, currentCode + L'0', codeTable);
    // 递归处理右子树，编码添加 '1'
    generateHuffmanCodes(root->right, currentCode + L'1', codeTable);
}


