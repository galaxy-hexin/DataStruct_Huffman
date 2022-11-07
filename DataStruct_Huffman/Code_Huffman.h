#pragma once
#include<iostream>
#include"Pre_Huffman.h"

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
		return -1;
	}
	else
	{
		fseek(fp, 0, SEEK_END);      //���ļ�ָ��ָ����ļ������
		file_size = ftell(fp);       //����ָ��λ�ã���ʱ��������ļ����ַ���
		fseek(fp, 0, SEEK_SET);
		memset(values, '\0', file_size * sizeof(char));  //��ʼ���˿ؼ����ݣ�������ܻ�������
		fread(values, sizeof(char), file_size, fp); //��ʼ��ȡ�����ļ�
		//cout << "�ļ�����Ϊ: " <<endl<< values << endl;
		fclose(fp);
	}
	return (int)file_size;
}
/*
�ַ�Ȩ�ؼ��㹤��
ʵ�ֽ��򿪵��ļ����������ݱ���ͳ�Ƹ��ַ���Ȩ�صĲ�������Ȩ��д�뵽huffman�ṹ�������У��Ա�֮��Ľ�������
*/
bool getword_weight(huffman Huf[MAXSIZE * 2 - 1], char* values) {
	int i = 0;//ѭ��������
	int values_ascii = 0;//��¼�ַ���ASCII�룬ֱ�Ӷ�ӦHuffman������±�
	while (values[i] != '\0')
	{
		values_ascii = (int)values[i];
		Huf[values_ascii].weight++;
		i++;
	}
	return true;
}
/*
�������
ʵ�ֽ���ȡ���ļ������Huffmancode������һ��string��������
ʵ�ַ�������֪һ��Huffman���������ַ���Ӧ��ASCII�����Huffman���������Ϸ��ʸ��ڵ㡣��һ���������浱ǰλ�ã��븸�ڵ�Ƚϵõ����㻹���ҽ�㣬������Ƶõ�һ������code����
*/
int code(char* values, int length, huffman Huf[MAXSIZE * 2 - 1], string& coderesult) {
	int signal_current = 0;//��ǰ�ַ���λ��
	int signal_parent = 0;//���ڵ���ַ�λ��
	int j = 0;//code�����ļ�����
	char code[20] = { '\0' };//���ÿ���ַ���������ʺ���������

	for (int i = 0; i < length; i++)
	{
		signal_current = (int)values[i];//��õ�ǰ�ַ���ASCII��,Ҳ������Huffman���еĶ�Ӧ��λ��
		if (signal_current > 128 || signal_current < 0)//��ֹ����ASCII��������ַ���
		{
			cout << "��ѡ����ȷ���ı�����ǰ�ı��д��ڲ��Ϸ��ַ�" << endl;
			exit(1);
		}
		while (Huf[signal_current].key[j] != '\0')
		{
			coderesult += Huf[signal_current].key[j];
			j++;
		}
		j = 0;
	}
	return (int)coderesult.length();
}
/*
Huffman����ת���ɶ�����
ʵ�ְ����е�code����˸��ֽ�һ�Σ���Ϊһ��char�ַ��洢
ʵ�ַ�����string��ȡ�ִ���������0��1��һ���ַ�������λ����
*/
bool Encoding_binary(string& code, char* encoding_binary) {
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
	return true;
}
/*
�ļ�д��
ʵ�ֽ�ת���õĶ������ַ���д���ļ�
*/
bool writefile_code(char* FILENAME, huffman Huf[MAXSIZE * 2 - 1], char* encoding_binary, int length) {
	FILE* fp;
	errno_t erro = fopen_s(&fp, FILENAME, "wb+");
	int weight = 0;
	char word = '\0';
	if (fp == NULL) {
		cout << "empty" << endl;
		return false;
	}
	else {
		for (int i = 0; i < MAXSIZE; i++)//��������д����Ȩ�ص��ַ���Ȩ�غ��ַ�ֵ���Լ�С�ڴ�
		{
			if (Huf[i].weight != 0) {
				weight = Huf[i].weight;
				word = Huf[i].value;
				fwrite(&weight, sizeof(int), 1, fp);
				fwrite(&word, sizeof(char), 1, fp);
			}
		}
		int weight = 0;
		char word = '\0';//�൱���������ֵ������ĵı߽���
		fwrite(&weight, sizeof(int), 1, fp);
		fwrite(&word, sizeof(char), 1, fp);
		fwrite(encoding_binary, sizeof(char), length, fp);
		fclose(fp);
		return true;
	}
}

/*
���ѹ��������ۺ�ʵ�������
*/
bool use_code(char* values, char* filename, char* filename_decode) {
	int word_length = 0;//�ļ��ַ�����
	int code_length = 0;//���볤��
	huffman Huf[MAXSIZE * 2 - 1] = { 0 };//����Huffman���Ĵ洢����
	string coderesult;//��ű�����

	//1.��ʼ��Huffman��
	if(huffman_creat(Huf))
		cout << "��ʼ������Huffman����ɹ�..." << endl;
	//2.��ȡ�ļ����������ļ���ȡ���ַ������Ա�֮��ĺ�������
	word_length = openfile_code(values, filename);
	if(word_length == -1) 
	{
		cout << "�ļ������ڣ���������Ч�����ļ�";
		return false;
	}
	else
		cout << "�ļ��򿪳ɹ���..." << endl;
	//3.������ȡȨֵ
	if (getword_weight(Huf, values))
		cout << "���ȫ�ı�����..." << endl;
	//4.������
	if(HuffmanTree_Creat(Huf)!=0)
		cout << "�ѽ���Huffman����..." << endl;
	//5.����õ�0��1�ַ������������ַ�������
	code_length = code(values, word_length, Huf, coderesult);
	//6.����ԭ����ת���ɶ����ƺ���ַ�������������̬��ά����
	code_length = code_length / 8 + (code_length % 8 != 0);
	char* encoding_binary = new char[(double)code_length + 1]();
	//7.��0��1�ַ���ת��Ϊ�ַ�����
	if(Encoding_binary(coderesult, encoding_binary))
		cout << "����ɹ�������д��Ŀ��..." << endl;
	//8.д���ļ�
	if(writefile_code(filename_decode, Huf, encoding_binary, code_length))
		cout << "ѹ���ɹ���" << endl;
	delete[] encoding_binary;//�ͷ��ڴ�
	return true;
}