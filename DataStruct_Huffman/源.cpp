#include<iostream>
using namespace std;
constexpr auto ARRAYSIZE = 1024*1024;
constexpr auto WEIGHTMAXSIZE = 10000;
constexpr auto MAXSIZE = 128;

struct huffman {
	int weight;
	char value;
	int ln;//the left node
	int rn;//the right node
	int pa;//the parent node
	char key[10];//the key of the values
};

/*
huffman�ṹ���ʼ��
ʵ�ֳ�ʼ��һ��Huffman�ṹ�壬��Ȩ����Ϊ0�����ҽ�㡢��Ӧ��huffmankeyֵ��Ϊ��ֵ-1
*/
void huffman_creat(huffman Huf[MAXSIZE*2-1]) {
	for (int i = 0; i < MAXSIZE; i++)//��ʼ����֪��maxsize��Ҷ��㡣
	{
		Huf[i] = { 0 , (char)i, -1, -1, -1 };
		for (int j = 0; j < 10; j++)
		{
			Huf->key[j] = '\0';
		}
	}
	for (int i = MAXSIZE; i < MAXSIZE * 2 - 1; i++)//��ʼ���Ժ�Ҫ�洢�ڽ���maxsize-1�����
	{
		Huf[i] = { 0, -1, -1, -1, -1 };
		for (int j = 0; j < 10; j++)
		{
			Huf->key[j] = '\0';
		}
	}
}

/*
����ʱ���ļ��򿪲���
ʵ�ִ��ļ��������ļ��е����ݶ���һ���㹻������飬�Ա�֮��ʹ��
*/
int openfile_code(char* values, char* Filename)
{
	FILE* fp;
	int file_size = 0;
	errno_t err = fopen_s(&fp, Filename, "rb");   //���ļ�
	if (fp == NULL)
	{
		cout << "faile to open" << endl;
	}
	else
	{
		fseek(fp, 0, SEEK_END);      //���ļ�ָ��ָ����ļ������
		file_size = ftell(fp);       //����ָ��λ�ã���ʱ��������ļ����ַ���
		fseek(fp, 0, SEEK_SET);
		memset(values, '\0', file_size * sizeof(char));  //��ʼ���˿ؼ����ݣ�������ܻ�������
		fread(values, sizeof(char), file_size, fp); //��ʼ��ȡ�����ļ�
		cout << "�ļ�����Ϊ: " <<endl<< values << endl;
		fclose(fp);
	}
	return (int)file_size;
}

/*
�ַ�Ȩ�ؼ��㹤��
ʵ�ֽ��򿪵��ļ����������ݱ���ͳ�Ƹ��ַ���Ȩ�صĲ�������Ȩ��д�뵽huffman�ṹ�������У��Ա�֮��Ľ�������
*/
void getword_weight(huffman Huf[MAXSIZE*2-1], char* values) {
	int i = 0;//ѭ��������
	int values_ascii = 0;//��¼�ַ���ASCII�룬ֱ�Ӷ�ӦHuffman������±�
	while (values[i]!='\0')
	{
		values_ascii = (int)values[i];
		Huf[values_ascii].weight++;
		i++;
	}
}

/*
Huffman������
ʵ��Huffman���Ĵ����������Ѿ��õ����ַ�Ƶ�ʵ�Huffman������д���
ʵ�ַ�������������weightֵ����ģ�����Ѱ��������СȨ�صĽ�㣬������Ӻ���Ϊ�½�㣬���޸�������С��ĸ������½���Ȩ��ֵ�������ӽڵ㡢�Լ�Ȩ��
*/
int HuffmanTree_Creat(huffman Huf[MAXSIZE * 2 - 1]) {
	int max = 0;
	for (int i = 0; i < MAXSIZE - 1; i++)
	{
		/*����MAXSIZE�飬ÿ����ѡȨ����С������*/
		/*��x1��x2����������Ԫ���������е�λ�ã����ñ���weight1��weight2���б������ǵ�Ȩ��*/
		int weight_min1, weight_min2;
		int x1, x2;
		weight_min1 = weight_min2 = WEIGHTMAXSIZE;//������Ҫ��αȽϺ�ȡСֵ�����Դ�����ĳ�ֵ�����㹻�Ĵ�
		x1 = x2 = -1;

		for (int j = 0; j < MAXSIZE + i; j++)
		{
			if (Huf[j].weight>0 &&Huf[j].weight < weight_min1 && Huf[j].pa == -1)
			//������1��.��֤�ý�������ַ����ֹ�����Ȩ�ش���0  
			//������2��.���ַ�С��weight_min1��ŵ��ַ���Ȩ��ʱ������weight_min1��ֵ
			//������3).ֻ��û�и��ڵ�Ľ�㣨��pa����Ϊ��ֵ-1�����б����ı�Ҫ
			{
				weight_min2 = weight_min1;
				weight_min1 = Huf[j].weight;
				x2 = x1;
				x1 = j;
			}
			else if (Huf[j].weight > 0 && Huf[j].weight < weight_min2 && Huf[j].pa == -1)
			//������2������weight_min1,С��weight_min2��ֻ�ƶ�weight_min2
			{
				weight_min2 = Huf[j].weight;
				x2 = j;
			}
		}
		/*�ڵ�n+i��λ���ϸ�ֵȨ�أ����������ӽڵ�λ�á�*/
		/*������������СȨ�ؽ��ĸ��ڵ㡣*/
		/*ֻ�е�x1��x2��ֵ�����ı�Ž��и�ֵ*/
		if (x1 != -1 && x2!=-1) 
		{
			max = MAXSIZE + i;
			Huf[x1].pa = MAXSIZE + i;
			Huf[x2].pa = MAXSIZE + i;
			Huf[i + MAXSIZE].weight = weight_min1 + weight_min2;
			Huf[i + MAXSIZE].ln = x1;
			Huf[i + MAXSIZE].rn = x2;
		}
	}
	return max;
}

/*
�������
ʵ�ֽ���ȡ���ļ������Huffmancode������һ��string��������
ʵ�ַ�������֪һ��Huffman���������ַ���Ӧ��ASCII�����Huffman���������Ϸ��ʸ��ڵ㡣��һ���������浱ǰλ�ã��븸�ڵ�Ƚϵõ����㻹���ҽ�㣬������Ƶõ�һ������code����
*/
int code(char* values, int length, huffman Huf[MAXSIZE * 2 - 1],string& coderesult) {
	int signal_current = 0;//��ǰ�ַ���λ��
	int signal_parent = 0;//���ڵ���ַ�λ��
	int j = 0;//code�����ļ�����
	char code[15] = { '\0' };//���ÿ���ַ���������ʺ���������

	for (int i = 0; i < length; i++)
	{
		signal_current = (int)values[i];//��õ�ǰ�ַ���ASCII��,Ҳ������Huffman���еĶ�Ӧ��λ��
		if (signal_current>255||signal_current<0)//��ֹ����ASCII��������ַ���
		{
			cout << "��ѡ����ȷ���ı�����ǰ�ı��д��ڲ��Ϸ��ַ�"<<endl;
			exit(1);
		}
		signal_parent = Huf[signal_current].pa;//��ø�����λ��

		while (signal_parent != -1) 
		//�����򸸽ڵ������������֤����ڵ㻹���ҽڵ㣬��0��1������code
		{
			if (Huf[signal_parent].ln == signal_current)
				code[j] = '0';
			else if (Huf[signal_parent].rn == signal_current)
				code[j] = '1';
			signal_current = signal_parent;
			signal_parent = Huf[signal_current].pa;
			j++;
		}
		
		for (int k = 14; k >= 0; k--)
		/*��code��������뻺��string��*/
		{
			if (code[k] != '\0')
			{
				coderesult = coderesult + code[k];
			}
		}
		for (int k = 0; k < 15; k++)//��������code[]������Ϊ��
			code[k] = '\0';
		j = 0;
	}
	return (int)coderesult.length();
}


/*
Huffman����ת���ɶ�����
ʵ�ְ����е�code����˸��ֽ�һ�Σ���Ϊһ��char�ַ��洢
ʵ�ַ�����string��ȡ�ִ���������0��1��һ���ַ�������λ����
*/
void Encoding_binary(string& code,char* encoding_binary) {
	//��Ϊ��֪������Ƿ�պ�Ϊ��λ������Ҫ�����������λ�����
	int length = (int)code.length() / 8;//ȡ�˵�����������������֮ǰ�ı���
	int final_length = (int)code.length() / 8 + (code.length() % 8 != 0);//�����������λ����ʵ�ʱ������ַ�����Ӧ�ü�һ
	string middle;//�м������ÿ�δ�string��ȡ����Ϊ8���ִ�
	while (final_length >= length)
	{
		for (int i = 0; i < length; i++)
		{
			middle = code.substr(static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(i) * 8, 8);
			for (int j = 0; j < 8; j++)
			{
				if (middle[j] == '1') encoding_binary[i] |= (1 << (7 - j));
				else if (middle[j] == '0')encoding_binary[i] &= ~(1 << (7 - j));
			}
		}
		//�����β�ַ���,������0
		if (final_length > length)
		{
			//�̳�ĩβ�Ӵ�
			string end = code.substr(static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(length) * 8, 8);
			int j = 0;
			while (end[j] != '\0' && j < 8)
			{
				if (end[j] == '1') encoding_binary[length] |= (1 << (7 - j));
				else if (end[j] == '0')encoding_binary[length] &= ~(1 << (7 - j));
				j++;
			}
			for (j; j < 8; j++)encoding_binary[length] &= ~(1 << (7 - j));//����
		}
		break;
	}
}

/*
�ļ�д��
ʵ�ֽ�ת���õĶ������ַ���д���ļ�
*/
void writefile_code(char* FILENAME, char* encoding_binary, int length) {
	FILE* fp;
	errno_t erro =  fopen_s(&fp, FILENAME, "wb+");
	if (fp == NULL) {
		cout<<"erro"<<endl;
	}
	else {
		fwrite(encoding_binary, sizeof(char), length, fp);
		fclose(fp);
	}
}

/*
��ȡ����
ʵ�ֶ�ȡ��Ҫ�����ļ������ݣ��������ļ�����
*/
int openfile_decode(char* values, char* Filename)
{
	FILE* fp;
	int file_size = 0;
	errno_t err = fopen_s(&fp, Filename, "rb");   //���ļ�
	if (fp == NULL)
	{
		cout << "faile to open" << endl;
	}
	else
	{
		fseek(fp, 0, SEEK_END);      //���ļ�ָ��ָ����ļ������
		file_size = ftell(fp);       //����ָ��λ�ã���ʱ��������ļ����ַ���
		fseek(fp, 0, SEEK_SET);
		memset(values, '\0', file_size * sizeof(char));  //��ʼ���˿ؼ����ݣ�������ܻ�������
		fread(values, sizeof(char), file_size, fp); //��ʼ��ȡ�����ļ�
		//cout << "�ļ�����Ϊ: " << endl << values << endl;
		fclose(fp);
	}
	return (int)file_size;
}

/*
�������ַ�ת����char
*/
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

/*
����
*/
void decode(char* values, int length, huffman Huf[MAXSIZE * 2 - 1],int max) {
	int m = max;
	string num = { '\0' };
	decode_key(values, length, num);
	//cout << num;

	for (int i = 0; i < length * 8+1; i++)
	{
		if (num[i] == '0')
		{
			//cout << "0";
			m = Huf[m].ln;
		}
		else if (num[i] == '1')
		{
			//cout << "1";
			m = Huf[m].rn;
		}
		if (Huf[m].ln == -1 && Huf[m].rn == -1)
		{
			//cout << " ";
			cout << Huf[m].value;
			//cout << endl;
			m = max;//��m��Ϊ�Ѷ�λ�ã��Ա����±�����
		}
	}

}

void use_code() {
	int word_length = 0;//�ļ��ַ�����
	int code_length = 0;//���볤��
	int max = 0;//�Ѷ�Ԫ�ػ�ȡ
	char filename[] = { "1.txt" };
	char filename_decode[] = { "2.txt" };
	char* values= new char[ARRAYSIZE]{ '\0' };
	huffman Huf[MAXSIZE * 2 - 1] = { 0 };//����Huffman���Ĵ洢����
	string coderesult;//��ű�����

	huffman_creat(Huf);//��ʼ��Huffman��
	word_length = openfile_code(values,filename);//��ȡ�ļ���ȡ���ַ������Ա�֮��ĺ�������
	getword_weight(Huf, values);//������ȡȨֵ
	max = HuffmanTree_Creat(Huf);//���������õ�����Ԫ��
	code_length = code(values, word_length, Huf, coderesult);//����õ�0��1�ַ������������ַ�������
	//cout << coderesult << endl;
	code_length = code_length / 8 + (code_length % 8 != 0);//����ԭ����ת���ɶ����ƺ���ַ������Ա㴴����ά����
	char* encoding_binary = new char[code_length+1]();//����һ������
	Encoding_binary(coderesult,encoding_binary);//��0��1�ַ���ת��Ϊ�ַ�����
	writefile_code(filename_decode,encoding_binary,code_length);//д���ļ�
	delete[] encoding_binary;//�ͷ��ڴ�

	/*for (int i = 0; i < MAXSIZE * 2 - 1; i++)
{
	cout.width(6);
	cout << Huf[i].value;
	cout.width(4);
	cout << i<<"����";
	cout.width(6);
	cout << Huf[i].weight;
	cout.width(4);
	cout << Huf[i].ln;
	cout.width(4);
	cout << Huf[i].rn;
	cout.width(4);
	cout << Huf[i].pa;
	cout << endl;
}*/


	cout << "��������Ϊ��" << endl;
	int length_decode = 0;
	length_decode = openfile_decode(values,filename_decode);
	decode(values,length_decode,Huf,max);
	delete[]  values;
}

int main() {
	use_code();;
}

