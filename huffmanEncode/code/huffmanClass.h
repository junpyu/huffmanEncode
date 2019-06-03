#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;
class Huffman
{
public:
	struct HuffmanNode
	{
		unsigned char value=0; //�ڵ�ֵ
		int frequency = 0; //�ڵ�Ƶ��
		struct HuffmanNode *Lchild = NULL;
		struct HuffmanNode *Rchild = NULL;

	};
private:
	struct CountVector
	{
		unsigned char value=0; //�ַ�
		int frequency = 0;  //�ַ�Ƶ��
		struct HuffmanNode *nodeAddress = NULL;
	};

	struct HuffmanCodeElement
	{
		string code;
		int codelen=0;
	};


	static bool mysortfunction(CountVector A, CountVector B)
	{  //����sort�����㷨
		return A.frequency < B.frequency;
	}
	static bool mysortfunction2(CountVector A, CountVector B)
	{  //����sort�����㷨
		return A.frequency > B.frequency;
	}
public:
	//���ڵ�
	HuffmanNode *root;
	string fileAddress;
	long int NumOfChar = 0;
	vector<CountVector> charCountFrequency;  //���ڴ洢�ַ�Ƶ��
	vector<HuffmanCodeElement> HuffmanCodeTable;
	Huffman(string sourcefile);  //���캯��
	void count();   //ͳ�Ƹ����ַ���Ƶ������
	void CreateHuffmanTree(vector<CountVector> charFrequency);  //����huffman��
	void GetHuffmanCode(HuffmanNode *root, int len);
	void WriteCode_(vector<HuffmanCodeElement> HFCode);
	void Decode_(string sourcefile, string dstfile);
	void charCountFrequencyOutZero();
};

Huffman::Huffman(string sourcefile)
{
	fileAddress = sourcefile;  //��ʼ���ļ������ַ
	HuffmanCodeElement* temp = new HuffmanCodeElement;
	for (int i = 0; i < 256; i++) {
		temp->codelen= 0;
		HuffmanCodeTable.push_back(*temp);
	}
}

void Huffman::count()
{
	ifstream readfile;
	readfile.open(fileAddress, ios::in | ios::binary);
	unsigned char *now = new unsigned char;  //�洢��ǰ��ȡ�����ַ�
	*now = 0;
	CountVector* temp = new CountVector;  
	for (int i = 0; i < 256; i++) {
		temp->value = i;
		temp->frequency=0;
		charCountFrequency.push_back(*temp);
	}
	while (!readfile.eof())
	{
		readfile.read((char*)now, sizeof(unsigned char));
		charCountFrequency[*now].frequency++;//ֻ��Ҫ�ڶ�Ӧ��λ���Ͻ���Ƶ����
		NumOfChar++;
	}
	charCountFrequency[*now].frequency--;
	readfile.close();

}
void Huffman::CreateHuffmanTree(vector<CountVector> charFrequency)
{
	vector<CountVector>  buildtree;
	//HuffmanNode newNode;
	HuffmanNode* rootnode = new HuffmanNode;
	buildtree = charFrequency;
	sort(buildtree.begin(), buildtree.end(), mysortfunction);
	vector<CountVector>::iterator last = buildtree.begin();
	for (int i = 0; i < buildtree.size(); i++) {
		if (buildtree[i].frequency != 0) {
			if (last != buildtree.begin()) {
				buildtree.erase(buildtree.begin(), last);
			}
			break;
		}
		last++;
	}
	int treedepth = 0;
	while (buildtree.size() > 1)
	{
		HuffmanNode* nodeLeft = new HuffmanNode,
			* nodeRight = new HuffmanNode,
			* newNode = new HuffmanNode;
		CountVector insertnew;
		if (buildtree[0].nodeAddress != NULL)
		{  //�����Ҷ�ӽڵ�Ļ�  ���������ĵ�ַ��ΪNULL
			nodeLeft->Lchild = buildtree[0].nodeAddress->Lchild;
			nodeLeft->Rchild = buildtree[0].nodeAddress->Rchild;
		}
		else
		{
			nodeLeft->Lchild = NULL;
			nodeLeft->Rchild = NULL;
		}
		if (buildtree[1].nodeAddress != NULL)
		{
			nodeRight->Lchild = buildtree[1].nodeAddress->Lchild;
			nodeRight->Rchild = buildtree[1].nodeAddress->Rchild;
		}
		else
		{
			nodeRight->Lchild = NULL;
			nodeRight->Rchild = NULL;
		}
		nodeLeft->frequency = buildtree[0].frequency;
		nodeLeft->value = buildtree[0].value;
		nodeRight->frequency = buildtree[1].frequency;
		nodeRight->value = buildtree[1].value;
		newNode->frequency = nodeRight->frequency + nodeLeft->frequency;
		newNode->Lchild = nodeLeft;
		newNode->Rchild = nodeRight;
		insertnew.frequency = newNode->frequency;
		insertnew.value = 0;
		insertnew.nodeAddress = newNode;
		//vector<CountVector>::iterator it = buildtree.begin();
		buildtree.erase(buildtree.begin());
		//vector<CountVector>::iterator it = buildtree.begin();
		buildtree.erase(buildtree.begin());
		//vector<CountVector>::iterator it = buildtree.begin();
		buildtree.insert(buildtree.begin(), insertnew);
		sort(buildtree.begin(), buildtree.end(), mysortfunction);   //ÿ�θ�����Ҫ����
		rootnode = newNode;
		treedepth++;

	}
	root = rootnode;
}


void  Huffman::GetHuffmanCode(HuffmanNode* root, int depth)
{
	static char code[512];
	//�ж������
	if (root->Lchild != NULL)
	{
		code[depth] = '0';
		code[depth + 1] = '\0';
		GetHuffmanCode(root->Lchild, depth + 1);
	}
	if (root->Rchild != NULL)
	{
		code[depth] = '1';
		code[depth + 1] = '\0';
		GetHuffmanCode(root->Rchild, depth + 1);
	}
	else
	{
		int codelength = 0;
		for (int j = 0; code[j] != '\0'; j++)
		{
			codelength++;
		}
		HuffmanCodeTable[root->value].codelen = codelength;
		HuffmanCodeTable[root->value].code = (string)code;
	}

}
void Huffman::WriteCode_(vector<HuffmanCodeElement> HFCode)
{
	//���ļ��ܶ�ȡ�ַ������б���
	charCountFrequencyOutZero();
	int codeNum = charCountFrequency.size();
	string address = fileAddress;
	ofstream writecode;
	ifstream read;
	read.open(address, ios::in | ios::binary);   //�����ļ�
	writecode.open(address + ".dada", ios::out | ios::binary);   //��*.dada����
	unsigned char *now = new unsigned char; //��ȡ�� ��ǰ�ַ�
	unsigned char save = 0;  //ÿһ�α���һ���ֽڵĳ���
	int len = 0;

	// ��Huffman����д��ͷ��������ͷ�ļ��������������
	char head = '>';
	writecode.write((char*)&head, sizeof(char));
	writecode.write((char *)&codeNum, sizeof(int));
	writecode.write((char *)& len, sizeof(int));  //д�����һ��д���λ��
	for (int i = 0; i < codeNum; i++)
	{    //д���ַ�ֵ��Ƶ��
		writecode.write((char*)&charCountFrequency[i].value, sizeof(unsigned char));
		writecode.write((char*)&charCountFrequency[i].frequency, sizeof(int));
	}
	//read.read((char*)now, 1);
	read.read((char*)now, sizeof(unsigned char));
	while (!read.eof())
	{
		for (int j = 0; j < HFCode[*now].codelen; j++)
		{
			if (len != 0)
				save = save << 1;
			save = save | (HFCode[*now].code[j] - '0');
			len++;
			if (len == 8)
			{
				writecode.write((char *)&save, sizeof(unsigned char));
				save = 0;
				len = 0;
			}
		}
		read.read((char*)now, sizeof(unsigned char));
		//*now = read.get();
	}
	if (len != 0)
	{
		writecode.write((char*)&save, sizeof(unsigned char));
	}
	writecode.seekp((sizeof(char)+ sizeof(int)), ios::beg);
	writecode.write((char*)& len, sizeof(int));  //д�����һ��д���λ��
	read.close();
	writecode.close();

}
void Huffman::Decode_(string sourcefile, string dstfile)
{
	ifstream read;
	ofstream write;
	vector<CountVector> arr;
	unsigned char now;  //��ȡ�ĵ�ǰ�ַ�
	int last = 0;   //���һ�ζ�ȡ��λ��
	int n; //�ַ�����
	read.open(sourcefile, ios::in | ios::binary);  //��ȡ�����ļ�
	write.open(dstfile, ios::out | ios::binary); //�򿪽������ļ�
	read.read((char*)& now, sizeof(now));
	if (!(now == '>'))
	{
		cout << "���ļ���Huffman�����ʽ����ȷ" << endl << endl;
		read.close();
		return;
	}
	read.read((char*)& n, sizeof(int));
	read.read((char*)& last, sizeof(last));
	for (int i = 0; i < n; i++)
	{
		CountVector* insert = new CountVector;

		read.read((char*) & (insert->value), sizeof(unsigned char));
		read.read((char*) & (insert->frequency), sizeof(int));
		arr.push_back(*insert);
	}
	this->root = new HuffmanNode;
	CreateHuffmanTree(arr);
	HuffmanNode* pNow = root;

	unsigned char* temp = new unsigned char; //ÿ�ζ�һ���ֽ�
	read.read((char*)temp, sizeof(unsigned char));
	while (!read.eof())
	{
		unsigned char* ifLast = new unsigned char; //�����ж��Ƿ�����ļ�ĩβ
		read.read((char*)ifLast, sizeof(unsigned char));
		int i;
		if (read.eof())
		{
			i = last - 1;
		}
		else
		{
			i = 7;
		}
		for (; i >= 0; i--)
		{
			if ((*temp >> i & 1) == 0)   //�����ƶ�7λ�ж϶�������0 ����1 
				pNow = pNow->Lchild;
			else
				pNow = pNow->Rchild;
			if (pNow->Lchild == NULL && pNow->Rchild == NULL)
			{
				write.write((char*) & (pNow->value), sizeof(unsigned char));
				pNow = root;
			}
		}
		temp = ifLast;
	}

	read.close();
	write.close();
}
void Huffman::charCountFrequencyOutZero()
{
	sort(charCountFrequency.begin(), charCountFrequency.end(), mysortfunction);
	vector<CountVector>::iterator last = charCountFrequency.begin();
	for (int i = 0; i < charCountFrequency.size(); i++) {
		if (charCountFrequency[i].frequency != 0) {
			if (last != charCountFrequency.begin()) {
				charCountFrequency.erase(charCountFrequency.begin(), last);
			}
			break;
		}
		last++;
	}
}


