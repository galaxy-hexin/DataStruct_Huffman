#pragma once
#include<iostream>
#include"Pre_Huffman.h"

/*
��ȡ����
ʵ�ֶ�ȡ��Ҫ�����ļ������ݣ��������ļ����ĳ���
*/
int openfile_decode(huffman Huf[MAXSIZE * 2 - 1], char* values, char* Filename)
{
	FILE* fp;
	int file_size_current = 0;
	int file_size_length = 0;
	int file_size_word = 0;
	int weight = 0;
	char word = '\0';
	int i = 0;//��ȡ�ֵ������
	errno_t err = fopen_s(&fp, Filename, "rb+");   //���ļ�
	if (fp == NULL)
	{
		cout << "faile to open" << endl;
		return -1;
	}
	else
	{
		fseek(fp, 0, SEEK_END);      //���ļ�ָ��ָ����ļ������
		file_size_length = ftell(fp);       //����ָ��λ�ã���ʱ��������ļ��ĳ���
		fseek(fp, 0, SEEK_SET);
		fread(&weight, sizeof(int), 1, fp);
		fread(&word, sizeof(char), 1, fp);
		while (weight != 0 && word != '\0')//�ж��Ƿ񵽴�д��ı߽��ߣ������ֵ�
		{
			Huf[(int)word].weight = weight;
			fread(&weight, sizeof(int), 1, fp);
			fread(&word, sizeof(char), 1, fp);
		}
		file_size_current = ftell(fp);
		file_size_word = file_size_length - file_size_current;//���������ַ��ĸ���
		memset(values, '\0', file_size_word * sizeof(char));  //��ʼ���˿ؼ����ݣ�������ܻ�������
		fread(values, sizeof(char), file_size_word, fp); //��ʼ��ȡ���Ĳ���
		fclose(fp);
	}
	return (int)file_size_word;
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
bool decode(char* values, int length, huffman Huf[MAXSIZE * 2 - 1], int max, string& decode_word) {
	int m = max;
	string num = { '\0' };
	decode_key(values, length, num);
	//cout << num;

	for (int i = 0; i < length * 8; i++)
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
			decode_word += Huf[m].value;
			//cout << Huf[m].value;
			//cout << endl;
			m = max;//��m��Ϊ�Ѷ�λ�ã��Ա����±�����
		}
	}
	return true;
}
/*
�����ļ�д�����
*/
bool writefile_decode(char* FILENAME, string& decode_word) {
	FILE* fp;
	errno_t erro = fopen_s(&fp, FILENAME, "wb");
	if (fp == NULL) {
		cout << "erro" << endl;
		return false;
	}
	else {
		int length = 0;
		length = (int)decode_word.length();
		char* value = new char[length]();
		for (int i = 0; i < length; i++)
		{
			value[i] = decode_word[i];
		}
		fwrite(value, sizeof(char), length, fp);
		fclose(fp);
		delete[] value;
		return true;
	}
}

/*
��ɽ�ѹ������ۺ�ʵ�ֺ͵���
*/
bool use_decode(char* values, char* FILENAME, char* FILENAME_target) {
	int max = 0;
	int length_decode = 0;
	huffman Huf[MAXSIZE * 2 - 1];
	string decode_word;
	//1.��ʼ��һ��huffman�ṹ������
	if(huffman_creat(Huf))
		cout << "��ʼ���ɹ���..." << endl;
	//2.��Ŀ���ļ�����ȡ�ֵ䵽Huffman���飬�����ؽ�ѹ���ݼ��䳤��
	length_decode = openfile_decode(Huf, values, FILENAME);
	if (length_decode == -1)
	{
		cout << "�ļ������ڣ���������Ч�����ļ�";
		return false;
	}
	else
		cout << "�ļ��򿪳ɹ���.." << endl;
	//3.�ؽ�Huffman���������ضѶ�Ԫ��λ��
	max = HuffmanTree_Creat(Huf);
	if(max!=0)
		cout << "Huffman���ؽ��ɹ���..." << endl;
	//4.����
	if(decode(values, length_decode, Huf, max, decode_word))
		cout << "��ѹ�ɹ�������д��Ŀ���ļ�..." << endl;
	//5.д������ļ�
	if(writefile_decode(FILENAME_target, decode_word))
		cout << "��ɽ�ѹ��" << endl;
	return true;
}