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
		//�����β�ַ���,������0
		if (final_length > length)
		{
			//�̳�ĩβ�Ӵ�
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
		����MAXSIZE�飬ÿ����ѡȨ����С������
		��x1��x2����������Ԫ���������е�λ�ã����ñ���weight1��weight2���б������ǵ�Ȩ��
		*/
		int weight_min1, weight_min2;
		int x1, x2;
		weight_min1 = weight_min2 = WEIGHTMAXSIZE;//������Ҫ��αȽϺ�ȡСֵ�����Դ�����ĳ�ֵ�����㹻�Ĵ�
		x1 = x2 = 0;
		for (int j = 0; j < MAXSIZE + i; j++)
		{
			if (huf[j].weight < weight_min1 && huf[j].pa == -1)//ÿһ�α���ȡ��С��ֵ�ϲ��������½�㣬����ֵ���ڵ�λ�ã����ֻ��û�и��ڵ�Ľ�㣨��pa����Ϊ��ֵ-1�����б����ı�Ҫ
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
		�ڵ�n+i��λ���ϸ�ֵȨ�أ����������ӽڵ�λ�á�
		������������СȨ�ؽ��ĸ��ڵ㡣
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
	char code[10] = { '\0' };//��ʱ��ű�����Huffman����
	string coderesult;

	for (int i = 0; i < length; i++)
	{
		signal_current = (int)values[i] - START;//��õ�ǰ�ַ��ڹ��������е���ʼλ�á�
		signal_parent = huf[signal_current].pa;//��ø�����λ��
		while (signal_parent != -1) {//�����򸸽ڵ������������֤����ڵ㻹���ҽڵ㣬�����ݴ�����
			if (huf[signal_parent].ln == signal_current)
				code[j] = '0';
			else if (huf[signal_parent].rn == signal_current)
				code[j] = '1';
			signal_current = signal_parent;
			signal_parent = huf[signal_current].pa;
			j++;
		}
		/*��code��������뻺��string��*/
		for (int k = 9; k >= 0; k--)
		{
			if (code[k] != '\0')
			{
				coderesult = coderesult + code[k];
			}
		}
		/*���ҽ���������code[]������Ϊ��*/
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
			m = 2 * MAXSIZE - 2;//��m��Ϊ�Ѷ�λ�á�
		}
	}

}

int openfile(char* tmp, int file_size, char* Filename)
{
	FILE* fp;
	errno_t err = fopen_s(&fp, Filename, "rb");   //���ļ�
	if (fp == NULL)
	{
		cout << "faile to open" << endl;
		return 0;
	}
	else
	{
		memset(tmp, '\0', file_size * sizeof(char));  //��ʼ���˿ؼ����ݣ�������ܻ�������
		fseek(fp, 0, SEEK_SET);                 //���½�ָ��ָ���ļ��ײ�
		fread(tmp, sizeof(char), file_size, fp); //��ʼ��ȡ�����ļ�
		cout << "�ļ�����Ϊ: " << tmp << endl;
		fclose(fp);
		return file_size;
	}

}

int File_size(char* FILENAME) {
	FILE* fp;
	errno_t err = fopen_s(&fp, FILENAME, "r");   //���ļ�
	int file_size;   //�����ļ��ַ���
	if (fp == NULL)
	{
		cout << "faile to open" << endl;
		return 0;
	}
	else
	{
		if (fp == NULL)
			return 0;
		fseek(fp, 0, SEEK_END);      //���ļ�ָ��ָ����ļ������
		file_size = ftell(fp);       //����ָ��λ�ã���ʱ��������ļ����ַ���
		fclose(fp);
		return file_size;
	}
}

void use() {

	huffman Huf[MAXSIZE * 2 - 1] = { 0 };//����Huffman���Ĵ洢����
	for (int i = 0; i < MAXSIZE; i++)//��ʼ����֪��maxsize��Ҷ��㡣
	{
		Huf[i] = { i + START, (char)(i + START), -1, -1, -1 };
	}
	for (int i = MAXSIZE; i < MAXSIZE * 2 - 1; i++)//��ʼ���Ժ�Ҫ�洢�ڽ���maxsize-1�����
	{
		Huf[i] = { 0, 0, -1, -1, -1 };
	}
	HuffmanCreat(Huf);//����Huffman��
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





