#include<iostream>
using namespace std;
constexpr auto WEIGHTMAXSIZE = 10000;
constexpr auto MAXSIZE = 95;
constexpr auto START = 32;

struct huffman {
	int weight;
	char value;
	int ln;//the left node
	int rn;//the right node
	int pa;//the parent node
	char a;//the key of the values

};

int writefile(char* FILENAME, char* a, int length, int close) {
	FILE* fp;
	fopen_s(&fp, FILENAME, "wb+");
	if (fp == NULL) {
		printf("erro");
		return 0;
	}
	else {
		fwrite(a, sizeof(char), length, fp);
		fclose(fp);
		return 1;
	}

}

void cal_key(string& s) {

	int length = (int)s.length() / 8;
	int final_length = (int)s.length() / 8 + (s.length() % 8 != 0);

	string middle;
	char* cal_KEY = new char[final_length]();
	char filename[6] = { "2.txt" };

	while (final_length >= length)
	{
		for (int i = 0; i < length; i++)
		{
			middle = s.substr(static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(i) * 8, 8);
			for (int j = 0; j < 8; j++)
			{
				if (middle[j] == '1') cal_KEY[i] |= (1 << (7 - j));
				else if (middle[j] == '0')cal_KEY[i] &= ~(1 << (7 - j));
			}
		}
		//处理结尾字符串,不足则补0
		if (final_length > length)
		{
			//继承末尾子串
			string end = s.substr(static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(length) * 8, 8);
			int j = 0;
			while (end[j] != '\0' && j < 8)
			{
				if (end[j] == '1') cal_KEY[length] |= (1 << (7 - j));
				else if (end[j] == '0')cal_KEY[length] &= ~(1 << (7 - j));
				j++;
			}
			for (j; j < 8; j++)cal_KEY[length] &= ~(1 << (7 - j));
		}
		break;
	}
	writefile(filename, cal_KEY, final_length, 0);
	delete[] cal_KEY;
}

void HuffmanCreat(huffman huf[MAXSIZE * 2 - 1]) {
	//huffman huftree[MAXSIZE * 2 - 1] = { 0 };//According to the nature of a completely binary tree,the innode is one less than the leaf node
	for (int i = 0; i < MAXSIZE - 1; i++)
	{
		/*
		遍历MAXSIZE遍，每次挑选权重最小的两个
		用x1和x2保存这两个元素在数组中的位置，并用变量weight1和weight2进行保存他们的权重
		*/
		int weight_min1, weight_min2;
		int x1, x2;
		weight_min1 = weight_min2 = WEIGHTMAXSIZE;//由于需要逐次比较后取小值，所以存放量的初值必须足够的大
		x1 = x2 = 0;
		for (int j = 0; j < MAXSIZE + i; j++)
		{
			if (huf[j].weight < weight_min1 && huf[j].pa == -1)//每一次遍历取最小两值合并树生成新结点，并赋值父节点位置，因此只有没有父节点的结点（即pa变量为初值-1）才有遍历的必要
			{
				weight_min2 = weight_min1;
				weight_min1 = huf[j].weight;
				x2 = x1;
				x1 = j;
			}
			else if (huf[j].weight < weight_min2 && huf[j].pa == -1)
			{
				weight_min2 = huf[j].weight;
				x2 = j;
			}
		}
		/*
		在第n+i的位置上赋值权重，设置左右子节点位置。
		并设置两个最小权重结点的父节点。
		*/
		huf[x1].pa = MAXSIZE + i;
		huf[x2].pa = MAXSIZE + i;
		huf[i + MAXSIZE].weight = weight_min1 + weight_min2;
		huf[i + MAXSIZE].ln = x1;
		huf[i + MAXSIZE].rn = x2;
	}
}

void code(char* values, int length, huffman huf[MAXSIZE * 2 - 1]) {
	int signal_parent = 0, signal_current = 0, j = 0;
	char code[10] = { '\0' };//暂时存放编码后的Huffman编码
	string coderesult;

	for (int i = 0; i < length; i++)
	{
		signal_current = (int)values[i] - START;//获得当前字符在哈夫曼树中的起始位置。
		signal_parent = huf[signal_current].pa;//获得父结点的位置
		while (signal_parent != -1) {//不断向父节点遍历，并且验证是左节点还是右节点，存入暂存数组
			if (huf[signal_parent].ln == signal_current)
				code[j] = '0';
			else if (huf[signal_parent].rn == signal_current)
				code[j] = '1';
			signal_current = signal_parent;
			signal_parent = huf[signal_current].pa;
			j++;
		}
		/*将code逆序处理放入缓存string流*/
		for (int k = 9; k >= 0; k--)
		{
			if (code[k] != '\0')
			{
				coderesult = coderesult + code[k];
			}
		}
		/*并且将缓存数组code[]重新置为空*/
		for (int k = 0; k < 10; k++)
			code[k] = '\0';
		j = 0;
	}
	cal_key(coderesult);


}

void decode_key(char* code, int length, string& decoding)
{
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < 8; j++) {
			int num = code[i];
			//0x80 :1000 0000
			if ((num & 0x80) == 0x80) {
				decoding += "1";
			}
			else {
				decoding += "0";
			}
			code[i] <<= 1;
		}
	}
}

void decode(char* values, int length, huffman huf[MAXSIZE * 2 - 1]) {
	int m = 2 * MAXSIZE - 2;
	string num = { '\0' };
	decode_key(values, length, num);

	for (int i = 0; i < length * 8; i++)
	{
		if (num[i] == '0')
		{
			//cout << "0";
			m = huf[m].ln;
		}
		else if (num[i] == '1')
		{
			//cout << "1";
			m = huf[m].rn;
		}
		if (huf[m].ln == -1 && huf[m].rn == -1)
		{
			//cout << " ";
			cout << huf[m].value;
			//cout << endl;
			m = 2 * MAXSIZE - 2;//将m置为堆顶位置。
		}
	}

}

int openfile(char* tmp, int file_size, char* Filename)
{
	FILE* fp;
	errno_t err = fopen_s(&fp, Filename, "rb");   //打开文件
	if (fp == NULL)
	{
		cout << "faile to open" << endl;
		return 0;
	}
	else
	{
		memset(tmp, '\0', file_size * sizeof(char));  //初始化此控件内容，否则可能会有乱码
		fseek(fp, 0, SEEK_SET);                 //重新将指针指向文件首部
		fread(tmp, sizeof(char), file_size, fp); //开始读取整个文件
		cout << "文件内容为: " << tmp << endl;
		fclose(fp);
		return file_size;
	}

}

int File_size(char* FILENAME) {
	FILE* fp;
	errno_t err = fopen_s(&fp, FILENAME, "r");   //打开文件
	int file_size;   //保存文件字符数
	if (fp == NULL)
	{
		cout << "faile to open" << endl;
		return 0;
	}
	else
	{
		if (fp == NULL)
			return 0;
		fseek(fp, 0, SEEK_END);      //将文件指针指向该文件的最后
		file_size = ftell(fp);       //根据指针位置，此时可以算出文件的字符数
		fclose(fp);
		return file_size;
	}
}

void use() {

	huffman Huf[MAXSIZE * 2 - 1] = { 0 };//设置Huffman树的存储数组
	for (int i = 0; i < MAXSIZE; i++)//初始化已知的maxsize个叶结点。
	{
		Huf[i] = { i + START, (char)(i + START), -1, -1, -1 };
	}
	for (int i = MAXSIZE; i < MAXSIZE * 2 - 1; i++)//初始化以后要存储内结点的maxsize-1个结点
	{
		Huf[i] = { 0, 0, -1, -1, -1 };
	}
	HuffmanCreat(Huf);//创建Huffman树
	/*for (int i = 0; i < MAXSIZE * 2 - 1; i++)
	{
		cout.width(4);
		cout << i;
		cout.width(6);
		cout << Huf[i].weight;
		cout.width(6);
		cout << Huf[i].value;
		cout.width(4);
		cout << Huf[i].ln;
		cout.width(4);
		cout << Huf[i].rn;
		cout.width(4);
		cout << Huf[i].pa;
		cout << endl;
	}*/


	char filename[] = { "1.txt" };
	int length = File_size(filename);
	char* a = new char[length];
	openfile(a, length, filename);
	code(a, length, Huf);
	cout << endl;

	char filename1[] = { "2.txt" };
	int lengthdecode = File_size(filename1);
	char* b = new char[lengthdecode];
	openfile(b, lengthdecode, filename1);
	decode(b, lengthdecode, Huf);
	cout << endl;

}

int main() {
	use();
}





