#include <string>
#include "huffmanClass.h"
#include <time.h>
int main()
{
	clock_t start, end, start1, end1;
	cout << "��-------------Huffmanѹ������---------��" << endl << endl;
	string filePath;
	cout << "������������ļ���ַ" << endl << endl;
	getline(cin, filePath);
	//filePath = "E:\people\forCheng\C_code\huffmanEncode\data\Pic.bmp";
	Huffman huf(filePath);
	start = clock();
	huf.count();  //��ȡ�ַ�Ƶ������charCountFrequency������
	end = clock();
	cout << "����ȡ�ַ�Ƶ��ʹ��ʱ��Ϊ :   " << double((end - start) ) << " /ms" << endl << endl;
	huf.CreateHuffmanTree(huf.charCountFrequency);
	huf.GetHuffmanCode(huf.root, 0);
	end = clock();
	cout << "������ʹ��ʱ��Ϊ :   " << double((end - start)) << " /ms" << endl << endl;
	huf.WriteCode_(huf.HuffmanCodeTable);
	end = clock();
	cout << "��ѹ��ʹ��ʱ��Ϊ :   " << double((end - start)) << " /ms" << endl << endl;
	
	cout << "!--------------�������------------!" << endl << endl;
	start1 = clock();
	Huffman hufde(filePath);
	string extensionName = filePath.substr(filePath.rfind("."),4);//��ȡ��׺��
	hufde.Decode_(filePath + ".dada", filePath + extensionName);
	end1 = clock();
	cout << "����ʹ��ʱ��Ϊ :   " << double((end1 - start1)) << " /ms" << endl << endl;
	cout << "!--------------�������------------!" << endl << endl;
	getchar();
}