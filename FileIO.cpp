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
//���캯������������
FileIO::FileIO(QString FileName)
{
    this->fileName = FileName;

}

FileIO::FileIO()
{
    this->fileName = "";
}



//ѹ��ģ��
QMap<QChar, int> FileIO::countCharacters()
{
    QMap<QChar, int> characterCount;

    // ���ļ�
    QFile file(fileName);

    // ����ļ��Ƿ�ɹ���
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // ����һ�� QTextStream �������ڶ�ȡ�ļ�����
        QTextStream in(&file);

        // �����ļ��ı��뷽ʽΪUTF-8
        in.setCodec("UTF-8");

        // ���ַ���ȡ�ļ�����
        
        
            QString line = in.readAll();

            for (const QChar& character : line)
            {
           
                if (characterCount.contains(character))
                {
                    // �����ӳ����У����������
                    characterCount[character]++;
                }
                else
                {
                    // �������ӳ����У���ӵ�ӳ��������ü���Ϊ1
                    characterCount[character] = 1;
                }
            }
       

        // �ر��ļ�
        file.close();
    }
    else
    {
        qDebug() << "�޷����ļ���" << endl;
        file.close();
    }

    return characterCount;
}

QMap<QChar, QString> FileIO::getHuffmanCodeTable()
{
    QMap<QChar, QString> codeTable;

    HuffmanTree tree;
    // ������������
    HuffmanTree_node* root = tree.buildTree(countCharacters());

    // ���ɹ���������
    generateHuffmanCodes(root, "", codeTable);

    // �����ַ�������������ӳ���

    return codeTable;
}




bool FileIO::compress()
{
    /* �򿪴�ѹ���ļ� */
    QFile rawFile(fileName);
    if (!rawFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << u8"�޷����ļ���";
        return false;
    }

    /* ����ѹ������ļ� */
    QString compressedFileName = fileName.left(fileName.length()-4) + ".huff";
    QFile resultFile(compressedFileName);
    if (!resultFile.open(QIODevice::WriteOnly))
    {
        qDebug() << u8"�޷��򿪱�ѹ�����ļ�1 " << compressedFileName;
        rawFile.close(); // ʧ�ܣ��ر� rawfile
        return false;
    }

    /* ���ַ�����ʽд��Ƶ�ʱ��ѹ���ļ�ͷ�� */
    QMap<QChar, int> temp_characters = countCharacters();
    QDataStream resultDataStream(&resultFile);
    resultDataStream << temp_characters; // ��Ƶ�ʱ�д��������ļ�

    /* ������תΪ01�ַ��� */
    QMap<QChar, QString> temp_code = getHuffmanCodeTable(); // ��ñ����
    QString rawString = rawFile.readAll();                  // ��ȡ�ļ���������
    QString resultString = "";                              // ����01�ַ�����
    for (int i = 0; i < rawString.length(); i++)
    {
        resultString += temp_code.value(rawString[i]); // 01�ַ�����д��resultString
    }

    /* ����01�ַ�����׼��д�� */
    QBitArray compressedBits(resultString.size());
    for (int i = 0; i < resultString.size(); ++i) {
        compressedBits.setBit(i, resultString.at(i) == '1');
    }
    // ��λ����д������ļ�
    resultDataStream << compressedBits;

    resultFile.close();
    rawFile.close();
    qDebug() << u8"ѹ���ɹ��������ļ���";
    return true;
}

bool FileIO::decompression()
{
    /* �򿪽�ѹ�����ļ� */
    QFile rawFile(fileName);
    if (!rawFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "���ܴ��ļ���";
        return false;
    }

    /* ��ȡƵ�ʱ� */
    QMap<QChar, int> dataMap;
    QDataStream in(&rawFile);
    in >> dataMap; // �Ӷ������ļ��ж�ȡƵ�ʱ�

    /* ������� */
    QBitArray compressedBits;
    in >> compressedBits;
    // �� QBitArray ת��Ϊλ�ַ���
    QString binaryString;
    for (int i = 0; i < compressedBits.size(); ++i) {
        binaryString.append(compressedBits.testBit(i) ? '1' : '0');
    }

    /* �����������ݣ����н�ѹ�� */
    QString resultString = "";                // ����д���ļ����ַ���
    HuffmanTree tree;                         // ������������
    HuffmanTree_node* root = tree.buildTree(dataMap);
    resultString = decodeHuffmanString(binaryString, root); // ��������ַ���

    /* д����� */
    QString decompressedFileName; // ������ѹ������ļ���
    if (fileName.contains(".huff"))
    {
        decompressedFileName = fileName.left(fileName.length() - 5)+"_dehuff.txt";
    }
    else
    {
        return false;
    }
    QFile resultFile(decompressedFileName); // �򿪽�ѹ�����ļ�
    if (!resultFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "�޷��򿪱�ѹ�����ļ� " << decompressedFileName;

        return false;
    }
    QTextStream decompressedFileStream(&resultFile);
    decompressedFileStream.setCodec("UTF-8");
    decompressedFileStream << resultString; // д���ļ�

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
            // ��ǰ�ڵ���Ҷ�ӽڵ㣬��Ӷ�Ӧ���ַ��������ַ����������´Ӹ��ڵ㿪ʼ
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

    // �����Ҷ�ӽڵ㣬���ַ��Ͷ�Ӧ�ı������ӳ���
    if (!root->left && !root->right)
    {
        codeTable[root->data] = currentCode;
    }

    // �ݹ鴦����������������� '0'
    generateHuffmanCodes(root->left, currentCode + L'0', codeTable);
    // �ݹ鴦����������������� '1'
    generateHuffmanCodes(root->right, currentCode + L'1', codeTable);
}


