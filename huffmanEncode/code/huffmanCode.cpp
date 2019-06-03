#include <string>
#include "huffmanClass.h"
#include <time.h>
int main()
{
	clock_t start, end, start1, end1;
	cout << "！-------------Huffman压缩编码---------！" << endl << endl;
	string filePath;
	cout << "请输入待编码文件地址" << endl << endl;
	getline(cin, filePath);
	//filePath = "E:\people\forCheng\C_code\huffmanEncode\data\Pic.bmp";
	Huffman huf(filePath);
	start = clock();
	huf.count();  //获取字符频数存在charCountFrequency数组中
	end = clock();
	cout << "到获取字符频数使用时间为 :   " << double((end - start) ) << " /ms" << endl << endl;
	huf.CreateHuffmanTree(huf.charCountFrequency);
	huf.GetHuffmanCode(huf.root, 0);
	end = clock();
	cout << "到编码使用时间为 :   " << double((end - start)) << " /ms" << endl << endl;
	huf.WriteCode_(huf.HuffmanCodeTable);
	end = clock();
	cout << "到压缩使用时间为 :   " << double((end - start)) << " /ms" << endl << endl;
	
	cout << "!--------------解码程序------------!" << endl << endl;
	start1 = clock();
	Huffman hufde(filePath);
	string extensionName = filePath.substr(filePath.rfind("."),4);//获取后缀名
	hufde.Decode_(filePath + ".dada", filePath + extensionName);
	end1 = clock();
	cout << "解码使用时间为 :   " << double((end1 - start1)) << " /ms" << endl << endl;
	cout << "!--------------程序结束------------!" << endl << endl;
	getchar();
}